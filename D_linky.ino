#ifndef INIT
#ifdef ISLINKY

#include <SoftwareSerial.h>

namespace label {
  const uint8_t BASE_VAL = 0 ;
  const uint8_t BASE_AVG = 1 ;
  const uint8_t BASE_MIN = 2 ;
  const uint8_t BASE_MAX = 3 ;

  const uint8_t PAPP_VAL = 4 ;
  const uint8_t PAPP_AVG = 5 ;
  const uint8_t PAPP_MIN = 6 ;
  const uint8_t PAPP_MAX = 7 ;

  const uint8_t IINST_VAL = 8 ;
  const uint8_t IINST_AVG = 9 ;
  const uint8_t IINST_MIN = 10 ;
  const uint8_t IINST_MAX = 11 ;


  const String text[] = {
    F("basval"), F("basavg"), F("basmin"), F("basmax"),
    F("papval"), F("papavg"), F("papmin"), F("papmax"),
    F("iinval"), F("iinavg"), F("iinmin"), F("iinmax")
  }
} ;


namespace linky {
  
  SoftwareSerial* _serial = NULL ;
  String label, value ;
 
  void begin(int p_rxpin) {
    pinMode(p_rxpin,INPUT) ;
    _serial = new SoftwareSerial(p_rxpin,SW_SERIAL_UNUSED_PIN) ;
    _serial->begin(1200) ;
  }
  
  bool readLine(void) {
    static const char ASCII_STX = 0x2 ;
    static const char ASCII_ETX = 0x3 ;
    static const char ASCII_CR  = 0xD ;
    static const char ASCII_LF  = 0xA ;
    static const uint8_t FRAME   = 1 ;
    static const uint8_t SPACE1  = 2 ;
    static const uint8_t SPACE2  = 3 ;
    static const uint8_t CHECKED = 4 ;
    static uint8_t l_status = 0 ;
    static char l_checksum ;

    if (_serial == NULL || !_serial->available()) return false ;
    char l_char = _serial->read() & 0x7F ; // Ignore parity bit
    switch(l_char) {
      case ASCII_STX :
        l_status = FRAME ;
        break ;
      case ASCII_ETX :
        l_status = 0 ;
        break ;
      case ASCII_LF :
        label = value = "" ;
        l_checksum = 0 ;
        if (l_status) l_status = FRAME ;
        break ;
      case ASCII_CR :
        if (l_status == CHECKED) return true ;
        break ;
      default :
        if ((l_status == SPACE2) && ((l_checksum & 0x3F) + 0x20 == l_char))
          l_status = CHECKED ;
        else {
          if (l_char == ' ') {
            if (l_status == FRAME) l_status = SPACE1 ;
            else if (l_status == SPACE1) l_status = SPACE2 ;
          } else {
            if (l_status) {
              if (l_status < SPACE1) label += l_char ;
              else value += l_char ;
            }
          }
          if (l_status < SPACE2) l_checksum += l_char ;
        }
    }
    return false ;
  }

}


void targetSetup(void) {
  linky::begin(D6) ;
}


void targetLoop(void) {
  if (linky::readLine())
    if ((linky::label == F("BASE")) || (linky::label == F("PAPP")) || (linky::label == F("IINST"))) {
      Serial.print(linky::label) ;
      Serial.print(" = ") ;
      Serial.println(linky::value) ;
    }
//if (linky::_serial->available()) Serial.print((char) (linky::_serial->read() & 0x7F)) ;

}

#endif
#endif
