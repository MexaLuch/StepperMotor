/*
  Простой пример управления одним шаговым двигателем
  
  Подключение:
  ENA=12, DIR=14, PUL=27
*/

#include "StepperMotor.h"

// Создаем объект мотора
StepperMotor motor(12, 14, 27);  // ENA=12, DIR=14, PUL=27

void setup() {
  Serial.begin(115200);
  Serial.println("Инициализация мотора...");
  
  // Инициализация мотора
  motor.init();
  motor.enable = true;
  
  Serial.println("Мотор готов к работе!");
}

void loop() {
  // ОБЯЗАТЕЛЬНО: обновляем состояние мотора
  motor.update();
  
  // Движение каждые 3 секунды
  static unsigned long lastCommand = 0;
  static bool direction = true;
  
  if (millis() - lastCommand > 3000) {
    if (direction) {
      motor.setOneStepTime(50);  // Быстро
      motor.setPose(300);
      Serial.println("Движение вправо на 300 шагов");
    } else {
      motor.setOneStepTime(100); // Медленнее
      motor.setPose(-300);
      Serial.println("Движение влево на 300 шагов");
    }
    direction = !direction;
    lastCommand = millis();
  }
  
  // Вывод позиции каждую секунду
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 1000) {
    Serial.print("Текущая позиция: ");
    Serial.println(motor.getCurrentPosition());
    lastPrint = millis();
  }
}
