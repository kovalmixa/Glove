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

#include <WiFi.h>

#define FOR_3 for (int i = 0; i < 3; i++)

const bool isDebug = false; //Switch to true, if need info during calibration
const char *ssid = "CAR_WIFI";
const char *password = "12345678";
int last_sens_val[3], sens_val[3], stop_val[3], PIN_SENS[3] = { 33, 32, 35 }, PIN_LED[3] = { 19, 18, 5 };

WiFiClient master;

void setup()
{
  Serial.begin(115200);
  Serial.println("Start!");
  FOR_3 ledcSetup(i, 40000, 8);        // настройка шим для светодиодов (i-канал;40000-частота;8(біт)-разрешение)
  FOR_3 ledcAttachPin(PIN_LED[i], i);  // настройка связи пинов с их каналами

  Setup_sensors();
  FOR_3 Serial.println(stop_val[i]);
}

void loop()
{
  if(WiFi.status() != WL_CONNECTED && !isDebug){                           // Автоконнект/Реконнект
    connectToWiFi();
    connectToServer();
  }
  Enter_and_convert_values();                                  // Ввод c АЦП значений напряжения и преобразование в нужний диапозон для последующей подачи значений машинке
  Values_per_LED();                                            // Подача значений в виде шим сигнала на светодиод
  Transmit_string_wifi();                                      // Передаем строку по wifi машинке
  FOR_3 Serial.printf("sens_val[%d] = %d\t%d\n", i, sens_val[i], analogRead(PIN_SENS[i]));  // Выводим значения в Serial порт
  delay(50);
}
