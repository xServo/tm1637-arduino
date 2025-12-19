#pragma once

#include <Arduino.h>

class Display {
  public:
    Display();

    // needs to be called frequently to keep the timer/stopwatch accurate
    void step();

    // doesn't include the colon
    // a is left most reg
    void write(int a, int b, int c, int d);

    // includes the colon
    void writeTime(int a, int b, int c, int d); 

    // turns display on/off
    void activate();
    void deactivate();

    // set the brightness 0 to 7 inclusive
    void setBrightness(uint8_t val);

    void startStopwatch();

    void startTimer(int startTime); 


    void pauseStopwatch();

    void playStopwatch(); 

    // toggles between celcius and fahrenheit
    void toggleTemperatureScale();

    void setTemperature(uint8_t val);

    int getCnt() { return cnt; }


  private:
    // constants
    const int stopwatch = 1;
    const int timer = 2;
    const int celcius = 0;
    const int fahrenheit = 1;

    // state info
    uint8_t active; // Are timer or stopwatch active?
    unsigned long time; // Last polled time.
    unsigned int cnt; // Seconds from 0. Used for timers.

    bool isPaused; 
    uint8_t temperatureScale; // Defaults to celcius.

    void displayTime(unsigned int cnt); 
};

