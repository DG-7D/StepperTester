#include "SimpleStepper.cpp"
#include <Arduino.h>

// モータ設定
constexpr float DEGREE_PER_STEP = 1.8;

// ピン設定
constexpr uint8_t PIN_OUT_EN = 7;
constexpr uint8_t PIN_OUT_A1 = 8;
constexpr uint8_t PIN_OUT_A2 = 9;
constexpr uint8_t PIN_OUT_B1 = 11;
constexpr uint8_t PIN_OUT_B2 = 12;

//
float currentRps = 0;

SimpleStepper stepper(PIN_OUT_A1, PIN_OUT_A2, PIN_OUT_B1, PIN_OUT_B2);

uint16_t rpsToStepPerSecond(float rps) {
    return rps * 360 / DEGREE_PER_STEP;
}

void onSpeedChange() {
    if (currentRps == 0) {
        digitalWrite(PIN_OUT_EN, LOW);
        stepper.setDirection(SimpleStepper::DIRECTION_STOP);
    } else {
        if (currentRps > 0) {
            stepper.setDirection(SimpleStepper::DIRECTION_FORWARD);
        } else {
            stepper.setDirection(SimpleStepper::DIRECTION_BACKWARD);
        }
        digitalWrite(PIN_OUT_EN, HIGH);
    }
    stepper.setSpeed(rpsToStepPerSecond(abs(currentRps)));
    Serial.print("RPS: ");
    Serial.println(currentRps);
}

void setup() {
    Serial.begin(9600);
    Serial.println("Start");

    pinMode(PIN_OUT_EN, OUTPUT);
    digitalWrite(PIN_OUT_EN, LOW);
}

void loop() {
    while (Serial.available()) {
        int c = Serial.read();
        switch (c) {
        case 'j':
            currentRps -= 0.1;
            onSpeedChange();
            break;
        case 'k':
            currentRps += 0.1;
            onSpeedChange();
            break;
        case 'h':
            currentRps -= 1.0;
            onSpeedChange();
            break;
        case 'l':
            currentRps += 1.0;
            onSpeedChange();
            break;
        case ' ':
            currentRps = 0;
            onSpeedChange();
            break;
        }
    }
    stepper.tick();
}
