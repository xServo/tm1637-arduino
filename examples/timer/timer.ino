#include <tm1637.h>

#include <Arduino.h>


Display display;
void setup() {
  display.activate();

  // 997 seconds on the timer 
  display.startTimer(997);

  delay(2500);
}

void loop() {
  display.step();
}
