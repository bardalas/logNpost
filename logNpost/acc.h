
void initACC() {
  accState = false;
  Serial.print("#MEMS...");
  if (cmpu.memsInit()) {
    Serial.println("OK");
    accState = true;
  }
  else {
    Serial.println("Fail");
    accState = false;
  }
}

void getAcc() {
/*
  //  cmpu.memsRead(acc, gyr, mag, &tmp);
  for (uint8_t i = 0 ; i < AVG_COUNT; i++) {
    cmpu.memsRead(accCur, gyrCur, magCur, &tmp);
    for (uint8_t j = 0; j < 3; j++) {
      accSum[j] += accCur[j];
      gyrSum[j] += gyrCur[j];
      magSum[j] += magCur[j];
      delay(MEMS_READ_INTERVAL_MS);
    }
  }
  for (uint8_t j = 0; j < 3; j++) {
    accVal[j] = accSum[j] / AVG_COUNT;
    gyrVal[j] = gyrSum[j] / AVG_COUNT;
    magVal[j] = magSum[j] / AVG_COUNT;
  }

  Serial.print("ACC: ");
  Serial.print(accVal[0]); Serial.print(",");
  Serial.print(accVal[1]); Serial.print(",");
  Serial.println(accVal[2]);

  Serial.print("GYRO: ");
  Serial.print(gyrVal[0]); Serial.print(",");
  Serial.print(gyrVal[1]); Serial.print(",");
  Serial.println(gyrVal[2]);

  Serial.print("Mag: ");
  Serial.print(magVal[0]); Serial.print(",");
  Serial.print(magVal[1]); Serial.print(",");
  Serial.println(magVal[2]);

  Serial.print("Temperature: ");
  Serial.println((float)tmp / 10);
*/
}

