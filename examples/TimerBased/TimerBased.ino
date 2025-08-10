/*
  Пример использования StepperMotor v2.0 с оптимизированными таймерами ESP32
  
  Этот пример демонстрирует новую архитектуру библиотеки,
  использующую аппаратные таймеры ESP32 для максимальной разгрузки CPU.
  
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
  Serial.println("=== StepperMotor v2.0 - Оптимизированная библиотека ===");
  Serial.println("Инициализация системы с оптимизированными таймерами...");
  
  // Инициализируем моторы
  motor1.init();
  motor2.init();
  motor3.init();
  
  // Включаем моторы
  motor1.enable();
  motor2.enable();
  motor3.enable();
  
  // Назначаем таймеры моторам (автоматическая инициализация)
  assignTimerToMotor(&motor1, TIMER_GROUP_0, TIMER_0);
  assignTimerToMotor(&motor2, TIMER_GROUP_0, TIMER_1);
  assignTimerToMotor(&motor3, TIMER_GROUP_1, TIMER_0);
  
  Serial.println("✓ Система готова к работе!");
  Serial.println("\n=== Команды управления ===");
  Serial.println("1 - Мотор 1: медленно (100 мкс)");
  Serial.println("2 - Мотор 2: средне (80 мкс)");
  Serial.println("3 - Мотор 3: быстро (50 мкс)");
  Serial.println("a - Все моторы одновременно");
  Serial.println("s - Остановить все моторы");
  Serial.println("p - Показать позиции");
  Serial.println("h - Возврат домой");
  Serial.println("f - Тест высоких скоростей");
  Serial.println("t - Тест точности позиционирования");
}

void loop() {
  // ОСНОВНОЕ ПРЕИМУЩЕСТВО: больше не нужно вызывать update()!
  // CPU полностью свободен для других задач
  
  // Обработка команд через Serial
  if (Serial.available()) {
    char cmd = Serial.read();
    
    switch (cmd) {
      case '1':
        Serial.println("Мотор 1: 400 шагов, период 100 мкс (медленно)");
        motor1.go(400, 100);
        break;
        
      case '2':
        Serial.println("Мотор 2: -300 шагов, период 80 мкс (средне)");
        motor2.go(-300, 80);
        break;
        
      case '3':
        Serial.println("Мотор 3: 200 шагов, период 50 мкс (быстро)");
        motor3.go(200, 50);
        break;
        
      case 'a':
        Serial.println("Все моторы одновременно:");
        motor1.go(500, 100);
        motor2.go(-400, 80);
        motor3.go(300, 60);
        break;
        
      case 's':
        Serial.println("Остановка всех моторов");
        motor1.stop();
        motor2.stop();
        motor3.stop();
        break;
        
      case 'p':
        Serial.printf("Позиции: М1=%ld М2=%ld М3=%ld\n",
                     motor1.getCurrentPosition(),
                     motor2.getCurrentPosition(),
                     motor3.getCurrentPosition());
        break;
        
      case 'h':
        Serial.println("Возврат всех моторов домой");
        motor1.home();
        motor2.home();
        motor3.home();
        break;
        
      case 'f':
        Serial.println("=== Тест высоких скоростей ===");
        Serial.println("Мотор 1: 1000 шагов, 30 мкс (33,333 шаг/сек)");
        Serial.println("Мотор 2: 1000 шагов, 25 мкс (40,000 шаг/сек)");
        Serial.println("Мотор 3: 1000 шагов, 20 мкс (50,000 шаг/сек)");
        motor1.go(1000, 30);
        motor2.go(-1000, 25);
        motor3.go(1000, 20);
        break;
        
      case 't':
        Serial.println("=== Тест точности позиционирования ===");
        Serial.println("Движение в точные позиции:");
        motor1.go(100, 100);
        delay(2000);
        motor1.go(200, 100);
        delay(2000);
        motor1.go(300, 100);
        delay(2000);
        motor1.go(0, 100);
        break;
    }
  }
  
  // Демонстрация автоматического движения
  static unsigned long lastDemo = 0;
  if (millis() - lastDemo > 15000) { // Каждые 15 секунд
    if (!motor1.isMotorMoving() && !motor2.isMotorMoving() && 
        !motor3.isMotorMoving()) {
      
      Serial.println("\n=== Автоматическая демонстрация ===");
      motor1.go(200, 100);
      delay(500);
      motor2.go(-150, 80);
      delay(500);
      motor3.go(100, 60);
    }
    lastDemo = millis();
  }
  
  // CPU полностью свободен для других задач!
  // Например, можно читать сенсоры, обрабатывать данные и т.д.
  
  // Демонстрация: мигание встроенным светодиодом
  static unsigned long lastBlink = 0;
  if (millis() - lastBlink > 1000) {
    digitalWrite(2, !digitalRead(2)); // Мигание LED на пине 2
    lastBlink = millis();
  }
}
