/* This is the code for the atmega328 based rotary position encoder using the TLE5501E0001XUMA1 analog position sensor
 * This device can send data with I2C or PWM. Quadratue to be supported eventualy
 * I2C Commands:
 * <e> Exists: returns i2c address
 * <m<state>> Mode: 1 = track wraparound, 0 = only track angle
 * <r> Reading: returns angle
 * <t> Total: returns total angle
 */

#include <Wire.h>

// Settings
#define DEBUG         // Debug mode - sends data over Serial
#define I2C           // I2C mode - enables I2C communitaion
#define PWM           // PWM mode - enables PWM output
// Not supported yet
//#ifndef PWM
//  #define QUADRATURE    // QUADRATURE - enables QUADRATURE output. you can't have QUADRATURE and PWM
//#endif

// Device characteristics
#define I2C_ADDRESS 6
#define PWM_PIN 2
#define QUADRATURE_PIN 1
#define angleRange 999
#define pwmScaler 0.2552
#define COS_P 1
#define COS_N 2
#define SIN_P 3
#define SIN_N 4

// Messages
#define Reading 'r'
#define Angle 'a'
#define TotalRotations 't'
#define Exist 'e'
#define Moad 'm'


// Varables
bool mode = false;
int angleAbs = -1;
int lastAngleAbs = -1;
double totalRotations = 0; 

// Setup: runs once
void setup() {
  // Debug mode
  #ifdef DEBUG
    Serial.begin(9600);
    Serial.println("Starting device");
    #ifdef I2C
      Serial.println(I2C_ADDRESS);
    #endif
  #endif

  // I2C init
  #ifdef I2C
    Wire.begin(I2C_ADDRESS);
    Wire.onReceive(receiveEvent);
  #endif
  // PWM init
  #ifdef PWM
    pinMode(PWM_PIN, OUTPUT);
  #endif
  // QUADRATURE init
  #ifdef QUADRATURE
    pinMode(PWM_PIN, OUTPUT);
    pinMode(QUADRATURE_PIN, OUTPUT);
  #endif
  
  // Debug mode
  #ifdef DEBUG
    Serial.println("Init Compleate!");
  #endif
}

// loop: loops
void loop() {
  updateAngle();
  #ifdef PWM
    updatePWM();
  #endif
}

// I2C Recive event
void receiveEvent(int howMany) {
  // Read msg
  char msg[2];
  for(int i = 0; Wire.available(); i++) {
    if(i < 2) {
      msg[i] = Wire.read();
    } else {
      Wire.read();
    }
  }
  // Commands
  if(msg[0] == Reading) {
    Wire.write(angleAbs);
  } else if(msg[0] == TotalRotations) {
    Wire.write(totalRotations);
  } else if(msg[0] == Exist) {
    Wire.write(I2C_ADDRESS);
  } else if(msg[0] == Moad) {
    mode = bool(int(msg[1]) - 48);
    Wire.write('1');
  }
}

void updateAngle() {
  int cos_p = analogRead(COS_P);
  int cos_n = analogRead(COS_N);
  int sin_p = analogRead(SIN_P);
  int sin_n = analogRead(SIN_N);
  // Insert math here
  if(mode) {
    // Insert wraparound here
  }
}

void updatePWM() {
  float a = pwmScaler * angleAbs;
  analogWrite(PWM_PIN, a);
}
