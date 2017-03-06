bool initGSM() {
  // discard any stale data
  one.xbPurge();
  for (byte n = 0; n < 10; n++) {
    if (sendGSMCommand(AT_ECHO_OFF) != 0) {
      //    Serial.println("GSM OK");
      return true;
    }
    // try turning on GSM
    toggleGSM();
    delay(2000);
  }
  //  Serial.println("GSM Fail");
  return false;
}

bool sendGSMCommand(const char* cmd, unsigned int timeout = 10, const char* expected = "OK")
{
  one.setTarget(TARGET_BEE);
  //  Serial.print("sendGSMCommand:");
  Serial.println(cmd);
  one.xbWrite(cmd);
  delay(10);
  if (one.xbReceive(buffer, sizeof(buffer), timeout, expected) != 0) {
    Serial.println(buffer);
    return true;
  }
  Serial.println(buffer);
  return false;
}

void toggleGSM()
{
  one.setTarget(TARGET_OBD); //TODO: WTF ?????
  one.sendCommand("ATGSMPWR\r", buffer, sizeof(buffer));
}
