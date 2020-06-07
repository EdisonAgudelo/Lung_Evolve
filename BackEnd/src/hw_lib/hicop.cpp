

#include "hicop.h"
#include "hicop_ll.h"
#include "time.h"
#include "low_level/hardware_interface.h"

#define HICOP_MAX_DATA_LENGTH 250
#define HICOP_STACK_SIZE 5
#define HICOP_MAX_RESPONSE_TIME 100 //ms
#define HICOP_MAX_SEND_TRIES 10    //times

//defines a buffer for tx request
typedef struct
{
    bool data_sent;                      //says if data was sent or not
    bool repeat_in_nack;                 //what should do on nack response
    uint8_t fail_count;                  //saves how many times transmission failed
    HicopHeaders header_type;            //what kind of header is
    uint8_t data_count;                  //data lenght
    uint8_t data[HICOP_MAX_DATA_LENGTH]; //store pending data
} HicopTxStackType;

typedef struct
{
    HicopHeaders header_type;            //what kind of header is
    uint8_t data_count;                  //data lenght
    uint8_t data[HICOP_MAX_DATA_LENGTH]; //store received data
} HicopRxStackType;

typedef struct
{
    uint8_t pos_head; //FIFO architecture
    uint8_t pos_tail;
    uint8_t stack_pending;
} HicopStack;

HicopStack g_tx_stack_pointer;
HicopStack g_rx_stack_pointer;
HicopTxStackType g_tx_stack_buffer[HICOP_STACK_SIZE];
HicopRxStackType g_rx_stack_buffer[HICOP_STACK_SIZE];

static void HicopStackRxPush(uint8_t *data, uint8_t lengh, HicopHeaders type);
static void HicopStackTxPop(void);

void HicopInit(void)
{
    HicopLLBegin();
    g_tx_stack_pointer.pos_head = 0;
    g_tx_stack_pointer.pos_tail = 0;
    g_tx_stack_pointer.stack_pending = 0;
    g_rx_stack_pointer.pos_head = 0;
    g_rx_stack_pointer.pos_tail = 0;
    g_rx_stack_pointer.stack_pending = 0;
}

void HicopLoop(void)
{
    //says when a flag is received
    bool rx_was_flag = false;
    //saves the value of flag received
    uint8_t *was_ack;
    //to avoid infinite ping pong response
    static uint8_t consecutive_nack = 0;
    //to take a maximum response time
    static uint32_t ref_time = 0;
    //for optimization at send moment
    static bool payload_change = true; //if true it means that original tx payload was over write by an ACK or NACK response or newer data

    //if a data is received
    if (HicopLLIsReceptionComplete())
    {
        //check data integrity
        if (HicopLLIsValidData())
        {
            consecutive_nack = 0; //reset counter

            switch (HicopLLGetHeader())
            {
            case kHicopHeaderData:
            case kHicopHeaderAlarm:
            case kHicopHeaderConfig:
                //make a copy of new data on stack
                HicopStackRxPush(HicopLLGetPayload(), HicopLLGetLength(), (HicopHeaders)HicopLLGetHeader());

                //clean up buffer for new reception
                HicopLLCleanBufferRx();

                //answer with and ACK
                HicopLLSetHeader(kHicopHeaderFlags);
                HicopLLAddPayload(kHicopAck);
                HicopLLSendPayload();
                payload_change = true;

                return;
            case kHicopHeaderFlags:
                //save values
                rx_was_flag = true;
                was_ack = HicopLLGetPayload();

                //clean up buffer for new reception
                HicopLLCleanBufferRx();

                break;
            default:
                break;
            }
        }
        else //it can't understand what other say
        {
            //clean up buffer for new reception
            HicopLLCleanBufferRx();

            if (consecutive_nack < 5)
            {
                consecutive_nack++;
                HicopLLSetHeader(kHicopHeaderFlags);
                HicopLLAddPayload(kHicopNack);
                HicopLLSendPayload();
                payload_change = true;
                return;
            }
        }
    }

    //this function only reach this point if data was no received or rx data is a flag type or if received data
    //has 5 o more consecutive errors.

    //check if there is pending data
    if (g_tx_stack_pointer.stack_pending != 0)
    {
        //if data has not be sent
        if (!g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].data_sent)
        {

            g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].data_sent = true;

            if (payload_change)
            {
                HicopLLSetHeader(g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].header_type);
                HicopLLAddPayload(g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].data, g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].data_count);
                HicopLLSendPayload();
                payload_change = false;
            }
            else
                HicopLLResendPayload();

            //reset counter
            consecutive_nack = 0; 
            ref_time = Millis();

            return;
        }
        //data is waiting for response and arrive a flag
        else if (rx_was_flag)
        {
            if (kHicopAck == *was_ack)
            {
                HicopStackTxPop(); //continue with the next one
                payload_change = true;
                return;
            }
        }
        //if the passing time is to short from data request
        else if (GetDiffTime(Millis(), ref_time) < HICOP_MAX_RESPONSE_TIME)
        {
            //wait a moment please
            return;
        }

        //function only reach this point if response is a nack or there is a timeout

        //count one error
        g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].fail_count++;

        //if data should be resend
        if (g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].repeat_in_nack &&
            g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].fail_count < HICOP_MAX_SEND_TRIES)
        {
            g_tx_stack_buffer[g_tx_stack_pointer.pos_tail].data_sent = false; //request other data send
        }
        else{
            HicopStackTxPop(); //continue with the next one
            payload_change = true;
        }
    }
}

//----------User methods---------//

bool HicopSendData(HicopHeaders type, uint8_t *data, uint8_t lengh)
{
    uint8_t i = 0;
    //if no space available
    if (g_tx_stack_pointer.stack_pending >= HICOP_STACK_SIZE)
        return false;

    //if it is the first place on buffer, do not increment pointer
    if (g_tx_stack_pointer.stack_pending != 0)
    {
        g_tx_stack_pointer.pos_head++;
        if (g_tx_stack_pointer.pos_head >= HICOP_STACK_SIZE)
            g_tx_stack_pointer.pos_head = 0;
    }

    //save params
    g_tx_stack_buffer[g_tx_stack_pointer.pos_head].data_count = lengh;
    while (lengh--)
    {
        g_tx_stack_buffer[g_tx_stack_pointer.pos_head].data[i] = data[i];
        i++;
    }

    g_tx_stack_buffer[g_tx_stack_pointer.pos_head].header_type = type;

    g_tx_stack_buffer[g_tx_stack_pointer.pos_head].data_sent = false;
    g_tx_stack_buffer[g_tx_stack_pointer.pos_head].fail_count = 0;

    if (type == kHicopHeaderAlarm || type == kHicopHeaderConfig)
        g_tx_stack_buffer[g_tx_stack_pointer.pos_head].repeat_in_nack = true;
    else
        g_tx_stack_buffer[g_tx_stack_pointer.pos_head].repeat_in_nack = false;

    //indicate new data
    g_tx_stack_pointer.stack_pending++;

    return true;
}

bool HicopReadData(HicopHeaders *type, uint8_t *data, uint8_t *lengh)
{
    uint8_t i = 0;
    if (g_rx_stack_pointer.stack_pending == 0)
        return false;

    for (i = 0; i < g_rx_stack_buffer[g_rx_stack_pointer.pos_tail].data_count; i++)
        data[i] = g_rx_stack_buffer[g_rx_stack_pointer.pos_tail].data[i];

    *type = g_rx_stack_buffer[g_rx_stack_pointer.pos_tail].header_type;

    *lengh = g_rx_stack_buffer[g_rx_stack_pointer.pos_tail].data_count;

    g_rx_stack_pointer.stack_pending--;
    if (g_rx_stack_pointer.stack_pending != 0)
    {
        g_rx_stack_pointer.pos_tail++;
        if (g_rx_stack_pointer.pos_tail >= HICOP_STACK_SIZE)
            g_rx_stack_pointer.pos_tail = 0;
    }

    return true;
}

//----------Raw methods----------//

//save data on rx buffer
void HicopStackRxPush(uint8_t *data, uint8_t lengh, HicopHeaders type)
{
    uint8_t i = 0;

    //if there are more data on buffer
    if (g_rx_stack_pointer.stack_pending != 0)
    {
        g_rx_stack_pointer.pos_head++;
        if (g_rx_stack_pointer.pos_head >= HICOP_STACK_SIZE)
            g_rx_stack_pointer.pos_head = 0;

        //if head reach tail, over write data and move tail counter but no incremente pending counter
        if (g_rx_stack_pointer.pos_head == g_rx_stack_pointer.pos_tail)
        {
            g_rx_stack_pointer.pos_tail++;
            if (g_rx_stack_pointer.pos_tail >= HICOP_STACK_SIZE)
                g_rx_stack_pointer.pos_tail = 0;
        }
        else
            g_rx_stack_pointer.stack_pending++;
    }
    else
        g_rx_stack_pointer.stack_pending++;

    //save parameters
    g_rx_stack_buffer[g_rx_stack_pointer.pos_head].data_count = lengh;
    while (lengh--)
    {
        g_rx_stack_buffer[g_rx_stack_pointer.pos_head].data[i] = data[i];
        i++;
    }

    g_rx_stack_buffer[g_rx_stack_pointer.pos_head].header_type = type;
}

void HicopStackTxPop(void)
{
    if (0 != g_tx_stack_pointer.stack_pending)
    {
        g_tx_stack_pointer.stack_pending--;
        if (g_tx_stack_pointer.stack_pending != 0)
        {
            g_tx_stack_pointer.pos_tail++;
            if (g_tx_stack_pointer.pos_tail >= HICOP_STACK_SIZE)
                g_tx_stack_pointer.pos_tail = 0;
        }
    }
}