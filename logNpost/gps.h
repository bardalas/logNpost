#define GPS_SERIAL_BAUDRATE 115200L

bool ready = false;

bool initGPS() {
  Serial.print("#GPS...");
  if (one.initGPS(GPS_SERIAL_BAUDRATE)) {
    Serial.println(MSG_OK);
    return true;
  }
  else {
    Serial.println(MSG_ERROR);
    return false;
  }
}


void showGPS()
{
  
  // parsed GPS data is ready
  Serial.print(" Time:");   Serial.print(time);
  Serial.print(" Speed:");  Serial.print(speed);     Serial.print("km/h");
  Serial.print(" Sat:");    Serial.print(sat);
  Serial.println();
  
}


bool proccessGPS() {
  
  // put your main code here, to run repeatedly:
  byte n = one.getGPSRawData(buffer, sizeof(buffer));
  if (n > 0) {
    bool updated = false;
    // need to skip heading ($GPS) and ending (>)
    for (byte m = 4; m < n - 1; m++) {
   //   if (!ready) Serial.write(buffer[m]);
      if (gps.encode(buffer[m])) {
        updated = true;
      }
    }
    if (updated) {
      // parsed GPS data is ready
      ready = true;
      gps.get_datetime(&date, &time, 0);
//      gps.get_position(&lat, &lon, 0);
//      lat_f = (float) lat / 100000;
//      lon_f = (float) lon / 100000;
//      altitude = gps.altitude() / 100 ;
      speed = (gps.speed() * 1852 / 100000); //km/h
      sat = gps.satellites();
      return true;
    }
  }
  return false;
}
