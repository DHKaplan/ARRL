#include <pebble.h>
#include "Callbacks.h"

//Receive Input from Config html to js file
void inbox_received_callback(DictionaryIterator *iterator, void *context) {
APP_LOG(APP_LOG_LEVEL_ERROR, "In Inbox received callback");

  FirstTime = 0;

  //Date Format
  Tuple *Date_Type = dict_find(iterator, MESSAGE_KEY_DATE_FORMAT_KEY);

  if(Date_Type) {
      strcpy(PersistDateFormat, (Date_Type->value->cstring));
      APP_LOG(APP_LOG_LEVEL_INFO,      "      Set Date Format to                    %s - Config - 0 = US, 1 = Int'l",PersistDateFormat);
  } else { //Check for Persist
        if(persist_exists(MESSAGE_KEY_DATE_FORMAT_KEY)) {
           persist_read_string(MESSAGE_KEY_DATE_FORMAT_KEY, PersistDateFormat, 1);
           APP_LOG(APP_LOG_LEVEL_INFO, "      Set Date Format to                    %s - Persistent - 0 = US, 1 = Int'l", PersistDateFormat);
        }  else {   // Set Default
           APP_LOG(APP_LOG_LEVEL_INFO, "      Set Date Format to                    %s - Default - 0 = US, 1 = Int'l", PersistDateFormat);
         }
  }
  
  if (strcmp(PersistDateFormat, "0") == 0) {     // US
     strcpy(date_format, "%b %e %Y");
  }  else  {
     strcpy(date_format, "%e %b %Y");
  }

  //Vibrate on BT Loss
  Tuple *BTVib = dict_find(iterator, MESSAGE_KEY_BT_VIBRATE_KEY);

  if(BTVib) {
    PersistBTLoss = BTVib->value->int32;
      APP_LOG(APP_LOG_LEVEL_INFO,      "      Set Vib on BT Loss to                 %d - Config - 0 = No Vib, 1 = Vib", PersistBTLoss);
  } else { //Check for Persist
        if(persist_exists(MESSAGE_KEY_BT_VIBRATE_KEY)) {
           PersistBTLoss = persist_read_int(MESSAGE_KEY_BT_VIBRATE_KEY);
           APP_LOG(APP_LOG_LEVEL_INFO, "      Set Vib on BT Loss to                 %d - Persistent - 0 = No Vib, 1 = Vib", PersistBTLoss);
        }  else {   // Set Default
           PersistBTLoss = 0;  // Default NO Vibrate
           APP_LOG(APP_LOG_LEVEL_INFO, "      Set Vib on BT Loss to                 %d - Default - 0 = No Vib, 1 = Vib", PersistBTLoss);
         }
  }


  //Vibrate on Low Batt
  Tuple *LowBatt = dict_find(iterator, MESSAGE_KEY_LOW_BATTERY_KEY);

  if(LowBatt) {
    PersistLow_Batt = LowBatt->value->int32;
    APP_LOG(APP_LOG_LEVEL_INFO,        "      Set Vib on Low Batt to                %d - Config - 0 = No Vib, 1 = Vib", PersistLow_Batt);
  } else { //Check for Persist
        if(persist_exists(MESSAGE_KEY_LOW_BATTERY_KEY)) {
           PersistLow_Batt = persist_read_int(MESSAGE_KEY_LOW_BATTERY_KEY);
           APP_LOG(APP_LOG_LEVEL_INFO, "      Set Vib on Low Batt to                %d - Persistent - 0 = No Vib, 1 = Vib", PersistLow_Batt);
        }  else {   // Set Default
           PersistLow_Batt = 0;  // Default NO Vibrate
           APP_LOG(APP_LOG_LEVEL_INFO, "      Set Vib on Low Batt to                %d - Default - 0 = No Vib, 1 = Vib", PersistLow_Batt);
         }
  }  
}

void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Inbox Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}


