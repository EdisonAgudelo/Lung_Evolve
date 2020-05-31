
#define PIN_EN 7
#define PIN_STEP 8

uint32_t time_ref;
uint32_t cuentas=0;
bool level=false;

void setup() {
  // put your setup code here, to run once:
  pinMode(PIN_EN, INPUT);
  pinMode(PIN_STEP, INPUT);
  Serial.begin(115200);
  time_ref = millis();
}

void loop() {

  if(digitalRead(PIN_EN))
  {
    if(digitalRead(PIN_STEP)!=level)
    {
      level = digitalRead(PIN_STEP);
      cuentas++;
    }
  }
  else
  {
      // put your main code here, to run repeatedly:
  if(millis()-time_ref>1000)
  {
    time_ref = millis();
    Serial.println(cuentas/2);
  }
    level = false;
  }
}
