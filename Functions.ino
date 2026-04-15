void mpuSetup() {
  FOR_N(NUM_SENSORS) {
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
  lastCheckTime = millis();
}

void setupStopValues(int waitTime){
  const int LED_INDEX_TO_WRITE = 0;
  if (IS_CATHODE) FOR_N(NUM_SENSORS) ledcWrite(PIN_LED[i], i == LED_INDEX_TO_WRITE ? 0 : 255);
  else ledcWrite(PIN_LED[LED_INDEX_TO_WRITE], 255);
  delay(waitTime);
  int sensVals[NUM_SENSORS];
  readSensors(sensVals);
  FOR_N(NUM_SENSORS) stopVals[i] = sensVals[i] + 10;
  ledcWrite(PIN_LED[0], 0);
}

void writeLeds(int* vals) {
  FOR_N(NUM_SENSORS) {
    if (vals[i] > stopVals[i]) ledcWrite(PIN_LED[i], IS_CATHODE ? 255 - vals[i] : vals[i]);
    else ledcWrite(PIN_LED[i], IS_CATHODE ? 255 : 0);
  }
}

void readSensors(int* arr) {
  float dt = (millis() - lastCheckTime) / 1000.0;
  lastCheckTime = millis();

  FOR_N(NUM_SENSORS) {
    tcaselect(i);
    sensors_event_t a, g, temp;
    mpu[i].getEvent(&a, &g, &temp);

    float rollAcc = atan2(a.acceleration.y, a.acceleration.z) * 180 / PI;
    rolls[i] = 0.98 * (rolls[i] + g.gyro.x * dt * 180 / PI) + 0.02 * rollAcc;
    arr[i] = constrain(map(rolls[i], MAX_ANGLE, MIN_ANGLE, 0, 255), 0, 255);

    if (arr[i] < stopVals[i]) arr[i] = 0;
  }
}