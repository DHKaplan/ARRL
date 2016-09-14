#ifndef Callbacks_h
#define Callbacks_h

extern int  FirstTime;

extern char date_format[];

extern char PersistDateFormat[];
extern int  PersistBTLoss;
extern int  PersistLow_Batt;

void inbox_received_callback(DictionaryIterator *iterator, void *context);

void inbox_dropped_callback(AppMessageResult reason, void *context);

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);

void outbox_sent_callback(DictionaryIterator *iterator, void *context);

#endif
