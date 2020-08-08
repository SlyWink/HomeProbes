namespace probeId {
  const uint8_t ALL     = 0 ;
  const uint8_t BRIDGE =  1 ;
  const uint8_t LINKY  =  2 ;
  const uint8_t GASPAR =  3 ;
  const uint8_t METEO  =  4 ;
  const uint8_t IDCOUNT = 5 ;
  const uint8_t MAXPROBES = 10 ;

#ifdef ISBRIDGE
  const uint8_t current = BRIDGE ;
#elif defined ISLINKY
  const uint8_t current = LINKY ;
#elif defined ISGASPAR
  const uint8_t current = GASPAR ;
#elif defined ISMETEO
  const uint8_t current = METEO ;
#endif
    
} ;

struct LOGIN_T {
  String wifiId ;
  String wifiPwd ;
} ;

struct WIFI_T {
  String name ;
  LOGIN_T mesh ;
  LOGIN_T ota ;  
} ;

struct OTA_T {
  byte value ;
} ;

struct EEMEM_T {
  WIFI_T probe ;
  OTA_T status ;
} ;
