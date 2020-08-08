
namespace wireless {

  void otaSetup(void) {
    WiFi.mode(WIFI_STA) ;
    Serial.print(F("Connexion Wifi en cours")) ;
    do {
      WiFi.begin(eemem::store.probe.ota.wifiId,eemem::store.probe.ota.wifiPwd) ;
    } while (WiFi.waitForConnectResult() != WL_CONNECTED) ;
    Serial.println(F(" OK")) ;
  }
  
}


namespace message {
  
  static const uint8_t SND_BUFF_SIZE = 8 ;
  static const uint8_t SND_REPEAT = 3 ;
  struct _SEND_T {
    uint8_t toProbeId ;
    uint8_t toMsgNum ;
    uint8_t repeat ;
    String msgBody ;
  } ;
  Fifo<_SEND_T> _sndBuffer(SND_BUFF_SIZE) ;
  
  static uint32_t _matchId[probeId::MAXPROBES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } ;

  static const uint8_t RCV_BUFF_SIZE = 8 ;
  struct _RECEIVE_T {
    uint8_t fromProbeId ;
    String msgBody ;    
  } ;
  Fifo<_RECEIVE_T> _rcvBuffer(RCV_BUFF_SIZE) ;

  static const uint8_t RCV_HIST_SIZE = probeId::MAXPROBES ;
  struct _MSGNUMS_T {
    uint8_t fromProbeId ;
    uint8_t fromMsgNum ;
  } ;
  Fifo<_MSGNUMS_T> _rcvLastNums(RCV_HIST_SIZE) ;

  bool sendToBuffer(uint8_t p_toProbeId, String& p_msgBody) {
    static uint8_t l_msgNum = 0 ;
    _SEND_T l_send ;

    if (_sndBuffer.isFull()) return false ; 
    l_send.toProbeId = p_toProbeId ;
    l_send.toMsgNum = l_msgNum++ ;
    l_send.repeat = SND_REPEAT ;
    l_send.msgBody = p_msgBody ;
    _sndBuffer.pushRef(l_send) ;
    return true ;
  }


#ifdef RECETTE
  void receiveToBuffer(uint32_t p_fromMeshId, String& p_msg) ;
  void sendBuffered(void) {
#else
  void sendBuffered(painlessMesh& p_mesh) {
#endif
    static uint8_t l_index = 0 ;
    _SEND_T l_send ;

    if (!_sndBuffer.getAt(l_index,l_send)) { l_index = 0 ; return ; }
    uint8_t l_toProbeId = l_send.toProbeId ;
    String l_msg = (String) l_toProbeId + " " + (String) probeId::current + " "
            + (String) l_send.toMsgNum + " " + l_send.msgBody ;
    uint32_t l_toMeshId = l_toProbeId ? _matchId[l_toProbeId-1] : 0 ;
 #ifdef RECETTE
     receiveToBuffer(123456,l_msg) ;
 #else
     if (l_toMeshId)
      p_mesh.sendSingle(l_toMeshId,l_msg) ;
    else
      p_mesh.sendBroadcast(l_msg);
 #endif
    if (--l_send.repeat) {
      _sndBuffer.setAt(l_index,l_send) ;
      l_index++ ;
    } else
      _sndBuffer.pull(l_send) ; // No more repeat, drop message from buffer
  }


  void receiveToBuffer(uint32_t p_fromMeshId, String& p_msg) {
    _RECEIVE_T l_receive ;
    String l_item ;
    static _MSGNUMS_T l_idAndNum ;

    if (_rcvBuffer.isFull()) return ;
   
    Field l_field(&p_msg) ;
    if (!l_field.findNext(l_item)) return ; // Drop message
    uint8_t l_toProbeId = l_item.toInt() ;
    if (l_toProbeId && l_toProbeId != probeId::current) return ; // Not recipient, drop message
    if (!l_field.findNext(l_item)) return ; // Drop message
    l_idAndNum.fromProbeId = l_item.toInt() ;
    if (!l_field.findNext(l_item)) return ; // Drop message
    l_idAndNum.fromMsgNum = l_item.toInt() ;
    if (_rcvLastNums.has([](_MSGNUMS_T& p_idAndNum)->bool {
      return ((p_idAndNum.fromProbeId == l_idAndNum.fromProbeId) && (p_idAndNum.fromMsgNum == l_idAndNum.fromMsgNum)) ;
    })) return ;
Serial.println(F("PASSE")) ;
    if (!l_field.findNext(l_item,true)) return ; // Drop message

    l_receive.fromProbeId = l_idAndNum.fromProbeId ;
    l_receive.msgBody = l_item ;
    _rcvBuffer.pushRef(l_receive,false) ;
    
    _matchId[l_idAndNum.fromProbeId-1] = p_fromMeshId ;
    _rcvLastNums.push(l_idAndNum) ;
  }


  bool readBuffered(uint8_t& p_fromProbeId, String& p_msgBody) {
    _RECEIVE_T l_receive ;

    if (_rcvBuffer.pull(l_receive)) {
      p_fromProbeId = l_receive.fromProbeId ;
      p_msgBody = l_receive.msgBody ;
      return true ;
    } else
      return false ;
  }


  void printSendBuffer(void) {
    Serial.print(F("S: ")) ;
    Serial.println(_sndBuffer.dump([](_SEND_T& p_send)->String {
      return (String) p_send.toProbeId + "/" + (String) p_send.toMsgNum + "/" + p_send.msgBody ; 
    })) ;
  }


 void printReceiveBuffer(void) {
    Serial.print(F("R: ")) ;
    Serial.println(_rcvBuffer.dump([](_RECEIVE_T& p_receive)->String {
      return (String) p_receive.fromProbeId + "/" + p_receive.msgBody ;
    })) ;
  }
  
}
