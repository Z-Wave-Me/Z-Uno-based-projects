
#include "EEPR.h"

#define VALVE_CTRL_PIN 1
#define VALVE_BUT_PIN 0
#define VALVE_LED_PIN 9
#define VALVE_CLOSE 255

#define ZUNO_CHANNEL_SWITCH 1
ZUNO_SETUP_CHANNELS(ZUNO_SWITCH_BINARY(getterValve, setterValve));

uint8_t valve_state = VALVE_CLOSE;

#define LED_ON LOW
#define LED_OFF HIGH
void valve_set(uint8_t state)
{
  digitalWrite(VALVE_CTRL_PIN, state);
  digitalWrite(VALVE_LED_PIN, state);
}

void setup() 
{
  Serial.begin();
//  Serial.println(sizeof(unsigned long));
  Serial.println("Serial init");
  pinMode(VALVE_CTRL_PIN, OUTPUT); 
  pinMode(VALVE_BUT_PIN, INPUT); 
  pinMode(VALVE_LED_PIN, OUTPUT);
  valve_set(LED_OFF);

  init_EEPROM();  
}

void loop() 
{
  check_btn();
  valve_set(valve_state);
  check_time();
}


void check_time()
{
  static uint8_t t_mins=0;
  static uint8_t t_hours=0;
  static unsigned long start_time;
  start_time = millis();
}


#define RELE_OFF 0
#define RELE_ON 255

void change_rele_st()
{
  if(valve_state == RELE_ON)
    valve_state = RELE_OFF;
  else
    valve_state = RELE_ON; 
  zunoSendReport(ZUNO_CHANNEL_SWITCH);  
}

uint8_t last_btn_st = HIGH;
void check_btn()
{
  if(digitalRead(VALVE_BUT_PIN) == LOW)
  {
    if(last_btn_st != LOW)
    {
      for(uint8_t i=0; i<10; ++i)
      {
        if(digitalRead(VALVE_BUT_PIN) == LOW)
          delay(5);
        else
          return;
      }
      last_btn_st = LOW;
    }
  }
  else
  {
    if(last_btn_st == LOW)
    {
      for(uint8_t i=0; i<10; ++i)
      {
        if(digitalRead(VALVE_BUT_PIN) == HIGH)
          delay(5);
        else
          return;
      }
      last_btn_st = HIGH;
      change_rele_st();
    }
  }  
}


BYTE getterValve() 
{
    return valve_state;
}

void setterValve(BYTE newState) 
{
  valve_state = newState;
}
