// 2025-12-07
// Ryan Blachowicz
// TM1637 basic functionality

#include "driver.h"


class Display {
  public:
    Display() :
    active(0) 
    {
      Setup();
      ResetAddr();
    }


    void step();

    // void Print(int a, int b, int c, int d) {
    //   ResetAddr();
    //
    //   uint8_t arr[4] = {a, b, c, d};
    //
    //   Write(arr, 0);
    // }

    // turns display on/off
    void activate() { SetDisplay(1); };
    void deactivate() { SetDisplay(0); };

    void startStopwatch() {
      cnt = 0;
      displayTime(cnt);
      active = stopwatch;
      time = millis();
      isPaused = 0;
    }

    void startTimer(int startTime) {
      cnt = startTime;
      displayTime(cnt);
      active = timer;
      time = millis();
      isPaused = 0;
    }


    void pauseStopwatch() {
      isPaused = 1;
    }

    void playStopwatch() {
      isPaused = 0;
      time = millis();
    }

    // TODO DEBUG
    int getCnt() { return cnt; }


  private:
    const int stopwatch = 1;
    const int timer = 2;
    int active;
    bool isPaused;
    unsigned long time;
    unsigned int cnt; // used for stopwatch and timers

    void displayTime(unsigned int cnt);
};

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



Display display;  // decplare display globally
void setup() {
  Serial.begin(9600); 
  display.activate();
  // display.Print(1, 3, 3, 7);
  // display.startStopwatch();
  display.startStopwatch();
}

// variables used in timer test
unsigned long test = 0;
int isTest = 0;

void loop() {
  display.step();
  // Serial.println(display.getCnt());

  // a test to pause the timer at 5s and play again after 5 seconds
  if (display.getCnt() == 5 && !isTest) {
    test = millis();
    display.pauseStopwatch(); 
    isTest = 1;
  }

  if (isTest == 1) {
    if ((millis() - test) > 5000)  {
      display.playStopwatch();
      isTest = 2;
    }
  }

}

