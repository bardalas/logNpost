typedef enum {
  GPRS_DISABLED = 0,
  GPRS_IDLE,
  GPRS_HTTP_RECEIVING,
  GPRS_HTTP_ERROR,
} GPRS_STATES;

typedef enum {
  HTTP_GET = 0,
  HTTP_POST,
} HTTP_METHOD;


bool initGPRS()
{
  timeStamp = millis();
  bool gprsState = false;
  do {
    gprsState = sendGSMCommand(AT_NET_REG, 5000, "+CREG: 0,") != 0;
    Serial.print('.');
  } while (!gprsState && millis() - timeStamp < MAX_CONN_TIME);
  if (!gprsState) {
    Serial.println(MSG_ERROR);
    return false;
  }
  sendGSMCommand(AT_SET_GPRS); //set GPRS mode
  sprintf_P(buffer, PSTR(AT_SET_APN), APN); //set APN name
  sendGSMCommand(buffer, MAX_CONN_TIME);
  timeStamp = millis();
  do {
    sendGSMCommand(AT_OPEN_BRR, 5000); //open bearer
    sendGSMCommand(AT_QUERY_BRR, 5000); //quary bearer (get ip)
    gprsState = !strstr_P(buffer, PSTR("0.0.0.0")) && !strstr_P(buffer, PSTR(MSG_ERROR)); //verify that we have ip add
  } while (!gprsState && millis() - timeStamp < MAX_CONN_TIME);

  return true;
}

void initHTTP() {
  sendGSMCommand(AT_INIT_HTTP);
  sendGSMCommand(AT_SET_HTTP_GET, 1000);

  sprintf_P(buffer, PSTR(AT_SET_URL), SERVER_URL, START_LOG_CMD); //set APN name
  sendHTTPGET(buffer);
  //  sendHTTPGET(AT_URL_START_LOG); //

}

void sendHTTPGET(char *val) {
  sendGSMCommand(val);
  sendGSMCommand(AT_SEND_HTTP); //session start (send the http)
}

void action() {
  sendHTTPGET("http://smarthouse.gear.host/cmd.php?set=1");
}

int getCommandStatus() {
  sendGSMCommand(AT_READ_HTTP);
  //  sendGSMCommand(AT_INIT_HTTP); http://smarthouse.gear.host/getVarFromDB.php?offset=1
  char *p;
  p = strstr(buffer, "srvCMD");
  if (p != NULL) {
    //    Serial.print("found:");
    p = strstr(buffer, "=");
    //    Serial.println(p[1]); //here is the command number from server
    return (int)((p[1]) - '0');//  Serial.print(srvCMW);
  }
  return -1;
}

void doCommand(int cmd) {
  /*
  if (cmd == srvCMD) {
    // command was not changed
    cmdCount++;
    if (cmdCount > CMD_ACTION_CNT) {
      //it is time to Act
      cmdCount = 0;
    }
    else return; //do nothing
  }
  else {
    //new CMD
    cmdCount = 0;
  }

*/
  if      (cmd == -1) ;//Serial.println("No Action found");
  else if (cmd == 0)  ;//Serial.println("Normal Run");
  else if (cmd == 1)  ;//Serial.println("Do something 1");
  else if (cmd == 2)  ;//Serial.println("Do something 2");
  else if (cmd == 3)  ;//Serial.println("Do something 3");
  else if (cmd == 4)  ;//Serial.println("Do something 3");
  else Serial.println("Unknown Action");

}


