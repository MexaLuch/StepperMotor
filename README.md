# StepperMotor Library

Библиотека для **параллельного управления несколькими шаговыми двигателями** через драйвер DM556 без блокировки основного цикла.

## 🚀 Основные возможности

- **Параллельное управление** - одновременно управляйте любым количеством моторов
- **Неблокирующая работа** - моторы работают в фоне, не мешая основной программе
- **Позиционное управление** - точное позиционирование с отслеживанием позиции
- **Настройка скорости** - регулируйте скорость каждого мотора независимо
- **Простота использования** - минимум кода для работы

## 📋 Подключение

### Схема подключения к ESP32:
```
ESP32          DM556
D12 -------- ENA+    (включение драйвера)
D14 -------- DIR+    (направление)
D27 -------- PUL+    (импульсы)
GND -------- ENA-
GND -------- DIR-
GND -------- PUL-
```

## ⚡ Быстрый старт

### Один мотор
```cpp
#include "StepperMotor.h"

StepperMotor motor(12, 14, 27);  // ENA=12, DIR=14, PUL=27

void setup() {
    motor.init();
    motor.enable = true;
}

void loop() {
    motor.update();  // ОБЯЗАТЕЛЬНО!
    
    // Ваш код здесь...
    motor.setPose(500);  // Движение на позицию 500
}
```

### Несколько моторов
```cpp
#include "StepperMotor.h"

// Создаем несколько моторов
StepperMotor motor1(12, 14, 27);
StepperMotor motor2(12, 26, 25);
StepperMotor motor3(12, 33, 32);

void setup() {
    // Инициализируем все моторы
    motor1.init(); motor1.enable = true;
    motor2.init(); motor2.enable = true;
    motor3.init(); motor3.enable = true;
}

void loop() {
    // Обновляем ВСЕ моторы
    motor1.update();
    motor2.update();
    motor3.update();
    
    // Каждый мотор может двигаться независимо
    motor1.setPose(200);
    motor2.setPose(-150);
    motor3.setPose(300);
}
```

## 📖 Основные методы

### `setPose(long position)`
Установка целевой позиции (неблокирующий)
```cpp
motor.setPose(500);   // Движение на позицию 500
motor.setPose(-200);  // Движение на позицию -200
```

### `setOneStepTime(int time)`
Настройка скорости (время между шагами в микросекундах)
```cpp
motor.setOneStepTime(50);   // Быстро (50 мкс)
motor.setOneStepTime(100);  // Медленно (100 мкс)
```

### `getCurrentPosition()`
Получение текущей позиции
```cpp
long pos = motor.getCurrentPosition();
```

### `stop()`
Экстренная остановка
```cpp
motor.stop();  // Останавливает движение
```

### `home()`
Возврат в домашнюю позицию
```cpp
motor.setHome(0);  // Установить домашнюю позицию
motor.home();      // Вернуться домой
```

## 🔧 Примеры использования

### Параллельное управление тремя моторами
```cpp
#include "StepperMotor.h"

StepperMotor motor1(12, 14, 27);
StepperMotor motor2(12, 26, 25);
StepperMotor motor3(12, 33, 32);

void setup() {
    motor1.init(); motor1.enable = true;
    motor2.init(); motor2.enable = true;
    motor3.init(); motor3.enable = true;
}

void loop() {
    motor1.update();
    motor2.update();
    motor3.update();
    
    // Мотор 1: каждые 2 секунды
    static unsigned long last1 = 0;
    if (millis() - last1 > 2000) {
        motor1.setPose(200);
        last1 = millis();
    }
    
    // Мотор 2: каждую секунду
    static unsigned long last2 = 0;
    if (millis() - last2 > 1000) {
        motor2.setPose(-150);
        last2 = millis();
    }
    
    // Мотор 3: каждые 500мс
    static unsigned long last3 = 0;
    if (millis() - last3 > 500) {
        motor3.setPose(100);
        last3 = millis();
    }
}
```

### Управление скоростью
```cpp
void loop() {
    motor.update();
    
    // Быстрое движение
    motor.setOneStepTime(40);
    motor.setPose(100);
    
    delay(1000);
    
    // Медленное движение
    motor.setOneStepTime(100);
    motor.setPose(-100);
}
```

## ⚠️ Важные моменты

1. **ОБЯЗАТЕЛЬНО вызывайте `update()`** для каждого мотора в `loop()`
2. **Минимальное время шага** = 40 микросекунд
3. **Общий GND** между ESP32 и драйвером обязателен
4. **Позиция отслеживается автоматически** - не нужно считать шаги

## 📁 Структура библиотеки

```
StepperMotor/
├── StepperMotor.h          # Заголовочный файл
├── StepperMotor.cpp        # Реализация
├── library.properties      # Метаданные для Arduino IDE
├── README.md              # Этот файл
└── examples/
    ├── SingleMotor/       # Пример с одним мотором
    └── MultipleMotors/    # Пример с несколькими моторами
```

## 🎯 Преимущества

- ✅ **Параллельная работа** - управляйте любым количеством моторов
- ✅ **Неблокирующий** - не останавливает основную программу
- ✅ **Простой API** - минимум кода для работы
- ✅ **Точное позиционирование** - знайте где находится каждый мотор
- ✅ **Гибкая настройка скорости** - разная скорость для каждого мотора

## 🔌 Совместимость

- **Платформа**: ESP32
- **Драйвер**: DM556 (и совместимые)
- **Arduino IDE**: Да
- **PlatformIO**: Да

## 📄 Лицензия

MIT License 