
namespace label {
  const String OTA_UPDATE = "otaupd" ;
  const String OTA_STATUS = "otasta" ;
} ;


namespace ota {
  const int DELAY = 5*60*1000 ;

  const byte AUTH_ERR    = 1 ;
  const byte BEGIN_ERR   = 2 ;
  const byte CONNECT_ERR = 3 ;
  const byte RECEIVE_ERR = 4 ;
  const byte END_ERR     = 5 ;
  const byte TIMEOUT_ERR = 6 ;
  const byte ERROR_MASK  = 0x07 ;
  const byte NEWOTA_MASK = 0x40 ;
  const byte UPDATE_MASK = 0x80 ;

  
  void setup(void) {

    wireless::otaSetup() ;
    
    ArduinoOTA.onStart([]() {
      Serial.println("Start updating");
    });
    
    ArduinoOTA.onEnd([]() {
      eemem::writeOtaStatus(UPDATE_MASK) ;
      Serial.println("\nEnd");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error) ;
      if (error == OTA_AUTH_ERROR) {
        eemem::writeOtaStatus(UPDATE_MASK|AUTH_ERR) ;
        Serial.println("Auth Failed") ;
      } else if (error == OTA_BEGIN_ERROR) {
        eemem::writeOtaStatus(UPDATE_MASK|BEGIN_ERR) ;
        Serial.println("Begin Failed") ;
      } else if (error == OTA_CONNECT_ERROR) {
        eemem::writeOtaStatus(UPDATE_MASK|CONNECT_ERR) ;
        Serial.println("Connect Failed") ;
      } else if (error == OTA_RECEIVE_ERROR) {
       eemem::writeOtaStatus(UPDATE_MASK|RECEIVE_ERR) ;
        Serial.println("Receive Failed") ;
      } else if (error == OTA_END_ERROR) {
        eemem::writeOtaStatus(UPDATE_MASK|END_ERR) ;
        Serial.println("End Failed") ;
      }
      ESP.restart() ;
    }) ;
    
    ArduinoOTA.begin();
  }


  void loop(void) {
    static long l_millis = millis() ;

    ArduinoOTA.handle() ;
    if (millis()-l_millis > DELAY) {
      eemem::writeOtaStatus(UPDATE_MASK|TIMEOUT_ERR) ;
      Serial.println("OTA timeout") ;
      ESP.restart() ;
    }
  }

}
