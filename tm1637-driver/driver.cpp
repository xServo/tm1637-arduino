#include "driver.h"

// indexed to 0-9 followed by A-F followed by degree symbol
const int hex_codes[17] = {
  0x3F, // 0
  0x30, // 1
  0x5B, // 2
  0x79, // 3
  0x74, // 4
  0x6D, // 5
  0x6F, // 6
  0x38, // 7
  0x7F, // 8
  0x7D, // 9

  0x7E, // A
  0x67, // b
  0x0F, // C
  0x73, // d
  0x4F, // E
  0x4E, // F

  0x5C  // degree
};

const int colon_code = 0x80; // add this to a hex code to include the colon

void Setup() {
  // set up pins
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);  // take control
}

void SetDisplay(uint8_t isOn) {
  // turn display on or off
  Start();

  if (isOn)
    Execute(0x88);
  else 
    Execute(0x80);

  Ack();
  Stop();
}

// writes the given array to the screen rightmost/lowest idx element is the leftmost display digit
void Write(uint8_t data[4], uint8_t has_colon)  {
  // write to disp
  Start();
  Execute(0x40);
  Ack();
  
  // data
  Execute(hex_codes[data[3]]);
  Ack();
  Execute(hex_codes[data[2]] | (has_colon * colon_code));
  Ack();
  Execute(hex_codes[data[1]]);
  Ack();
  Execute(hex_codes[data[0]]);
  Ack();
  Stop();

  ResetAddr();
}

// goes back to first digit on display
void ResetAddr() {
  Start();
  Execute(0xC0);
  Ack();
  Stop();
}

// start the i2c transmission
void Start() {
  SetClock(HIGH);
  digitalWrite(data_pin, HIGH);
  delayMicroseconds(clock_period);
  digitalWrite(data_pin, LOW);
}

// end the i2c transmission
void Stop() {
  SetClock(LOW);
  delayMicroseconds(clock_period);
  digitalWrite(data_pin, LOW);
  delayMicroseconds(clock_period);
  SetClock(HIGH);
  delayMicroseconds(clock_period);
  digitalWrite(data_pin, HIGH);
}

// receive i2c acknowledgement
void Ack() {
  SetClock(LOW);
  // clock is low
  pinMode(data_pin, INPUT);  // release control
  delayMicroseconds(clock_period);

  // set hgigh
  SetClock(HIGH);
  delayMicroseconds(clock_period);

  // receive ACK
  if(digitalRead(data_pin) != 0) {
    Serial.println("ACK Not received!");
  }

  // set low
  SetClock(LOW);
  delayMicroseconds(clock_period);

  // REGAIN control
  pinMode(data_pin, OUTPUT);
}

void Execute(uint8_t data) {
  for (int i=0; i<8; i++) {
    // set clock to low
    SetClock(LOW);

    // extract first bit 
    uint8_t bit = ((data >> i) & 1) == 1;

    // write first bit
    digitalWrite(data_pin, bit);
    // Serial.print(bit);

    // step clock
    delayMicroseconds(clock_period);
    SetClock(HIGH);
    delayMicroseconds(clock_period);
  }
}

void Halt() {
  while (1) {
    delay(1000);
  }
}

void SetClock(int val) {
  digitalWrite(clock_pin, val);
}
