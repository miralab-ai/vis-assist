#include <Arduino.h>

#define NUM_OF_TAPS 10

// Distance motor pins
const int motor_pin_dis_1 = 3;
const int motor_pin_dis_2 = 5;
const int motor_pin_dis_3 = 6;
const int motor_pin_dis_4 = 9;

// Object motor pins (digital)
const int motor_pin_obj_1 = 22;
const int motor_pin_obj_2 = 23;
const int motor_pin_obj_3 = 24;
const int motor_pin_obj_4 = 25;
const int motor_pin_obj_5 = 26;
const int motor_pin_obj_6 = 27;
const int motor_pin_obj_7 = 28;
const int motor_pin_obj_8 = 29;
const int motor_pin_obj_9 = 30;

const int obj_motor_pins[9] = {
  motor_pin_obj_1,
  motor_pin_obj_2,
  motor_pin_obj_3,
  motor_pin_obj_4,
  motor_pin_obj_5,
  motor_pin_obj_6,
  motor_pin_obj_7,
  motor_pin_obj_8,
  motor_pin_obj_9
};

String inputBuffer = "";

// Object class tap arrays
uint8_t person[NUM_OF_TAPS]         = {1, 1, 2, 2, 3, 3, 3, 2, 2, 1};
uint8_t bicycle[NUM_OF_TAPS]        = {2, 0, 2, 0, 2, 0, 0, 0, 0, 8};
uint8_t car[NUM_OF_TAPS]            = {1, 0, 1, 0, 1, 0, 0, 8, 8, 8};
uint8_t motorcycle[NUM_OF_TAPS]     = {2, 0, 2, 0, 2, 0, 0, 8, 8, 8};
uint8_t bus[NUM_OF_TAPS]            = {6, 0, 6, 0, 6, 0, 0, 8, 8, 8};
uint8_t train[NUM_OF_TAPS]          = {6, 6, 0, 6, 0, 0, 8, 8, 8, 8};
uint8_t truck[NUM_OF_TAPS]          = {6, 6, 0, 6, 6, 6, 0, 8, 8, 8};
uint8_t traffic_light[NUM_OF_TAPS]  = {8, 8, 0, 5, 5, 0, 0, 1, 2, 3};
uint8_t fire_hydrant[NUM_OF_TAPS]   = {8, 8, 0, 5, 5, 0, 0, 3, 2, 1};
uint8_t bench[NUM_OF_TAPS]          = {3, 0, 3, 3, 3, 0, 4, 0, 4, 0};
uint8_t backpack[NUM_OF_TAPS]       = {3, 0, 0, 0, 5, 5, 0, 0, 0, 3};
uint8_t bottle[NUM_OF_TAPS]         = {8, 0, 0, 8, 0, 0, 0, 0, 0, 5};
uint8_t cup[NUM_OF_TAPS]            = {8, 0, 0, 5, 0, 0, 0, 0, 0, 2};
uint8_t chair[NUM_OF_TAPS]          = {2, 2, 5, 5, 8, 8, 8, 5, 5, 2};
uint8_t bed[NUM_OF_TAPS]            = {2, 2, 2, 2, 6, 6, 6, 3, 3, 3};
uint8_t dining_table[NUM_OF_TAPS]   = {6, 6, 0, 1, 1, 0, 6, 3, 3, 3};
uint8_t tv[NUM_OF_TAPS]             = {8, 8, 0, 6, 0, 6, 0, 6, 0, 6};
uint8_t laptop[NUM_OF_TAPS]         = {3, 3, 0, 6, 5, 4, 0, 0, 1, 1};
uint8_t keyboard[NUM_OF_TAPS]       = {1, 5, 3, 5, 1, 5, 3, 5, 1, 5};

void setup() {
  Serial.begin(9600);

  // Set distance driver pins
  pinMode(motor_pin_dis_1, OUTPUT);
  pinMode(motor_pin_dis_2, OUTPUT);
  pinMode(motor_pin_dis_3, OUTPUT);
  pinMode(motor_pin_dis_4, OUTPUT);

  // Set object driver pins
  for (int i = 0; i < 9; i++) {
    pinMode(obj_motor_pins[i], OUTPUT);
    digitalWrite(obj_motor_pins[i], LOW);  // Initially off
  }
}

void loop() {
  while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    if (inChar == '|') {
      parseAndDrive(inputBuffer);
      inputBuffer = "";
    } else {
      inputBuffer += inChar;
    }
  }
}

void parseAndDrive(String data) {
  int commaIndex = data.indexOf(',');
  if (commaIndex == -1) return;

  String label = data.substring(0, commaIndex);
  int distance = data.substring(commaIndex + 1).toInt();

  distance_driver(distance);
  object_driver(label);
}

void object_driver(String label) {
  uint8_t* taps = nullptr;

  if (label == "person") taps = person;
  else if (label == "bicycle") taps = bicycle;
  else if (label == "car") taps = car;
  else if (label == "motorcycle") taps = motorcycle;
  else if (label == "bus") taps = bus;
  else if (label == "train") taps = train;
  else if (label == "truck") taps = truck;
  else if (label == "traffic_light") taps = traffic_light;
  else if (label == "fire_hydrant") taps = fire_hydrant;
  else if (label == "bench") taps = bench;
  else if (label == "backpack") taps = backpack;
  else if (label == "bottle") taps = bottle;
  else if (label == "cup") taps = cup;
  else if (label == "chair") taps = chair;
  else if (label == "bed") taps = bed;
  else if (label == "dining_table") taps = dining_table;
  else if (label == "tv") taps = tv;
  else if (label == "laptop") taps = laptop;
  else if (label == "keyboard") taps = keyboard;

  if (taps == nullptr) return;

  for (int i = 0; i < NUM_OF_TAPS; i++) {
    // Turn off all object motors
    for (int j = 0; j < 9; j++) {
      digitalWrite(obj_motor_pins[j], LOW);
    }

    uint8_t tap = taps[i];

    if (tap >= 1 && tap <= 9) {
      digitalWrite(obj_motor_pins[tap - 1], HIGH);  // 1-indexed to 0-indexed
    }

    delay(120); // 120ms delay between taps
  }

  // Ensure all motors are turned off at the end
  for (int j = 0; j < 9; j++) {
    digitalWrite(obj_motor_pins[j], LOW);
  }
}

// Don't touch the distance_driver()
void distance_driver(int D) {
  Serial.println("Distance Feedback");
  unsigned long int D_max = 500;
  unsigned long int DC_max = 255;
  unsigned long int DC_min = 120;
  unsigned long int M = 0;
  unsigned long int Diff = 0;
  unsigned long int DC1, DC2, DC3, DC4 = 0;
  unsigned long int tmp1 = 0;
  unsigned long int tmp2 = 0;
  unsigned long int tmp3 = 0;

  if (D <= D_max) {
    tmp1 = D_max - D;
    tmp2 = DC_max - DC_min;
    tmp3 = tmp1 * tmp2;
    M = (4 * (D_max - D) * (DC_max - DC_min)) / (D_max);
    Diff = DC_max - DC_min;

    if (M <= (Diff * 3)) {
      DC1 = 0;
      if (M <= (Diff * 2)) {
        DC2 = 0;
        if (M <= Diff) {
          DC3 = 0;
          DC4 = M + DC_min;
        } else {
          DC3 = M - Diff + DC_min;
          DC4 = DC_max;
        }
      } else {
        DC2 = M - (Diff * 2) + DC_min;
        DC3 = DC_max;
        DC4 = DC_max;
      }
    } else {
      DC1 = M - (Diff * 3) + DC_min;
      DC2 = DC_max;
      DC3 = DC_max;
      DC4 = DC_max;
    }

    analogWrite(motor_pin_dis_1, DC1);
    analogWrite(motor_pin_dis_2, DC2);
    analogWrite(motor_pin_dis_3, DC3);
    analogWrite(motor_pin_dis_4, DC4);
  } else {
    analogWrite(motor_pin_dis_1, 0);
    analogWrite(motor_pin_dis_2, 0);
    analogWrite(motor_pin_dis_3, 0);
    analogWrite(motor_pin_dis_4, 0);
  }
}
