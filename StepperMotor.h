#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <Arduino.h>

class StepperMotor {
public:
    // Поля пинов
    int enaPin;          // Пин ENA+
    int dirPin;          // Пин DIR+
    int pulPin;          // Пин PUL+
    
    // Поля состояния
    bool enable;         // Включен/выключен
    bool direction;      // Направление (true - вправо, false - влево)
    long pose;           // Текущая позиция
    long target;         // Целевая позиция
    int setStepTime;     // Время шага в микросекундах
    int minStepTime;     // Минимальное время шага в микросекундах
    long homePosition;   // Позиция "дома" (нулевая позиция)
    
    // Поля для неблокирующей работы
    bool isMoving;       // Двигается ли мотор
    long stepsToGo;      // Оставшиеся шаги
    unsigned long lastStepTime; // Время последнего шага
    bool stepStartedFlag; // Флаг начала шага
    unsigned long stepStartTime; // Время начала текущего шага
    
    // Конструктор с пинами
    StepperMotor(int ena, int dir, int pul);
    
    // Методы
    void init();
    void setOneStepTime(int time);
    void setPose(long position);
    void setCurrentPosition(long position);  // Новый метод для установки текущей позиции
    long getCurrentPosition();  // Переименованный метод получения текущей позиции
    void update();
    void stop();
    void home();
    void setHome(long position);
};

#endif 