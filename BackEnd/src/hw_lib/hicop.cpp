

#include "hicop.h"

#include "low_level/hardware_interface.h"
#include "time.h"
#include "../fw_lib/crc8.h"

#include <stdint.h>

#define HICOP_BUFFER_LENGTH 255
#define SERIAL_BAUDRATE 115200
#define HICOP_INVALID_HEADER 0xff
#define HICOP_MAX_RX_TIME 100 //ms
#define HICOP_CRC8_POLY 0x31
#define HICOP_CRC8_INIT 0x00

struct HicopManagement
{
    uint16_t prev_pointer_last; //do some backup to know how many data was before buffer clean up
    uint16_t pointer_last; //saves the actual buffer pos
    uint32_t time_stamp;
    union {

        //buffer transaction
        struct
        {
            uint8_t header; //type of data
            uint8_t length;
            uint8_t payload[HICOP_BUFFER_LENGTH - 2];
        };
        uint8_t exchange[HICOP_BUFFER_LENGTH]; //general buffer declaration
    };

} g_hicop_tx, g_hicop_rx;

static void HicopReceptionEvent(void);
static bool HicopSendBuffer(void);
static bool HicopWriteBuffer(uint8_t data_out);

void HicopBegin(void)
{
    UartBegin(SERIAL_BAUDRATE);
    UartConfigCallback(HicopReceptionEvent);
    g_hicop_rx.pointer_last = 0;
    g_hicop_tx.pointer_last = 0;
}

///////////// Rx Methods ////////////

bool HicopIsReceptionComplete(void)
{
    //if no data, no reception
    if (g_hicop_rx.pointer_last == 0)
        return false;

    //if there is a complete frame
    if (g_hicop_rx.pointer_last >= (g_hicop_rx.length + 4))
        return true;

    //if no more data is received
    if (GetDiffTime(Millis(), g_hicop_rx.time_stamp) > HICOP_MAX_RX_TIME)
        return true;

    return false;
}

void HicopCleanBufferRx(void)
{
    //reset buffer pointer
    g_hicop_rx.prev_pointer_last = g_hicop_rx.pointer_last;
    g_hicop_rx.pointer_last = 0;
}

uint8_t HicopGetHeader(void)
{
    //return header info if there are enough buffer data
    if (g_hicop_rx.pointer_last < 1)
        return HICOP_INVALID_HEADER;

    return g_hicop_rx.header;
}

uint8_t HicopGetLength(void)
{
    //return length info if there are enough buffer data
    if (g_hicop_rx.pointer_last < 2)
        return 0;
    return g_hicop_rx.length;
}

uint8_t *HicopGetPayload(void)
{
    //return data on payloda under user responsibility
    return &g_hicop_rx.payload[0];
}

bool HicopIsValidData(void)
{
    //check if there is data
    if (g_hicop_rx.pointer_last == 0)
        return false;

    // check if there is enough data
    if (!((g_hicop_rx.length + 4) == g_hicop_rx.pointer_last))
        return false;

    //check if the last character is the expected
    if (g_hicop_rx.exchange[g_hicop_rx.pointer_last - 1] != 0xff)
        return false;

    //then check if CRC is OK
    CRC8Configure(HICOP_CRC8_POLY, HICOP_CRC8_INIT);
    if (g_hicop_rx.exchange[g_hicop_rx.pointer_last - 2] != CRC8Calculate(&g_hicop_rx.exchange[0], g_hicop_rx.length + 2))
        return false;

    //if everything is OK
    return true;
}

///////////// Tx Methods ////////////

void HicopSetHeader(uint8_t header)
{
    //new data will be loaded
    g_hicop_tx.length = 0;
    //confing requested header
    g_hicop_tx.header = header;
    //update buffer
    g_hicop_tx.pointer_last = 2;
}

bool HicopAddPayload(uint8_t *payload, uint8_t length)
{
    uint16_t i = 0;

    //do anything
    if (length == 0)
        return false;

    while (length--)
    {
        if (HicopWriteBuffer(payload[i++]))
            g_hicop_tx.length++;
        else
            return false; //if no more data can be added
    }

    return true;
}

bool HicopSendPayload(void)
{
    //compute data from de begining to actual date
    CRC8Configure(HICOP_CRC8_POLY, HICOP_CRC8_INIT);
    if (!HicopWriteBuffer(CRC8Calculate(&g_hicop_tx.exchange[0], g_hicop_tx.pointer_last)))
        return false;
    //add end byte
    if (HicopWriteBuffer(0xff))
        return false;

    g_hicop_tx.prev_pointer_last=g_hicop_tx.pointer_last;
    HicopSendBuffer();

    return true;
}

bool HicopResendPayload(void)
{
    //make sure tha there was a previous frame
    if(g_hicop_tx.prev_pointer_last<4)
        return false;

    g_hicop_tx.pointer_last=g_hicop_tx.prev_pointer_last;
    
    HicopSendBuffer();
    return true;
}

////////////// Raw Methods ///////////////

bool HicopWriteBuffer(uint8_t data_out)
{
    if (g_hicop_tx.pointer_last >= HICOP_BUFFER_LENGTH)
        return false;

    g_hicop_tx.exchange[g_hicop_tx.pointer_last++] = data_out;

    return true;
}

bool HicopSendBuffer(void)
{
    if (0 == g_hicop_tx.pointer_last)
        return false;

    g_hicop_tx.time_stamp = Millis();
    while (g_hicop_tx.pointer_last--)
    {
        UartWrite(g_hicop_tx.exchange[g_hicop_tx.pointer_last]);
    }
    return true;
}

static void HicopReceptionEvent(void)
{
    g_hicop_rx.time_stamp = Millis();
    while (UartAvailable())
    {
        g_hicop_rx.exchange[g_hicop_rx.pointer_last++] = UartRead();
    }
}

