#ifndef TaskScheduler_h
#define TaskScheduler_h

#include "Globals.h"

typedef struct Tasks {
   long unsigned int previous;
   long unsigned int interval;
   void (*function)();
} Task;

class TaskScheduler {
  public:
    TaskScheduler();
    void loop();
    void addTask(void (*function)(), int interval);
  private:
    Task _tasks[NOTASKS];
    int _count;
    unsigned long millis();
};

#endif

