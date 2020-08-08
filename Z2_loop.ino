void loop() {
  
}

/*
void loopY() {
  static unsigned long ms1 = millis() ;
  static unsigned long ms2 = millis() ;
  static unsigned long ms3 = millis() ;

//  if (eemem::store.status.value & ota::NEWOTA_MASK) ota::loop() ;
//    else targetLoop() ;

  static uint8_t pid ;
  static String msg ;
  
  if (millis() - ms1 >= 5000L) {
    message::sendBuffered() ;
 Serial.println("<<SB>>") ;
    ms1 = millis() ;
  }
  if (millis() - ms2 >= 1000L) {
    message::printSendBuffer() ;
    message::printReceiveBuffer() ;
    ms2 = millis() ;
  }
  if (millis() - ms3 >= 22000L) {
    if (message::readBuffered(pid,msg)) Serial.println((String) pid + "=" + msg) ;
 Serial.println("<<RB>>") ;
    ms3 = millis() ;
  }

}
*/

/*
void loopX() {
  delay(5000) ;
  Serial.println('.') ;
}
*/
