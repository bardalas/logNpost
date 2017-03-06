#include <avr/wdt.h> //for Watchdog reset

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <FreematicsONE.h>
#include <TinyGPS.h>


COBDSPI one;
TinyGPS gps;
CMPU9250 cmpu;
//CMPU6050 cmpu;


#include "config.h"
#include "gsm.h"
#include "gps.h"
//#include "acc.h"
#include "gprs.h"


void setup() {

  Serial.begin(115200);
  Serial.setTimeout(100);
  Serial.println(MSG_HELLO);

  Wire.begin(); // start I2C communication (MEMS)
  one.begin(); //start FreematicsONE and get Firmware vewrsion.

  delay(1000);

  if (!initGPS())   reset();
  if (!initGSM())   reset();
  if (!initGPRS())  reset();
  initHTTP();

  delay(2000);
}

void loop() {

  proccessGPS();

#ifdef DEBUG
  proccessSerial();
#endif

  //cloudLog(parseE, httpEnable);
  cloudLog(true, true);

  // srvCMD = getCommandStatus();
  //  Serial.println(x);
  //  doCommand(getCommandStatus());


}

void cloudLog(bool parseEnable = true, bool cloudEnable = false) {

  if (record == 0) {
    //start of chunk - get timeStamp
    timeStamp = (time / 100) + TIME_ZONE_FIX; //Time Zone correction
    lastTime = time;
    speedChunk[record] = speed;
    record ++;
    Serial.print(".");
  }


  if ((time - lastTime) >= INTERVAL_MS / 10) {  //Interval has passed, do the log or post to cloud
    lastTime = time;

    if (record < RECORDS_AMOUNT) {
      speedChunk[record] = speed;
      record ++;
      Serial.print(".");
      //      delay (INTERVAL_MS);
    }
    else {
      Serial.println("Done");
      record = 0;
      if (parseEnable) {
        char tmp[30];
        sprintf(buffer, AT_SET_URL_DATA, SERVER_URL, "set=1"); //set APN name
        strcat(buffer, "&t="); //t=
        dtostrf(timeStamp, 3, 0, tmp);
        strcat(buffer, tmp); //t=234
        strcat(buffer, ","); //t=234,

        sprintf(tmp, "&s=");
        strcat(buffer, tmp); //t=234&s=
        for (uint8_t i = 0; i < RECORDS_AMOUNT; i++) {
          itoa (speedChunk[i], tmp, 10);
          strcat(buffer, tmp); //ts1=234&sp1=213
          sprintf(tmp, ",", i); //&sp1=
          strcat(buffer, tmp); //ts1=234&sp1=
        }

        strcat(buffer, "\"\r");

        if (cloudEnable)sendHTTPGET(buffer);
        Serial.println(buffer);
      }
    }
  }
}

#ifdef DEBUG
void proccessSerial() {

  byte c = Serial.read();

  if (c == '1') {
    parseE = !parseE;
    return;
  }
  else if (c == '2') {
    httpEnable = !httpEnable;
    return;
  }
  else if (c == '3') {
    action();
    return;
  }
  else if (c == '4') {
    getCommandStatus();
    return;
  }
  else if (c == 'e') {
    //break the if state ==> enter serial AT mode
    ;
  }

  else if (c == 'r') {
    Serial.println("Reset");
    reset();
    return;
  }
  else {
    return;
  }


  serial = "";
  Serial.println("serial AT ON");
  while (1) {
    serial = Serial.readStringUntil('\r');
    serial.toCharArray(buffer, sizeof(buffer) / sizeof(buffer[0]));

    if (strcmp(buffer, "") == 0); //empty string - do nothing just keep loop
    else if (strcmp(buffer, "***") == 0) {
      serialEditorEnable = false;  //Get out from serialEditor
      Serial.println("serial AT OFF");
      return;
    }
    else {
      sendGSMCommand(buffer);
    }
  }
  return;
}
#endif


void reset() {
  Serial.print("GSM shutDown..");
  sleepAndPrint(3);
  Serial.println("Done");
  toggleGSM();
  Serial.print("Low power mode 30 Seconds..");
  one.enterLowPowerMode();
  sleepAndPrint(30);
  Serial.print("Wakup and reset");
  one.leaveLowPowerMode();
  wdt_enable(3);
}



void sleepAndPrint(uint8_t sleepSec) {
  for (uint8_t i = 0 ; i < sleepSec * 2; i++) {
    Serial.print(".");
    delay(500);
  }
}

