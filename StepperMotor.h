#ifndef STEPPERMOTOR_H
#define STEPPERMOTOR_H

#include <Arduino.h>
#include "driver/timer.h"

class StepperMotor {
private:
    // Поля пинов
    int enaPin;          // Пин ENA+
    int dirPin;          // Пин DIR+
    int pulPin;          // Пин PUL+
    
    // Таймер для этого мотора
    timer_group_t timerGroup;
    timer_idx_t timerIndex;
    bool timerInitialized;
    
    // Параметры движения
    long currentPosition;    // Текущая позиция
    unsigned long stepsToGo; // Оставшиеся шаги
    int pulsePeriod;        // Период импульса в микросекундах
    bool isMoving;          // Флаг движения
    bool direction;         // Направление (true - вправо, false - влево)
    
    // Состояние пина PUL
    bool pulseState;        // Текущее состояние PUL пина
    
    // Домашняя позиция
    long homePosition;      // Позиция "дома"
    
    // Состояние включения
    bool enabled;           // Включен/выключен мотор
    
    // Оптимизированные поля для ISR
    uint32_t pulPinMask;    // Маска для быстрого управления PUL пином
    int positionIncrement;  // Инкремент позиции (+1 или -1)
    
    // ISR callback для таймера
    static bool IRAM_ATTR timerCallback(void* param);
    
public:
    // Конструктор
    StepperMotor(int ena, int dir, int pul);
    
    // Инициализация
    void init();
    
    // Основные методы
    void go(long steps, int period);        // Движение: шаги и период
    void stop();                            // Остановка движения
    bool isMotorMoving();                   // Проверка движения
    
    // Методы для работы с позицией
    void setCurrentPosition(long position); // Установка текущей позиции
    long getCurrentPosition();              // Получение текущей позиции
    void setHome(long position);            // Установка домашней позиции
    void home();                            // Возврат домой
    
    // Управление включением/выключением
    void enable();                          // Включить мотор
    void disable();                         // Выключить мотор
    bool isEnabled();                       // Проверить состояние
};

// Глобальная функция для назначения таймера мотору
void assignTimerToMotor(StepperMotor* motor, timer_group_t group, timer_idx_t timer);

#endif 