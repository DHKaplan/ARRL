#include <pebble.h>
#include "handle_bluetooth.h"

void handle_bluetooth(bool connected) {

  if (connected) {
         BTConnected = 1;     // Connected
         BTVibesDone = 0;

    } else {
         BTConnected = 0;      // Not Connected
          if ((BTVibesDone == 0) && (PersistBTLoss == 1)) {
             BTVibesDone = 1;
             vibes_long_pulse();
         }
    }

    layer_mark_dirty(BTLayer);
}
