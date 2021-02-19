#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x29);

sensors_event_t event; //saves event data from BNO055

//Driver1
int driver1[4] = {19,18,15,5}; //pins connected to driver one

//Driver2
int driver2[4] = {25,33,26,27}; //pins connected to driver two

// pin waarop de joystick is aangesloten
int vrx = 35;  
int vry = 32; 

//button
int button[2] = {13,14};
bool buttonState = 0;

//x and y value joystick
int xValue, yValue;

//timings
//periods between timings
int period[6] ={50,100,250,250,1000,2000};
//stores the time
unsigned long clockMillis[7] = {0,0,0,0,0,0,0};

//sensor        
int trigPins[3] = {13, 16, 17}; //outputPin (Trig) of ultrasonesensor
int echoPins[3] = {32, 35, 34}; //inputPin (Echo) of ultrasonesensor
//distances read by sensors
int OldDistances[3] = {0,0,0};
int Distances[3] = {1000,1000,1000};
int current_sensor = 0;

//pin for reading battery voltage
const int potPin = 36;

// variable for storing the voltage value
double potValue = 0;

//Buzzer
const int BUZZER = 14;
bool buzzerState;

String last_movement;

//State variables
bool speedState = 0;      //Did the speed change?
bool moveState = 0;     //Did the wheels turn on change?
bool rotState = 0;      //Are you correcting rotation change?
bool onHill = 0;      //Are you on a hill?
bool afterHill = 0;     //Did you just go off a hill?
bool movedB = false;      //Did you move back?

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_batmobile"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
  
  //Make a sound if booting
  digitalWrite(BUZZER, HIGH);
  delay(2);
  digitalWrite(BUZZER, LOW);
  delay(1000);
  digitalWrite(BUZZER, HIGH);
  delay(2);
  digitalWrite(BUZZER, LOW);

  bno_setup();
  
  pinMode(vrx, INPUT);
  pinMode(vry, INPUT);
  pinMode(button[0], INPUT);
  pinMode(button[1], INPUT);
  pinMode(BUZZER, OUTPUT);
  for(int i = 0; i < 3; i++){
    pinMode(trigPins[i], OUTPUT);
    pinMode(echoPins[i], INPUT);
  }
  
  for(int i = 0; i < 7; i++){
    clockMillis[i] = millis();
  }

  for(int i = 0; i < 4; i++){
    pinMode(driver1[i], OUTPUT);
    digitalWrite(driver1[i], LOW);
    pinMode(driver2[i], OUTPUT);
    digitalWrite(driver2[i], LOW);
  }
  
  SerialBT.println("Booted");
  delay(1000);
  SerialBT.println("Booted");
  delay(1000);
  SerialBT.println("Booted");
}

void loop() {
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }
  if(timer(4)){
    printData();
  }
  
  if(speedState){ //Change speed to default
    period[0] = 50;
    period[2] = 250;
    period[3] = 250;
    speedState = false;
  }
  //buttonCheck();
  //JoyStick();
  
  potValue = double (analogRead(potPin)/ double(4095)) * 3.3 * 3; //read battery voltage
  Alarm(); //battery alarm if voltage is to low
  if(timer(1) && !moveState){ //if not moving read sensors every 100ms
    bno_read();     //Read gyro data
    echo();     //Read sensor data
  }
  CorrectRot();     //Rotation correction
  PathFinding();
  if(timer(0)){ //turn off wheels every 50ms or 25ms to slow down (only when moving)
    off();
  }
}

bool timer(int i) { //timer functions times multiple events in the code
  if (millis() >= clockMillis[i]){clockMillis[i] = (millis() + period[i]); return true;}
  else {return false;}
}

void Alarm(){ //battery alarm if voltage is to low
  while(potValue < 6 && potValue > 4){ //while loop prevents code main loop from running while battery is low
      digitalWrite(BUZZER, buzzerState);
      if (millis() >= clockMillis[4]){clockMillis[4] = millis() + period[4]; buzzerState = !buzzerState; }
      else{digitalWrite(BUZZER, LOW);}
  }
}

void printData(){
  SerialBT.println(last_movement);
  /* Display the floating point data */
  SerialBT.print("X: ");
  SerialBT.print(event.orientation.x, 4);
  SerialBT.print("\Y: ");
  SerialBT.print(event.orientation.y, 4);
  SerialBT.print("\Z: ");
  SerialBT.print(event.orientation.z, 4);

  /* Optional: Display calibration status */
  displayCalStatus();

  /* New line for the next sample */
  SerialBT.println("");
  for(int i = 0; i < 5; i++){
    SerialBT.print("period");
    SerialBT.print(i);
    SerialBT.print(": ");
    SerialBT.print(period[i], 5);
    SerialBT.print("/");
  }
  SerialBT.println("");
    for(int i = 0; i < 3; i++){
    SerialBT.print("Afstand ");
    SerialBT.print(i);
    SerialBT.print(" : ");
    SerialBT.print(Distances[i]);
    SerialBT.print(" cm");
    SerialBT.print("/");
  }
  SerialBT.println("");
  SerialBT.print(last_movement);
  SerialBT.println("");
  SerialBT.println("------------------------");
}
