

#include "heltec.h"
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  10        /* Time ESP32 will go to sleep (in seconds) */
#define BUTTON_PIN_BITMASK 0x300000000    //For GPIO33 & 32 as external wakeup 

#define CONTINUOUS_MODE true
RTC_DATA_ATTR int bootCount = 0;



#define BAND    915E6  //you can set band here directly,e.g. 868E6,915E6

int const level[125]   =          {18018, 17820, 17622, 17424, 17226, 17028, 16830, 16632, 16434, 16236,
                                  16038, 15840, 15642, 15642, 15444, 15246, 15048, 14850, 14652, 14454,
                                  14256, 14058, 13860, 13662, 13662, 13464, 13266, 13068, 12870, 12672,
                                  12474, 12474, 12276, 12078, 11880, 11682, 11484, 11484, 11286, 11088,
                                  10890, 10692, 10692, 10494, 10296, 10098, 9900, 9900, 9702, 9504,
                                  9306, 9306, 9108, 8910, 8712, 8712, 8514, 8316, 8316, 8118,
                                  7920, 7722, 7722, 7524, 7326, 7326, 7128, 6930, 6732, 6732, 
                                   6534, 6336, 6336, 6138, 5940, 5940, 5742, 5544, 5544, 5346,
                                   5148, 5148, 4950, 4752, 4752, 4554, 4554, 4356, 4158, 4158,
                                   3960, 3762, 3762, 3762, 3564, 3564, 3366, 3168, 3168, 2970,
                                   2970, 2772, 2574, 2376, 2178, 1980, 1980, 1782, 1782, 1584, 
                                   1584, 1386, 1188, 1188, 990, 990, 792, 792, 594, 594,
                                   396, 396, 198, 000, 000};
                                   

// this structure must be shared across all nodes
typedef struct TankLevelPayloads {
  int adc_value;
  byte switch_value;
  int inches;
} TankLevelPayload;

// message types IDs
#define MSG_TYPE_DATA 0x1F
#define MSG_TYPE_OTHERS 0x20

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;
int adc_value=0;
int in_pin = 12;
int val;


////////////Inches variable/////////
int inches,adc_value1,adc_value2,adc_value3,adc_value4,adc_value5;

void readAndDisplayData() {
  //WIFI Kit series V1 not support Vext control
 

  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  adc_value =    analogRead(13);
  adc_value = adc_value/32;

  inches=level[adc_value];
  inches=inches*0.01;
  val = digitalRead(in_pin);
if (val==1){  
  Heltec.display->drawString(0, 40, "Received High ");
  Heltec.display->drawString(90, 40, "On Pin 12");
  Heltec.display->display();
}




  Heltec.display->drawString(0, 0, "Sensor Value: ");
  Heltec.display->drawString(90, 0, String(adc_value));
  Heltec.display->display();
  Heltec.display->drawString(0, 20, "Inches: ");
  Heltec.display->drawString(90, 20, String(inches));
  Heltec.display->display();
  TankLevelPayload payload;
  payload.adc_value = adc_value;
  payload.switch_value = val;
  payload.inches = inches;

  uint8_t serializedPayload[sizeof(TankLevelPayload) + 1];
  memcpy(serializedPayload, &payload, sizeof(payload));
  // send packet
  LoRa.beginPacket();
  
/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  // LoRa.print("Sensor Value");
  // LoRa.println(adc_value);
  // LoRa.print("Switch Value");
  // LoRa.println(val);
  LoRa.write(MSG_TYPE_DATA);
  LoRa.write((uint8_t*)&serializedPayload, sizeof(payload));
  LoRa.endPacket();
  LoRa.end();

}

void setup()
{
   
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  pinMode(in_pin,INPUT);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
      Heltec.display->clear();

  delay(100);
  delay(100);

}

void loop()
{
//  readAndDisplayData();
  Heltec.display->clear();
    Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  adc_value =    analogRead(13);
  adc_value = adc_value/32;

  inches=level[adc_value];
  inches=inches*0.01;
  val = digitalRead(in_pin);
if (val==1){  
  Heltec.display->drawString(0, 40, "Received High ");
  Heltec.display->drawString(90, 40, "On Pin 12");
  Heltec.display->display();
}




  Heltec.display->drawString(0, 0, "Sensor Value: ");
  Heltec.display->drawString(90, 0, String(adc_value));
  Heltec.display->display();
  Heltec.display->drawString(0, 20, "Inches: ");
  Heltec.display->drawString(90, 20, String(inches));
  Heltec.display->display();
  TankLevelPayload payload;
  payload.adc_value = adc_value;
  payload.switch_value = val;
  payload.inches = inches;

  uint8_t serializedPayload[sizeof(TankLevelPayload) + 1];
  memcpy(serializedPayload, &payload, sizeof(payload));
  // send packet
  LoRa.beginPacket();
  
/*
 * LoRa.setTxPower(txPower,RFOUT_pin);
 * txPower -- 0 ~ 20
 * RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
 *   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
 *   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  // LoRa.print("Sensor Value");
  // LoRa.println(adc_value);
  // LoRa.print("Switch Value");
  // LoRa.println(val);
  LoRa.write(MSG_TYPE_DATA);
  LoRa.write((uint8_t*)&serializedPayload, sizeof(payload));
  LoRa.endPacket();
  
  delay(2000);
}