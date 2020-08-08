namespace eemem {

  static EEMEM_T store ;

  
  void readBytes(byte* p_to, int p_count, int p_start=0) {
    for (int l_i=0 ; l_i<p_count ; l_i++)
      p_to[l_i] = EEPROM.read(l_i+p_start) ;
  }


  void writeBytes(const byte* p_from, int p_count, int p_start=0) {
    for (int l_i=0 ; l_i<p_count ; l_i++)
      EEPROM.write(l_i+p_start,p_from[l_i]) ;
    EEPROM.commit() ;
  }


  void writeOtaStatus(byte p_value) {
    eemem::store.status.value = p_value ;
    writeBytes((byte*) &eemem::store.status,sizeof(OTA_T),sizeof(WIFI_T)) ;
  }


  void readConfig(void) {
    readBytes((byte*) &eemem::store,sizeof(EEMEM_T)) ;
  }

}
