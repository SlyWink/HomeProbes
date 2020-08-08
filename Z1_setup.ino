void setup(void) {
  EEPROM.begin(sizeof(EEMEM_T)) ;
  Serial.begin(115200) ;
  Serial.println("DEBUT") ;
}


/*
void setupY() {

  EEPROM.begin(sizeof(EEMEM_T)) ;
  Serial.begin(115200) ;
  Serial.println("DEBUT") ;
//  eemem::readConfig() ;
//  if (eemem::store.status.value & ota::NEWOTA_MASK) ota::setup() ;
//    else targetSetup() ;

  String msg1 = F("message L1") ;
  String msg2 = F("message G2") ;
  String msg3 = F("message L3") ;

  Serial.begin(115200) ;
  Serial.println(F("DEBUT")) ;
  message::sendToBuffer(probeId::LINKY,msg1) ;
  message::sendToBuffer(probeId::GASPAR,msg2) ;
  message::sendToBuffer(probeId::LINKY,msg3) ;
  message::printSendBuffer() ;
  delay(1000) ;
}
*/

/*
void setupX(void) {
  Fifo<uint8_t> l_fifo(4) ;
  
  Serial.begin(115200) ;
  Serial.println(F("DEBUT")) ;
  l_fifo.push(10) ;
  l_fifo.push(20) ;
  l_fifo.push(30) ;
  //l_fifo.display() ;
//  if (l_fifo.has(10)) Serial.println(F("OK 10")) ; else Serial.println(F("KO 10")) ;
//  if (l_fifo.has(100)) Serial.println(F("OK 100")) ; else Serial.println(F("KO 100")) ;
  l_fifo.push(40) ;
  l_fifo.push(50) ;
  //l_fifo.display() ;  
} 
*/
