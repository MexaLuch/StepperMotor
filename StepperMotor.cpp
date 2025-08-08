#include "StepperMotor.h"

// Конструктор с пинами
StepperMotor::StepperMotor(int ena, int dir, int pul) {
    enaPin = ena;
    dirPin = dir;
    pulPin = pul;
    enable = false;
    direction = true;
    pose = 0;
    target = 0;
    setStepTime = 70;  // По умолчанию 70 мкс
    minStepTime = 40;  // Минимум 40 мкс
    homePosition = 0;  // По умолчанию дом = 0
    isMoving = false;
    stepsToGo = 0;
    lastStepTime = 0;
    stepStartedFlag = false;
    stepStartTime = 0;
}

// Метод инициализации
void StepperMotor::init() {
    pinMode(pulPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enaPin, OUTPUT);
    
    digitalWrite(pulPin, LOW);
    digitalWrite(dirPin, LOW);
    digitalWrite(enaPin, LOW);
}

// Метод установки времени шага
void StepperMotor::setOneStepTime(int time) {
    if (time < minStepTime) {
        setStepTime = minStepTime;  // Минимум из поля
    } else {
        setStepTime = time;
    }
}

// Метод установки позиции (неблокирующий)
void StepperMotor::setPose(long position) {
    target = position;
    long steps = position - pose;
    
    if (steps != 0) {
        direction = (steps > 0);
        stepsToGo = abs(steps);
        isMoving = true;
        lastStepTime = 0;
        
        digitalWrite(dirPin, direction ? HIGH : LOW);
    } else {
        isMoving = false;
        stepsToGo = 0;
    }
}

// Метод остановки
void StepperMotor::stop() {
    isMoving = false;
    stepsToGo = 0;
    target = pose;  // Целевая позиция = текущая позиция
}

// Метод возврата домой
void StepperMotor::home() {
    setPose(homePosition);
}

// Метод установки позиции "дома"
void StepperMotor::setHome(long position) {
    homePosition = position;
}

// Метод установки текущей позиции (без движения)
void StepperMotor::setCurrentPosition(long position) {
    pose = position;
    target = position;  // Целевая позиция = текущая позиция
    isMoving = false;   // Останавливаем движение
    stepsToGo = 0;      // Обнуляем счетчик шагов
}

// Метод обновления (вызывается в loop)
void StepperMotor::update() {
    if (isMoving && stepsToGo > 0) {
        unsigned long currentTime = micros();
        
        if (currentTime - lastStepTime >= setStepTime) {
            // Начинаем новый шаг
            if (!stepStartedFlag) {
                digitalWrite(pulPin, LOW);
                stepStartTime = currentTime;
                stepStartedFlag = true;
            }
            // Завершаем шаг через setStepTime микросекунд
            else if (currentTime - stepStartTime >= setStepTime) {
                digitalWrite(pulPin, HIGH);
                
                // Инкремент позиции
                if (direction) {
                    pose++;
                } else {
                    pose--;
                }
                
                stepsToGo--;
                lastStepTime = currentTime;
                stepStartedFlag = false;
                
                // Проверяем завершение движения
                if (stepsToGo == 0) {
                    isMoving = false;
                }
            }
        }
    }
}

// Метод получения текущей позиции
long StepperMotor::getCurrentPosition() {
    return pose;
} 