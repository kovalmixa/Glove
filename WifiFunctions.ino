void formDataStruct(int* vals) {
  if (!vals[0] && vals[1] && !vals[2]) FOR_N(NUM_MOTORS) motorData.arr[i] = vals[1] / 2;
  else if (!vals[0] && !vals[1] && vals[2]) FOR_N(NUM_MOTORS) motorData.arr[i] = -vals[2] / 2;
  else if (vals[0] && vals[1] && !vals[2]) FOR_N(NUM_MOTORS) motorData.arr[i] = vals[0] / 4 * (i % 2 ? 1 : -1);
  else if (vals[0] && !vals[1] && vals[2]) FOR_N(NUM_MOTORS) motorData.arr[i] = vals[0] / 4 * (i % 2 ? -1 : 1);
  else FOR_N(NUM_MOTORS) motorData.arr[i] = 0;
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