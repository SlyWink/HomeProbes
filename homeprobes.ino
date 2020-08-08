#define INIT
#define ISLINKY

#include <EEPROM.h>
#include <ArduinoOTA.h>
#include <painlessMesh.h>


/*
 * Message : to_probe_id " " from_probe_id  " "  probe_msg_num  " "  message_body
 * 
 * Request for value body : label | "ALL"
 * Send answer body       : label  "="  value  [ " " label "=" value ] ... [ " " label "=" value ]
 * Probe event body       : label  "="  value
 */
