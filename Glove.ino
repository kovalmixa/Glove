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

#include <esp_now.h>
#include <WiFi.h>

#define FOR_3 for (int i = 0; i < 3; i++)

const bool isDebug = false; //Switch to true, if need info during calibration
uint8_t broadcastAddress[] = {0xe0, 0x5a, 0x1b, 0x75, 0x85, 0xc8};
int stopVals[3] = {0}, PIN_SENS[3] = { 33, 32, 35 }, PIN_LED[3] = { 19, 18, 5 };

typedef struct MotorDataStruct {
  signed char a;
  signed char b;
  signed char c;
  signed char d;
  MotorDataStruct(char a, char b, char c, char d) : a(a), b(b), c(c), d(d) {}
  MotorDataStruct() : a(0), b(0), c(0), d(0) {}
} MotorDataStruct;

MotorDataStruct motorData;
esp_now_peer_info_t peerInfo;



void setup() {
  Serial.begin(115200);
  Serial.println("Start!");

  //sensors and LED setup
  FOR_3 ledcAttach(PIN_LED[i], 40000, 8); 

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
  setupStopValues(); 
  FOR_3 Serial.println(stopVals[i]);
}

void loop() {
  int sensVals[3];
  readSensors(sensVals);
  writeLEDs(sensVals);

  formDataStruct(sensVals);
  trySendData();

  FOR_3 Serial.printf("sensor[%d] = %d\t%d\n", i, sensVals[i], analogRead(PIN_SENS[i]));
  Serial.printf("motor data: %d,%d,%d,%d", motorData.a, motorData.b, motorData.c, motorData.d);
  delay(100);
}