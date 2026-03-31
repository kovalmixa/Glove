#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// I2C address of multiplecsor TCA9548A
#define TCAADDR 0x70
#define NUM_SENSORS 3
#define SDA 21
#define SCL 22
Adafruit_MPU6050 mpu[NUM_SENSORS];

const int MIN_ANGLE = -45;
const int MAX_ANGLE = 0;
float rolls[NUM_SENSORS];
unsigned long lastTime;

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void mpuSetup() {
  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    tcaselect(i);
    if (!mpu[i].begin()) {
      Serial.print("MPU6050 on channel ");
      Serial.print(i);
      Serial.println(" not found!");
    } else {
      Serial.print("MPU6050 on channel ");
      Serial.print(i);
      Serial.println(" initialized.");
    }
    rolls[i] = 0;
  }
}

void setup() {
  Serial.begin(921600);
  Serial.println("loooool");
  Wire.begin(SDA, SCL);   //I2C setup
  mpuSetup();
  
  lastTime = millis();
}

void loop() {
  float dt = (millis() - lastTime) / 1000.0;
  lastTime = millis();

  for (uint8_t i = 0; i < NUM_SENSORS; i++) {
    tcaselect(i);
    sensors_event_t a, g, temp;
    mpu[i].getEvent(&a, &g, &temp);
    float rollAcc = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
    rolls[i] = 0.98 * (rolls[i] + g.gyro.x * dt * 180 / PI) + 0.02 * rollAcc;

    float rollMapVal = constrain(map(rolls[i], MAX_ANGLE, -MIN_ANGLE, 0, 255), 0, 255);
    
    Serial.print("S");
    Serial.print(i);
    Serial.print(" Roll:");
    Serial.print(rollMapVal);
    Serial.print("  ");
  }
  
  Serial.println();
  delay(10);
}