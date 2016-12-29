#ifndef WouterRTOS_h
#define WouterRTOS_h

#include "WouterRTOSSettings.h"

typedef struct Tasks {
   long unsigned int previous;
   long unsigned int interval;
   void (*function)();
} Task;

class WouterRTOS {
  public:
    WouterRTOS();
    void loop();
    void addTask(void (*function)(), int interval);
  private:
    Task _tasks[NOTASKS];
    int _count;
    unsigned long millis();
};

#endif

