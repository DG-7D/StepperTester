#include <Arduino.h>

class SimpleStepper {
public:
    enum Direction : uint8_t {
        DIRECTION_STOP,
        DIRECTION_FORWARD,
        DIRECTION_BACKWARD,
    };

private:
    const uint8_t PIN_MOTOR_A1;
    const uint8_t PIN_MOTOR_A2;
    const uint8_t PIN_MOTOR_B1;
    const uint8_t PIN_MOTOR_B2;

    uint16_t interval; // [us]

    unsigned long lastTickTime;
    uint16_t currentStep;
    uint8_t currentPhase;
    Direction currentDirection;

public:
    /// @brief
    /// @param speedStepPerSecond step in full step
    SimpleStepper(uint8_t pinA1, uint8_t pinA2, uint8_t pinB1, uint8_t pinB2, uint16_t speedStepPerSecond = 200)
        : PIN_MOTOR_A1(pinA1),
          PIN_MOTOR_A2(pinA2),
          PIN_MOTOR_B1(pinB1),
          PIN_MOTOR_B2(pinB2) {
        setSpeed(speedStepPerSecond);
        pinMode(PIN_MOTOR_A1, OUTPUT);
        pinMode(PIN_MOTOR_A2, OUTPUT);
        pinMode(PIN_MOTOR_B1, OUTPUT);
        pinMode(PIN_MOTOR_B2, OUTPUT);
        setDirection(DIRECTION_STOP);
    }

    void setSpeed(uint16_t speedStepPerSecond) {
        interval = 1000000 / (speedStepPerSecond * 2);
    }

    void step() {
        if (currentDirection == DIRECTION_FORWARD) {
            currentPhase = (currentPhase + 1) % 8;
        } else {
            currentPhase = (8 + currentPhase - 1) % 8;
        }
        digitalWrite(PIN_MOTOR_A1, 0 <= currentPhase && currentPhase < 3);
        digitalWrite(PIN_MOTOR_B1, 2 <= currentPhase && currentPhase < 5);
        digitalWrite(PIN_MOTOR_A2, 4 <= currentPhase && currentPhase < 7);
        digitalWrite(PIN_MOTOR_B2, 6 <= currentPhase || currentPhase < 1);
    }

    void tick() {
        if (currentDirection == DIRECTION_STOP) {
            lastTickTime = micros();
            return;
        }
        if (micros() - lastTickTime > interval) {
            lastTickTime += interval;
            step();
        }
    }

    void setDirection(Direction direction) {
        if (direction != currentDirection) {
            currentDirection = direction;
        }
    }
};