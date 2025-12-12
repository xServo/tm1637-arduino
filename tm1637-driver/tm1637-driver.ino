// 2025-12-07
// Ryan Blachowicz
// Put the TM1637 in test mode

const int clock_pin = 13;
const int data_pin = 8;
const int debug_led_pin = 7;  // light up according to clock signal

const int clock_period = 100;  // ms

const uint8_t display_on_data[8] = { 1, 0, 0, 0, 1, 0, 0, 0 };
const uint8_t display_off_data[8] = { 1, 0, 0, 0, 0, 0, 0, 0 };
const uint8_t set1_data[8] = { 1, 1, 0, 0, 0, 0, 0, 0 };       // display set to 00H addr
const uint8_t width_data[8] = { 1, 0, 0, 0, 0, 1, 1, 1 };      // set pulse width
const uint8_t test_mode_data[8] = { 0, 1, 0, 0, 1, 0, 0, 0 };  // test mode?

const uint8_t write_to_disp_reg_data[8] = { 0, 1, 0, 0, 0, 0, 0, 0 };
const uint8_t reg_1[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
const uint8_t reg_2[8] = { 0, 0, 0, 0, 0, 0, 1, 0 };
const uint8_t my_data_1[8] = { 1, 1, 1, 1, 1, 0, 0, 1 };


const uint8_t my_num_1[8] = { 0, 0, 1, 1, 0, 0, 0, 0 };
const uint8_t my_num_2[8] = { 0, 1, 0, 1, 1, 0, 1, 1 };
const uint8_t my_num_3[8] = { 1, 1, 1, 1, 1, 0, 0, 1 };
const uint8_t my_num_4[8] = { 0, 1, 1, 1, 0, 1, 0, 0 };
// const int my_data_1[8] = {0, 1, 1, 1, 1, 1, 1, 0};

void setup() {
  // set up pins
  pinMode(clock_pin, OUTPUT);
  pinMode(data_pin, OUTPUT);  // take control
  Serial.begin(9600);

}

void loop() {
  // Set data here


  // turn on display
  Start();
  Execute(display_on_data);
  Ack();
  Stop();

  // write to disp
  Start();
  Execute(write_to_disp_reg_data);
  Ack();
  //nums
  Execute(my_num_1);
  Ack();
  Execute(my_num_2);
  Ack();
  Execute(my_num_3);
  Ack();
  Execute(my_num_4);
  Ack();
  Stop();

  // Execute(set1_data); // display reg 0
  // for (uint8_t i = 1; i<256; i++) {
  //   Execute(display_on_data);  // THUS MAKES IT START TAKING IN DATA??
  //   uint8_t* data = BinConvert(i);
  //   Execute(data);
  //   Execute(data);
  //   Execute(data);
  //   Execute(data);
  //   free(data);


  //   Init();  // THIS MAKES IT STOP TAKING IN DATA???
  // }

  // Execute(display_off_data);  // 9 steps



  digitalWrite(data_pin, LOW);
  Halt();
}

// takes in unsigned 8 bit
// returns an arr of 8 representing the binary value
uint8_t* BinConvert(uint8_t val) {
  uint8_t* arr = malloc(sizeof(uint8_t) * 8);

  for (int i = 0; i < 8; i++) {
    arr[i] = (val & (1 << i)) != 0;
  }

  return arr;
}

void Start() {
  Serial.println("Init.");
  SetClock(HIGH);
  digitalWrite(data_pin, HIGH);
  delay(1000);
  digitalWrite(data_pin, LOW);
}

void Stop() {
  SetClock(LOW);
  delay(clock_period / 2);
  digitalWrite(data_pin, LOW);
  delay(clock_period / 2);
  SetClock(HIGH);
  delay(clock_period / 2);
  digitalWrite(data_pin, HIGH);
}

// get acknowledgement
void Ack() {
  SetClock(LOW);
  // clock is low
  pinMode(data_pin, INPUT);  // release control
  delay(clock_period / 2);

  // set hgigh
  SetClock(HIGH);
  delay(clock_period / 2);

  // receive ACK
  Serial.println(digitalRead(data_pin));

  // set low
  SetClock(LOW);
  delay(clock_period / 2);

  // REGAIN control
  pinMode(data_pin, OUTPUT);
}

void Execute(uint8_t data[8]) {
  Serial.println("Executing:");
  for (int i = 7; i >= 0; i--) {
  // for (int i=0; i<8; i++) {
    // set clock to low
    SetClock(LOW);


    digitalWrite(data_pin, data[i]);

    Serial.print(data[i]);

    delay(clock_period / 2);

    SetClock(HIGH);
    delay(clock_period / 2);
  }
}

void Halt() {
  while (1) {

    delay(1000);
  }
}
void SetClock(int val) {
  digitalWrite(debug_led_pin, val);
  digitalWrite(clock_pin, val);
}
void Clock_Pulse() {
  // rise clock, data will be read following this
  digitalWrite(debug_led_pin, HIGH);
  digitalWrite(clock_pin, HIGH);
  delay(clock_period / 2);
  // set clock to low
  digitalWrite(debug_led_pin, LOW);
  digitalWrite(clock_pin, LOW);
  delay(clock_period / 2);
}
