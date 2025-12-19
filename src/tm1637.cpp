// 2025-12-07
// Ryan Blachowicz
// TM1637 basic functionality

#include "driver.h"
#include "tm1637.h"

Display::Display() 
  : active(0), temperatureScale(0) 
{
  Setup();
  ResetAddr();
}


void Display::step() {
  if (!active || isPaused) return;

  if (active == stopwatch) {
    // Serial.println(1);
    // ensure 1 second has passed
    if ((millis() - time) < 1000) { return; }
    cnt++;

    // reset time
    time = millis();


    // display the stopwatch
    displayTime(cnt);

    // stop after 60m
    if (cnt >= 3600) {
      pauseStopwatch();
      return;
    }

  }

  if (active == timer) {
    // ensure 1 second has passed
    if ((millis() - time) < 1000) { return; }
    cnt--;

    // reset time
    time = millis();


    // display the stopwatch
    displayTime(cnt);

    // stop upon hitting 0
    if (cnt == 0) {
      pauseStopwatch(); // not the best name 
      return;
    }
    
  }
} 

// cnt represents how many seconds away from 0
void Display::displayTime(unsigned int cnt) {
  uint8_t minutes = cnt / 60;
  uint8_t seconds = cnt % 60;

  // what gets displayed
  uint8_t vals[4];

  // set the display vals
  vals[0] = minutes / 10;
  vals[1] = minutes % 10;
  vals[2] = seconds / 10;
  vals[3] = seconds % 10;

  // write to display
  Write(vals, 1);
  // uint8_t debug = seconds / 10;
  // Serial.println(debug);
}

void Display::setBrightness(uint8_t val) {
  // check for invalid input
  if (val > 7) return; // TODO error handle

  SetBrightness(val);
}

void Display::write(int a, int b, int c, int d) {
  ResetAddr();

  uint8_t arr[4] = {a, b, c, d};

  Write(arr, 0);
}

void Display::writeTime(int a, int b, int c, int d) {
  ResetAddr();

  uint8_t arr[4] = {a, b, c, d};

  Write(arr, 1);
}

void Display::startStopwatch() {
  cnt = 0;
  displayTime(cnt);
  active = stopwatch;
  time = millis();
  isPaused = 0;
}

void Display::startTimer(int startTime) {
  cnt = startTime;
  displayTime(cnt);
  active = timer;
  time = millis();
  isPaused = 0;
}

void Display::pauseStopwatch() {
  isPaused = 1;
}

void Display::playStopwatch() {
  isPaused = 0;
  time = millis();
}

void Display::toggleTemperatureScale() {
  temperatureScale ^= 1; // flip the bit
}

void Display::setTemperature(uint8_t val) {
  if (val > 99) return; // TODO ERROR HANDLING

  // get temp symbol
  uint8_t scale_reg = 0xC; // celcius
  if (temperatureScale) {
    scale_reg = 0xF; // fahrenheit
  }

  uint8_t to_write[4];
  to_write[0] = val / 10; // upper digit
  to_write[1] = val % 10; // lower digit
  to_write[2] = 0x10; // 0x10 is the degree symbol
  to_write[3] = scale_reg;

  Write(to_write, 0); 
}

// turns display on/off
void Display::activate() { SetDisplay(1); };
void Display::deactivate() { SetDisplay(0); };
