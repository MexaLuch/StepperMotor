/*
  Простой тест StepperMotor v2.0
  
  Один мотор крутится на 100 шагов 2 раза в секунду
  со скоростью 400 шагов в секунду
  
  Подключение:
  ENA=12, DIR=14, PUL=27
*/

#include <StepperMotor.h>

StepperMotor motor(12, 14, 27);  // ENA=12, DIR=14, PUL=27

void setup() {
  Serial.begin(115200);
  Serial.println("=== Простой тест StepperMotor v2.0 ===");
  
  // Инициализация мотора
  motor.init();
  motor.enable();
  
  // Назначение таймера мотору
  assignTimerToMotor(&motor, TIMER_GROUP_0, TIMER_0);
  
  Serial.println("Мотор готов к работе!");
  Serial.println("Скорость: 400 шагов/сек");
  Serial.println("Движение: 100 шагов каждые 500 мс");
}

void loop() {
  // Движение на 100 шагов со скоростью 400 шагов/сек
  // Период = 1,000,000 мкс / 400 = 2,500 мкс
  motor.go(100, 2500);
  
  // Ждем 500 мс (2 раза в секунду)
  delay(500);
}
