#include "StepperMotor.h"
#include "driver/gpio.h"

// Глобальные переменные для управления таймерами
static StepperMotor* motorInstances[4] = {nullptr, nullptr, nullptr, nullptr};
static int motorCount = 0;
static bool timerUsed[2][2] = {{false, false}, {false, false}}; // Отслеживаем использованные таймеры

// Оптимизированный ISR callback для таймеров
bool IRAM_ATTR StepperMotor::timerCallback(void* param) {
    StepperMotor* motor = (StepperMotor*)param;
    
    // Быстрая проверка - объединяем все условия в одно
    if (!motor || !motor->enabled || motor->stepsToGo == 0) {
        return false;
    }
    
    // Переключаем состояние PUL пина (оптимизированно)
    motor->pulseState = !motor->pulseState;
    
    // Быстрое управление GPIO через маску
    if (motor->pulseState) {
        GPIO.out_w1ts = motor->pulPinMask;  // Установить HIGH
    } else {
        GPIO.out_w1tc = motor->pulPinMask;  // Установить LOW
        
        // Уменьшаем счетчик шагов
        motor->stepsToGo--;
        
        // Обновляем позицию (оптимизированно)
        motor->currentPosition += motor->positionIncrement;
        
        // Проверяем завершение движения
        if (motor->stepsToGo == 0) {
            motor->isMoving = false;
            timer_pause(motor->timerGroup, motor->timerIndex);
        }
    }
    
    return false;
}

// Конструктор
StepperMotor::StepperMotor(int ena, int dir, int pul) {
    enaPin = ena;
    dirPin = dir;
    pulPin = pul;
    
    // Инициализация новых полей
    timerInitialized = false;
    currentPosition = 0;
    stepsToGo = 0;
    pulsePeriod = 100;
    isMoving = false;
    direction = true;
    pulseState = false;
    homePosition = 0;
    enabled = false;
    
    // Инициализация оптимизированных полей
    pulPinMask = (1ULL << pulPin);
    positionIncrement = 1;
    
    // Массив мотороов
    if (motorCount < 4) {
        motorInstances[motorCount++] = this;
    }
}

// Инициализация пинов
void StepperMotor::init() {
    pinMode(pulPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enaPin, OUTPUT);
    
    digitalWrite(pulPin, LOW);
    digitalWrite(dirPin, LOW);
    digitalWrite(enaPin, LOW);
    
    // Инициализируем GPIO для прямого управления в ISR
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << pulPin) | (1ULL << dirPin) | (1ULL << enaPin);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
}

// Основной метод движения через таймер
void StepperMotor::go(long steps, int period) {
    if (!timerInitialized || !enabled) {
        return;
    }
    
    if (steps == 0) {
        stop();
        return;
    }
    
    // Устанавливаем направление и оптимизированные поля
    direction = (steps > 0);
    positionIncrement = direction ? 1 : -1;
    digitalWrite(dirPin, direction ? HIGH : LOW);
    
    // Устанавливаем количество шагов (абсолютное значение)
    stepsToGo = abs(steps);
    pulsePeriod = period;
    isMoving = true;
    
    // Настраиваем таймер
    timer_set_divider(timerGroup, timerIndex, 80); // 80MHz / 80 = 1MHz
    timer_set_alarm_value(timerGroup, timerIndex, pulsePeriod); // Период в микросекундах
    timer_set_auto_reload(timerGroup, timerIndex, TIMER_AUTORELOAD_EN);
    
    // Запускаем таймер
    timer_start(timerGroup, timerIndex);
}

// Остановка движения
void StepperMotor::stop() {
    if (timerInitialized) {
        timer_pause(timerGroup, timerIndex);
    }
    isMoving = false;
    stepsToGo = 0;
}

// Проверка движения
bool StepperMotor::isMotorMoving() {
    return isMoving;
}

// Установка текущей позиции
void StepperMotor::setCurrentPosition(long position) {
    currentPosition = position;
}

// Получение текущей позиции
long StepperMotor::getCurrentPosition() {
    return currentPosition;
}

// Установка домашней позиции
void StepperMotor::setHome(long position) {
    homePosition = position;
}

// Возврат домой
void StepperMotor::home() {
    long stepsToHome = homePosition - currentPosition;
    go(stepsToHome, pulsePeriod);
}

// Включить мотор
void StepperMotor::enable() {
    enabled = true;
    digitalWrite(enaPin, LOW); // LOW включает мотор
}

// Выключить мотор
void StepperMotor::disable() {
    enabled = false;
    digitalWrite(enaPin, HIGH); // HIGH выключает мотор
    stop();
}

// Проверить состояние включения
bool StepperMotor::isEnabled() {
    return enabled;
}

// Назначение таймера мотору с автоматической инициализацией
void assignTimerToMotor(StepperMotor* motor, timer_group_t group, timer_idx_t timer) {
    if (motor) {
        // Инициализируем конкретный таймер, если он еще не использовался
        if (!timerUsed[group][timer]) {
            timer_config_t config = {};
            config.divider = 80;
            config.counter_dir = TIMER_COUNT_UP;
            config.counter_en = TIMER_PAUSE;
            config.alarm_en = TIMER_ALARM_EN;
            config.auto_reload = TIMER_AUTORELOAD_EN;
            config.intr_type = TIMER_INTR_LEVEL;
            timer_init(group, timer, &config);
            timerUsed[group][timer] = true;
        }
        
        motor->timerGroup = group;
        motor->timerIndex = timer;
        motor->timerInitialized = true;
        
        // Сallback для этого таймера
        timer_isr_callback_add(group, timer, StepperMotor::timerCallback, motor, 0);
        timer_enable_intr(group, timer);
    }
} 