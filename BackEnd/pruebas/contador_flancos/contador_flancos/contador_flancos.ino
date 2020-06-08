
#define PIN_EN 7
#define PIN_STEP 2

uint32_t time_ref;
uint32_t cuentas=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_EN, INPUT);
  pinMode(PIN_STEP, INPUT);
  Serial.begin(115200);
  time_ref = millis();
  attachInterrupt(digitalPinToInterrupt(PIN_STEP), count, RISING);
}

void loop() {

  if(!digitalRead(PIN_EN))
  {
    if(millis()-time_ref>1000)
    {
      time_ref = millis();
      Serial.println(cuentas);
    }
  }
}

void count(void)
{
  cuentas++;
}
