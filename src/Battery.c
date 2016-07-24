#include <pebble.h>

#include "Battery.h"

#include "ARRL.h"

int       batterycharging; 
int       batterychargepct;
int       BatteryVibesDone;
int       PersistLow_Batt       =  0;     // 0 = No Vib, 1 = Vib 
  
  void battery_line_layer_update_callback(Layer *BatteryLineLayer, GContext* ctx) {
     graphics_context_set_fill_color(ctx, GColorWhite);
     graphics_fill_rect(ctx, layer_get_bounds(BatteryLineLayer), 3, GCornersAll);
        
     if (batterycharging == 1) {
       #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorBlue);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif

       graphics_fill_rect(ctx, GRect(2, 1, 100, 4), 3, GCornersAll);

     } else if (batterychargepct > 20) {
       #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorGreen);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif

       graphics_fill_rect(ctx, GRect(2, 1, batterychargepct, 4), 3, GCornersAll);

     } else {
      #ifdef PBL_COLOR
          graphics_context_set_fill_color(ctx, GColorRed);
       #else
          graphics_context_set_fill_color(ctx, GColorBlack);
       #endif

       graphics_fill_rect(ctx, GRect(2, 1, batterychargepct, 4),3, GCornersAll);
     }

  //Battery % Markers
      #ifdef PBL_COLOR
        graphics_context_set_fill_color(ctx, GColorBlack);
      #else
        if(batterycharging == 1) {
            graphics_context_set_fill_color(ctx, GColorBlack);
        } else {
            graphics_context_set_fill_color(ctx, GColorWhite);
        }
      #endif

      graphics_fill_rect(ctx, GRect(89, 1, 3, 4), 3, GCornerNone);
      graphics_fill_rect(ctx, GRect(79, 1, 3, 4), 3, GCornerNone);
      graphics_fill_rect(ctx, GRect(69, 1, 3, 4), 3, GCornerNone);
      graphics_fill_rect(ctx, GRect(59, 1, 3, 4), 3, GCornerNone);
      graphics_fill_rect(ctx, GRect(49, 1, 3, 4), 3, GCornerNone);
      graphics_fill_rect(ctx, GRect(39, 1, 3, 4), 3, GCornerNone);
      graphics_fill_rect(ctx, GRect(29, 1, 3, 4), 3, GCornerNone);
      graphics_fill_rect(ctx, GRect(19, 1, 3, 4), 3, GCornerNone);
      graphics_fill_rect(ctx, GRect(9,  1, 3, 4), 3, GCornerNone);

}


void handle_battery(BatteryChargeState charge_state) {
    static char BatteryPctTxt[] = "+100%";

  batterychargepct = charge_state.charge_percent;

  if (charge_state.is_charging) {
    batterycharging = 1;
    text_layer_set_background_color(text_battery_layer, GColorOxfordBlue);
    text_layer_set_text_color(text_battery_layer, GColorYellow);
  } else {
    batterycharging = 0;
  }

  // Reset if Battery > 20% ********************************
  if ((batterychargepct > 20) && (batterycharging == 0)){
        BatteryVibesDone = 0;

      text_layer_set_background_color(text_battery_layer, GColorOxfordBlue);
      text_layer_set_text_color(text_battery_layer, GColorYellow);
  }

  //
  if ((batterychargepct < 30) && (batterycharging == 0)) {
       #ifdef PBL_COLOR
           text_layer_set_text_color(text_battery_layer, GColorRed);
       #else
           text_layer_set_text_color(text_battery_layer, GColorBlack);
       #endif

       text_layer_set_background_color(text_battery_layer, GColorWhite);

       if ((BatteryVibesDone == 0) && (PersistLow_Batt == 1)) {            // Do Once
            BatteryVibesDone = 1;
            APP_LOG(APP_LOG_LEVEL_WARNING, "Battery Vibes Sent");
            vibes_long_pulse();

            #ifdef PBL_COLOR
              text_layer_set_text_color(text_battery_layer, GColorRed);
            #else
             text_layer_set_text_color(text_battery_layer, GColorBlack);
            #endif

            text_layer_set_background_color(text_battery_layer, GColorWhite);
      }
  }

   if (charge_state.is_charging) {
     strcpy(BatteryPctTxt, "Chrg");
  } else {
     snprintf(BatteryPctTxt, 5, "%d%%", charge_state.charge_percent);
  }
   text_layer_set_text(text_battery_layer, BatteryPctTxt);

  layer_mark_dirty(BatteryLineLayer);
}

