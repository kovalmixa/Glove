void Values_per_LED()  // Выводим значения в виде шим сигнала на светодиод
{
  FOR_3 {
    if (sens_val[i] > 100){
      ledcWrite(i, (int)(sens_val[i] * 1.66) - 169);
    }
    else ledcWrite(i, 0);
  }
}

void Enter_and_convert_values() // Берем значения с АЦП и преобразуем в ШИМ данные для машинки 
{
  //FOR_3 sens_val[i] = analogRead(PIN_SENS[i]);                          // Выведение значений с аналого_цифрового преобразователя
  FOR_3 sens_val[i] = map(analogRead(PIN_SENS[i]), 1800, 600, 0, 255);  // Преобрзование значений с одного диапозона в другой
  FOR_3 sens_val[i] = constrain(sens_val[i], 0, 255);                   // Ограничение значений в заданом диапозоне (0 - 255)
  if (sens_val[0] < 140) sens_val[0] = 0;      
  if (sens_val[1] < 210) sens_val[1] = 0;                         // Задаем предельное значение шим сигнала
  if (sens_val[2] < 160) sens_val[2] = 0;
}

void Blink_conect(int pin)  // Функция мигания светодиодом
{
    for (int j = 0; j < 3; j++) {
    ledcWrite(pin,255);
    delay(300);
    ledcWrite(pin,0);
    delay(300);
  }
}
