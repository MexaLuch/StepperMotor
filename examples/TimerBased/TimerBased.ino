/*
  Пример использования StepperMotor v2.0 с аппаратными таймерами ESP32
  
  Простое управление 3 шаговыми двигателями
  с использованием оптимизированных таймеров ESP32
  
  Подключение:
  Мотор 1: ENA=12, DIR=14, PUL=27
  Мотор 2: ENA=12, DIR=26, PUL=25
  Мотор 3: ENA=12, DIR=33, PUL=32
*/

#include "StepperMotor.h"

// Создаем объекты моторов
StepperMotor motor1(12, 14, 27);  // ENA=12, DIR=14, PUL=27
StepperMotor motor2(12, 26, 25);  // ENA=12, DIR=26, PUL=25
StepperMotor motor3(12, 33, 32);  // ENA=12, DIR=33, PUL=32

void setup() {
  Serial.begin(115200);
  Serial.println("=== StepperMotor v2.0 ===");
  
  // Инициализируем моторы
  motor1.init();
  motor2.init();
  motor3.init();
  
  // Включаем моторы
  motor1.enable();
  motor2.enable();
  motor3.enable();
  
  // Назначаем таймеры моторам
  assignTimerToMotor(&motor1, TIMER_GROUP_0, TIMER_0);
  assignTimerToMotor(&motor2, TIMER_GROUP_0, TIMER_1);
  assignTimerToMotor(&motor3, TIMER_GROUP_1, TIMER_0);
  
  Serial.println("Система готова!");
  Serial.println("Команды: 1,2,3 - управление моторами, a - все вместе, s - остановка");
}

void loop() {
  // Обработка команд через Serial
  if (Serial.available()) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case '1':
        Serial.println("Мотор 1: 400 шагов");
        motor1.go(400, 100);
        break;
        
      case '2':
        Serial.println("Мотор 2: 400 шагов");
        motor2.go(400, 100);
        break;
        
      case '3':
        Serial.println("Мотор 3: 400 шагов");
        motor3.go(400, 100);
        break;
        
      case 'a':
        Serial.println("Все моторы одновременно");
        motor1.go(500, 100);
        motor2.go(500, 100);
        motor3.go(500, 100);
        break;
        
      case 's':
        Serial.println("Остановка всех моторов");
        motor1.stop();
        motor2.stop();
        motor3.stop();
        break;
    }
  }
}
