void formDataStruct(int* vals) {
  if (!vals[0] && vals[1] && !vals[2])
    motorData = MotorDataStruct(vals[1], vals[1], vals[1], vals[1]);
  else if (!vals[0] && !vals[1] && vals[2])
    motorData = MotorDataStruct(vals[2], vals[2], vals[2], vals[2]);
  else if (vals[0] && vals[1] && !vals[2])
    motorData = MotorDataStruct(-vals[0] * 0.5 , vals[0] * 0.5, -vals[0] * 0.5, vals[0] * 0.5);
  else if (vals[0] && !vals[1] && vals[2])
    motorData = MotorDataStruct(vals[0] * 0.5 , -vals[0] * 0.5, vals[0] * 0.5, -vals[0] * 0.5);
  else motorData = MotorDataStruct();
}

void trySendData() {
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &motorData, sizeof(motorData));
  if (result == ESP_OK) Serial.println("Sent with success");
  else Serial.println("Error sending the data");
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}