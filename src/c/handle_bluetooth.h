#ifndef handle_bluetooth_h
#define handle_bluetooth_h

extern int   BTConnected; 
extern int   BTVibesDone;
extern int   PersistBTLoss;
extern Layer *BTLayer;

void handle_bluetooth(bool connected);

#endif