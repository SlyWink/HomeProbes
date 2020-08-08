
void taskSend(void) {
  message::sendBuffered(mesh::m) ;
}


void taskReceive(void) {
  static uint16_t l_counter = 1 ;
  uint8_t l_fromProbeId ;
  String l_fromMsgBody ;
  String l_key ;
  String l_val ;
 
  if (!message::readBuffered(l_fromProbeId, l_fromMsgBody)) return ;
  Body l_body(&l_fromMsgBody) ;
  while (l_body.nextKeyVal(l_key,l_val)) {
    if (l_key == "test") {
      if (l_val == "") {// Request
        String l_body = "test=" + (String) l_counter ;
        message::sendToBuffer(l_fromProbeId,l_body) ;
        l_counter += 10 ;
      } else {
        Serial.println((String) l_fromProbeId+": "+ l_key + "->" + l_val) ;
      }
    }
  }
}
