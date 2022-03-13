#include <Arduino.h>

int throttleMotorDistanceInput = A0;
int throttleInput = 5;
int steeringInput = 6;
int reverseOut = 9;
int throttleOut = 10;


void reverseAccelerate(int throttle);

void accelerate(int throttle);

void moveToInitialPosition(int motorDistance);

void setup() {
    // put your setup code here, to run once:
    pinMode(steeringInput, INPUT);
    pinMode(throttleInput, INPUT);
    pinMode(throttleMotorDistanceInput, INPUT);
    pinMode(throttleOut, OUTPUT);
    pinMode(reverseOut, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    // Default values for motor endpoint resistance
    int lowerEndpoint = 50;
    int upperEndpoint = 1000;
    // int motorMiddle = 500;
    // Channel 2: Throttle 996 - 1984
    int throttle = pulseIn(throttleInput, HIGH);
    int motorDistance = analogRead(throttleMotorDistanceInput);
    Serial.print("DISTANCE: ");
    Serial.println(motorDistance);

    if (motorDistance > lowerEndpoint && throttle < 1380) {
        reverseAccelerate(throttle);
    } else if (motorDistance < upperEndpoint && throttle > 1580) {
        accelerate(throttle);
    } else {
        analogWrite(throttleOut, 0);
        analogWrite(reverseOut, 0);
    }

    if (throttle > 1380 && throttle < 1580) {
        moveToInitialPosition(motorDistance);
    }
}

// H-Bridge: RPWM
void accelerate(int throttle) {
    Serial.print("ACCELERATE: ");
    Serial.println(throttle);
    int forwardPWM = map(throttle, 1580, 1990, 0, 255);
    analogWrite(reverseOut, 0);
    analogWrite(throttleOut, forwardPWM);
}

// H-Bridge: LPWM
void reverseAccelerate(int throttle) {
    Serial.print("REVERSE ACCELERATE: ");
    Serial.println(throttle);
    int backwardPWM = map(throttle, 990, 1380, 255, 0);
    analogWrite(throttleOut, 0);
    analogWrite(reverseOut, backwardPWM);
}

void moveToInitialPosition(int motorDistance) {
    if (motorDistance > 450 && motorDistance < 550) {
        analogWrite(throttleOut, 0);
        analogWrite(reverseOut, 0);
    } else if (motorDistance > 500) {
        reverseAccelerate(990);
    } else if (motorDistance < 500) {
        accelerate(1900);
    } else {
        analogWrite(throttleOut, 0);
        analogWrite(reverseOut, 0);
    }
}