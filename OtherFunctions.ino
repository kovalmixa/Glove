void setupStopValues(){
  ledcWrite(PIN_LED[0], 255);
  delay(3000);
  int sensVals[3];
  readSensors(sensVals);
  FOR_N(3) stopVals[i] = sensVals[i] + 10;
  ledcWrite(PIN_LED[0], 0);
}

void writeLEDs(int* vals) {
  FOR_N(3) {
    if (vals[i] > stopVals[i]) ledcWrite(PIN_LED[i], vals[i]);
    else ledcWrite(PIN_LED[i], 0);
  }
}

void readSensors(int* arr) {
  FOR_N(3) {
    if (i) arr[i] = map(analogRead(PIN_SENS[i]), 1800, 400, 0, 255);
    else arr[i] = map(analogRead(PIN_SENS[0]), 1850, 1500, 0, 255);
    arr[i] = constrain(arr[i], 0, 255);
  }
  FOR_N(3) if (arr[i] < stopVals[i]) arr[i] = 0;
}