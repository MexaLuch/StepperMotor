/*
  Пример параллельного управления несколькими шаговыми двигателями
  
  Этот пример демонстрирует, как одновременно управлять тремя моторами
  без блокировки основного цикла программы.
  
  Подключение:
  Мотор 1: ENA=12, DIR=14, PUL=27
  Мотор 2: ENA=12, DIR=26, PUL=25  
  Мотор 3: ENA=12, DIR=33, PUL=32
*/

#include "StepperMotor.h"

// Создаем объекты для трех моторов
StepperMotor motor1(12, 14, 27);  // ENA=12, DIR=14, PUL=27
StepperMotor motor2(12, 26, 25);  // ENA=12, DIR=26, PUL=25
StepperMotor motor3(12, 33, 32);  // ENA=12, DIR=33, PUL=32

void setup() {
  Serial.begin(115200);
  Serial.println("Инициализация системы...");
  
  // Инициализация всех моторов
  motor1.init();
  motor2.init();
  motor3.init();
  
  // Включаем моторы
  motor1.enable = true;
  motor2.enable = true;
  motor3.enable = true;
  
  Serial.println("Моторы инициализированы и готовы к работе!");
}

void loop() {
  // ОБЯЗАТЕЛЬНО: обновляем состояние всех моторов
  motor1.update();
  motor2.update();
  motor3.update();
  
  // Пример 1: Мотор 1 движется каждые 2 секунды
  static unsigned long lastCommand1 = 0;
  static bool direction1 = true;
  
  if (millis() - lastCommand1 > 2000) {
    if (direction1) {
      motor1.setOneStepTime(50);  // Быстро
      motor1.setPose(200);
    } else {
      motor1.setOneStepTime(75);  // Медленнее
      motor1.setPose(-200);
    }
    direction1 = !direction1;
    lastCommand1 = millis();
  }

  // Пример 2: Мотор 2 движется каждую секунду
  static unsigned long lastCommand2 = 0;
  static bool direction2 = true;
  
  if (millis() - lastCommand2 > 1000) {
    if (direction2) {
      motor2.setOneStepTime(60);
      motor2.setPose(150);
    } else {
      motor2.setOneStepTime(80);
      motor2.setPose(-150);
    }
    direction2 = !direction2;
    lastCommand2 = millis();
  }

  // Пример 3: Мотор 3 движется каждые 500мс
  static unsigned long lastCommand3 = 0;
  static bool direction3 = true;
  
  if (millis() - lastCommand3 > 500) {
    if (direction3) {
      motor3.setOneStepTime(40);
      motor3.setPose(100);
    } else {
      motor3.setOneStepTime(60);
      motor3.setPose(-100);
    }
    direction3 = !direction3;
    lastCommand3 = millis();
  }
  
  // Вывод позиций каждые 3 секунды
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint > 3000) {
    Serial.print("Позиции: М1=");
    Serial.print(motor1.getCurrentPosition());
    Serial.print(" М2=");
    Serial.print(motor2.getCurrentPosition());
    Serial.print(" М3=");
    Serial.println(motor3.getCurrentPosition());
    lastPrint = millis();
  }
}
