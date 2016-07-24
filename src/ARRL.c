#include "pebble.h"

#include "ARRL.h"

#include "handle_bluetooth.h"

#include "BTLine_update_callback.h"

#include "Battery.h"

#include "Callbacks.h"


Window *window;

TextLayer *text_dayname_layer;
TextLayer *text_mmdd_layer;
TextLayer *text_year_layer;
TextLayer *text_time_layer;

GFont        fontHelvNewLight20;
GFont        fontRobotoCondensed25;
GFont        fontSystemGothicBold28;
GFont        fontRobotoBoldSubset40;
GFont        fontRobotoBoldSubset49;

 GBitmap     *image;
 BitmapLayer *image_layer;

GPoint     Linepoint;

//Common Defines

int       BTConnected = 1;
int       BTVibesDone = 0;

Layer     *BatteryLineLayer;
Layer     *BTLayer;

TextLayer *text_battery_layer;

int       PersistBTLoss         =  0;     // 0 = No Vib, 1 = Vib
char      PersistDateFormat[]   = "0";    // 0 = US, 1 = Intl

 char     date_format[]="%b %e, %Y";

//End Common

 int FirstTime = 0;

 char date_type[]="us  ";
 char dayname_text[] = "XXX";
 char time_text[] = "00:00";
 char mmdd_text[] = "XXXX00";
 char year_text[] = "0000";
 char seconds_text[] = "00";

void handle_appfocus(bool in_focus){
    if (in_focus) {
        handle_bluetooth(bluetooth_connection_service_peek());
    }
}

void handle_tick(struct tm *tick_time, TimeUnits units_changed) {
  char time_format[] = "%I:%M";

  strftime(seconds_text, sizeof(seconds_text), "%S", tick_time);

  if (clock_is_24h_style()) {
       strcpy(time_format,"%R");
     } else {
       strcpy(time_format,"%I:%M");
     }

  strftime(time_text, sizeof(time_text), time_format, tick_time);
 
  // Kludge to handle lack of non-padded hour format string
  // for twelve hour clock.
  if (!clock_is_24h_style() && (time_text[0] == '0')) {
    memmove(time_text, &time_text[1], sizeof(time_text) - 1);
  }
  
  if((strcmp(seconds_text,"00") == 0) || (FirstTime == 0)) {
     strftime(dayname_text, sizeof(dayname_text), "%a",    tick_time);
     strftime(mmdd_text,    sizeof(mmdd_text), date_format, tick_time);
     strftime(year_text,    sizeof(year_text),    "%Y",    tick_time);

     text_layer_set_text(text_dayname_layer, dayname_text);
     text_layer_set_text(text_mmdd_layer, mmdd_text);
     text_layer_set_text(text_year_layer, year_text);
  }

if (units_changed & DAY_UNIT) {
   // Only update the day name & date when it's changed.
    text_layer_set_text(text_dayname_layer, dayname_text);
    text_layer_set_text(text_mmdd_layer, mmdd_text);
  }


 if((strcmp(seconds_text,"00") == 0) || (FirstTime == 0)) {
     text_layer_set_text(text_time_layer, time_text); 
  }
  FirstTime = 1;
 }


void handle_deinit(void) {
  tick_timer_service_unsubscribe();
  
  persist_write_string(MESSAGE_KEY_DATE_FORMAT_KEY,       PersistDateFormat);
  persist_write_int(MESSAGE_KEY_BT_VIBRATE_KEY,           PersistBTLoss);
  persist_write_int(MESSAGE_KEY_LOW_BATTERY_KEY,          PersistLow_Batt);
  
  battery_state_service_unsubscribe();
  bluetooth_connection_service_unsubscribe();
  app_focus_service_unsubscribe();

  text_layer_destroy(text_time_layer);
  text_layer_destroy(text_year_layer);
  text_layer_destroy(text_mmdd_layer);
  text_layer_destroy(text_dayname_layer);
  text_layer_destroy(text_battery_layer);

  layer_destroy(BatteryLineLayer);
  layer_destroy(BTLayer);

  fonts_unload_custom_font(fontHelvNewLight20);
  fonts_unload_custom_font(fontRobotoCondensed25);
  fonts_unload_custom_font(fontRobotoBoldSubset40);
  fonts_unload_custom_font(fontRobotoBoldSubset49);

  window_destroy(window);

  gbitmap_destroy(image);
  bitmap_layer_destroy(image_layer);
}



void handle_init(void) {
  window = window_create();
  window_stack_push(window, true /* Animated */);
  window_set_background_color(window, GColorOxfordBlue);

  Layer *window_layer = window_get_root_layer(window);

  fontHelvNewLight20     = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_HELV_NEW_LIGHT_20));
  fontRobotoCondensed25  = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_CONDENSED_25));
  fontSystemGothicBold28 = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  fontRobotoBoldSubset40 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_40));
  fontRobotoBoldSubset49 = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_BOLD_SUBSET_49));

  // Register callbacks
  app_message_register_inbox_received(inbox_received_callback);
  app_message_register_inbox_dropped(inbox_dropped_callback);
  app_message_register_outbox_failed(outbox_failed_callback);
  app_message_register_outbox_sent(outbox_sent_callback);

  // Open AppMessage
  app_message_open(64, 64);

  // Dayname
  #ifdef PBL_PLATFORM_CHALK
      text_dayname_layer = text_layer_create(GRect(65, 25, 91, 40));
  #else
      text_dayname_layer = text_layer_create(GRect(59, 25, 91, 40));
  #endif 
    
  text_layer_set_text_color(text_dayname_layer,GColorYellow);
  text_layer_set_background_color(text_dayname_layer, GColorOxfordBlue);
  text_layer_set_text_alignment(text_dayname_layer, GTextAlignmentCenter);
  text_layer_set_font(text_dayname_layer, fontSystemGothicBold28);
  layer_add_child(window_layer, text_layer_get_layer(text_dayname_layer));

  // mmdd
  #ifdef PBL_PLATFORM_CHALK
      text_mmdd_layer = text_layer_create(GRect(65, 55, 91, 40));
  #else
       text_mmdd_layer = text_layer_create(GRect(59, 55, 91, 40));
  #endif
    
  text_layer_set_text_color(text_mmdd_layer, GColorYellow);
  text_layer_set_background_color(text_mmdd_layer, GColorOxfordBlue);
  text_layer_set_text_alignment(text_mmdd_layer, GTextAlignmentCenter);
  text_layer_set_font(text_mmdd_layer, fontRobotoCondensed25);
  layer_add_child(window_layer, text_layer_get_layer(text_mmdd_layer));

  // year
  #ifdef PBL_PLATFORM_CHALK
      text_year_layer = text_layer_create(GRect(65, 86, 91, 40));
  #else
      text_year_layer = text_layer_create(GRect(59, 86, 91, 40));
  #endif
 
  text_layer_set_text_alignment(text_year_layer, GTextAlignmentCenter);  
  text_layer_set_text_color(text_year_layer, GColorYellow);
  text_layer_set_background_color(text_year_layer, GColorOxfordBlue); 
  text_layer_set_font(text_year_layer, fontRobotoCondensed25);
  layer_add_child(window_layer, text_layer_get_layer(text_year_layer));

// ARRL Logo
  #ifdef PBL_PLATFORM_CHALK
      image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ARRL_LOGO_ROUND);
      image_layer = bitmap_layer_create(GRect(20, 17,46, 100));
  #else //Aplite or Basalt
      image = gbitmap_create_with_resource(RESOURCE_ID_IMAGE_ARRL_LOGO);
      image_layer = bitmap_layer_create(GRect(5, 1, 58, 119));
  #endif
 
  bitmap_layer_set_bitmap(image_layer, image);
  bitmap_layer_set_alignment(image_layer, GAlignCenter);
  layer_add_child(window_layer, bitmap_layer_get_layer(image_layer));

  //Persistent Value Date Format:
  if (persist_exists(MESSAGE_KEY_DATE_FORMAT_KEY)) {
     persist_read_string(MESSAGE_KEY_DATE_FORMAT_KEY  , PersistDateFormat, sizeof(PersistDateFormat));
     APP_LOG(APP_LOG_LEVEL_INFO, "    Set Date Format to                    %s - Persistent - 0 = US, 1 = Int'l", PersistDateFormat);
  }  else {
     APP_LOG(APP_LOG_LEVEL_INFO, "    Set Date Format to                    0 - Default - US");
     strcpy(PersistDateFormat, "0"); //Default
  }

  if (strcmp(PersistDateFormat, "0") == 0) {     // US
     strcpy(date_format, "%b %e %Y");
  } else {
     strcpy(date_format, "%e %b %Y");
  }

  //Persistent Value Vib On BTLoss
  if(persist_exists(MESSAGE_KEY_BT_VIBRATE_KEY)) {
     PersistBTLoss = persist_read_int(MESSAGE_KEY_BT_VIBRATE_KEY);
     APP_LOG(APP_LOG_LEVEL_INFO, "    Set BT Vibrate To                     %d - Persistent - 0 = NO Vib, 1 = Vib", PersistBTLoss);
  }  else {
     PersistBTLoss = 0; // Default
     APP_LOG(APP_LOG_LEVEL_INFO, "    Set BT Vibrate To                     0 - Default - No Vibrate");

  }

  //Persistent Value Vib on Low Batt
  if(persist_exists(MESSAGE_KEY_LOW_BATTERY_KEY)) {
     PersistLow_Batt = persist_read_int(MESSAGE_KEY_LOW_BATTERY_KEY);
     APP_LOG(APP_LOG_LEVEL_INFO, "    Set Low Batt Vibrate To               %d - Persistent - 0 = NO Vib, 1 = Vib", PersistLow_Batt);
  }  else {
     PersistLow_Batt = 0; // Default
     APP_LOG(APP_LOG_LEVEL_INFO, "    Set Low Batt Vibrate To               0 - Default - No Vibrate");

  }

  
  // Time of Day is here
  #ifdef PBL_PLATFORM_CHALK
      text_time_layer = text_layer_create(GRect(1, 118, 180, 50));
      text_layer_set_font(text_time_layer, fontRobotoBoldSubset40);
  #else
      text_time_layer = text_layer_create(GRect(1, 118, 144, 50));
      text_layer_set_font(text_time_layer, fontRobotoBoldSubset49);
  #endif
    
  text_layer_set_text_color(text_time_layer, GColorYellow);
  text_layer_set_background_color(text_time_layer, GColorOxfordBlue);
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  // Line
  #ifdef PBL_PLATFORM_CHALK
      GRect line_frame = GRect(38, 118, 104, 6);
  #else
      GRect line_frame = GRect(22, 122, 104, 6);
  #endif
    
  BatteryLineLayer = layer_create(line_frame);
  layer_set_update_proc(BatteryLineLayer, battery_line_layer_update_callback);
  layer_add_child(window_layer, BatteryLineLayer);

  tick_timer_service_subscribe(SECOND_UNIT, handle_tick);


    //Bluetooth Logo Setup area
    GRect BTArea = GRect(60, 5, 20, 20);
    BTLayer = layer_create(BTArea);

    layer_add_child(window_layer, BTLayer);

    layer_set_update_proc(BTLayer, BTLine_update_callback);

    bluetooth_connection_service_subscribe(&handle_bluetooth);

    //Battery Text
    #ifdef PBL_PLATFORM_CHALK
        text_battery_layer = text_layer_create(GRect(80,2,55,28));
    #else
        text_battery_layer = text_layer_create(GRect(85,2,55,28));
    #endif
      
    text_layer_set_text_color(text_battery_layer, GColorYellow);
    text_layer_set_background_color(text_battery_layer, GColorOxfordBlue);
    text_layer_set_text_alignment(text_battery_layer, GTextAlignmentRight);
    text_layer_set_font(text_battery_layer, fontHelvNewLight20);

    layer_add_child(window_layer, text_layer_get_layer(text_battery_layer));

    battery_state_service_subscribe(&handle_battery);
    bluetooth_connection_service_subscribe(&handle_bluetooth);
    app_focus_service_subscribe(&handle_appfocus);

    handle_battery(battery_state_service_peek());
    handle_bluetooth(bluetooth_connection_service_peek());
  
  // Ensures time is displayed immediately (will break if NULL tick event accessed).
  // (This is why it's a good idea to have a separate routine to do the update itself.)
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  handle_tick(current_time, SECOND_UNIT);

}


int main(void) {
   handle_init();

   app_event_loop();

   handle_deinit();
}
