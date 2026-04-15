/*
--Это код перчатки

Проект Glove состоит из перчатки(манипулятора) и машинки (которым управляют)

Перчатка является сервером к которой подключается машинка
Перчатка имеет Flex sensore, он меняет сопротивление при его сгибе.
Мы фиксируем на АЦП какое напряжение падает на этом сенсоре дальше 
преобразует диапазон с (1800-600) в (0-255) чтобы можно было управлять машинкой с помощью шим.
Все числа меньше 100 превращаем в 0 , потому что такой шим не сможет запустить двигатели .
Так-же с помощью этого числа управляется шим подающийся на светодиоды .
Светодиоды на плате нужны для более удобного управления перчаткой, 
средний светодиод мигает когда налажена связь между машинкой и перчаткой и выводит значения шим машинки на светодиоды . 
*/
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <esp_now.h>
#include <WiFi.h>

#define TCAADDR 0x70
#define NUM_SENSORS 3
#define NUM_MOTORS 4
#define SDA 21
#define SCL 22

#define FOR_N(n) for (int i = 0; i < (n); i++)

const bool IS_CATHODE = true;
const int MIN_ANGLE = 90;
const int MAX_ANGLE = 0;
unsigned long lastCheckTime;
uint8_t broadcastAddress[] = {0xe0, 0x5a, 0x1b, 0x75, 0x85, 0xc8};

const int PIN_LED[NUM_SENSORS] = { 19, 18, 5 };
Adafruit_MPU6050 mpu[NUM_SENSORS];
float rolls[NUM_SENSORS];
int stopVals[NUM_SENSORS] = {0};

union MotorData {
    struct {
        signed char a, b, c, d;
    };
    signed char arr[4];
};

MotorData motorData;
esp_now_peer_info_t peerInfo;

void tcaselect(uint8_t i) {
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

void setup() {
  Serial.begin(921600);
  Serial.println("Start!");

  Wire.begin(SDA, SCL);   //I2C setup
  mpuSetup();

  //sensors and LED setup
  FOR_N(NUM_SENSORS) ledcAttach(PIN_LED[i], 40000, 8);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_send_cb(esp_now_send_cb_t(onDataSent));
  
  // Register peer
  memset(&peerInfo, 0, sizeof(peerInfo));
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }

  setupStopValues(3000); 
  FOR_N(NUM_SENSORS) Serial.printf("stop value[%d] = %d\troll[%d] = %f\n", i, stopVals[i], i, rolls[i]);
}

void loop() {
  int sensVals[NUM_SENSORS];
  readSensors(sensVals);
  writeLeds(sensVals);

  formDataStruct(sensVals);
  trySendData();

  //for sens values
  FOR_N(NUM_SENSORS) Serial.printf("sensor value[%d] = %d\troll[%d] = %f\n", i, sensVals[i], i, rolls[i]);
  //for motors
  // FOR_N(NUM_MOTORS)Serial.printf("motor[%d]: %d\n", i, motorData.arr[i] + 0);
  delay(100);
}