#ifndef Battery_h
#define Battery_h

void battery_line_layer_update_callback(Layer *BatteryLineLayer, GContext* ctx);

void handle_battery(BatteryChargeState charge_state);

#endif