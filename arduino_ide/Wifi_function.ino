//Передаем строку по wifi машинке и Выводим значения в Serial порт,
//если значения на датчиках отличаются от прошлых.
void Transmit_string_wifi()                                     
{
  if (sens_val[1] && !sens_val[0] && !sens_val[2]){
    master.print("A" + String(sens_val[1]) + " B" + String(sens_val[1]) 
    + " C" + String(sens_val[1]) + " D" + String(sens_val[1]) + " ");
  }
  else if (sens_val[2] && !sens_val[0] && !sens_val[1]){
     master.print("A" + String(-sens_val[2]) + " B" + String(-sens_val[2])
     + " C" + String(-sens_val[2]) + " D" + String(-sens_val[2]) + " ");
  }
  else if (sens_val[1] && sens_val[0] && !sens_val[2]){
    master.print("A" + String(-sens_val[1]*0.5) + " B" + String(sens_val[1]*0.5)
    + " C" + String(-sens_val[1]*0.5) + " D" + String(sens_val[1]*0.5) + " ");
  }
  
  else if (sens_val[0] && sens_val[2] && !sens_val[1]){
    master.print("A" + String(sens_val[0]*0.5) + " B" + String(-sens_val[0]*0.5)
    + " C" + String(sens_val[0]*0.5) + " D" + String(-sens_val[0]*0.5) + " ");
  }
  else{
    master.print("A0 B0 C0 D0 ");
  }
  master.flush();
  FOR_3 last_sens_val[i] = sens_val[i];
}

// Создание сети wifi
void connectToWiFi()
{
  Serial.println("Подключение к Wi-Fi...");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Blink_conect(1);

  Serial.println(" ");
  Serial.print(F("Connected to Wifi! IP address : "));
  Serial.println(WiFi.localIP());
}

// Подключение через сервер  с машинкой
void connectToServer()
{
  Serial.println("Подключение к серверу...");

  if (master.connect("192.168.4.1", 80)) {
    Serial.println("Connected to server!");
    Blink_conect(1);
  } else Serial.println("Connected to server error!");
}
