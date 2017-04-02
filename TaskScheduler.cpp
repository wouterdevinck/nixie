#include "TaskScheduler.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

// This uses Timer 1 to implement millis() - see https://adnbr.co.uk/articles/counting-milliseconds
// Note: this can also easily be done using the Arduino provided millis() and delay() functions

#define CTC_MATCH_OVERFLOW ((F_CPU / 1000) / 8)

volatile unsigned long timer1_millis;

ISR (TIMER1_COMPA_vect) {
  timer1_millis++;
}

TaskScheduler::TaskScheduler() {
  TCCR1B |= (1 << WGM12) | (1 << CS11);
  OCR1AH = (CTC_MATCH_OVERFLOW >> 8);
  OCR1AL = (uint8_t)CTC_MATCH_OVERFLOW;
  TIMSK1 |= (1 << OCIE1A);
  DDRC |= (1 << PC0);
  _count = 0;
}

unsigned long TaskScheduler::millis() {
  unsigned long m;
  ATOMIC_BLOCK(ATOMIC_FORCEON) {
    m = timer1_millis;
  }
  return m;
}

void TaskScheduler::addTask(void (*function)(), int interval) {
  _tasks[_count].previous = 0;
  _tasks[_count].interval = interval;
  _tasks[_count].function = function;
  _count++;
}

void TaskScheduler::loop() {
  unsigned long time = millis();
  for(int i = 0; i < NOTASKS; i++) {
    Task task = _tasks[i];
    if (time - task.previous > task.interval) {
      _tasks[i].previous = time;
      task.function();
    }
  }
  _delay_ms(TASKLOOPWAITMS);
}
