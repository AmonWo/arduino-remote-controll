#include <Arduino.h>
//Vollgas: 80
//Mitte: 1000
int steeringMotorDistanceInput = A1;
int throttleMotorDistanceInput = A2;
int steeringLeftOut = 9;
int steeringRightOut = 10;
int throttleInput = 5;
int steeringInput = 6;
int reverseOut = 3;
int throttleOut = 11;
int steeringLowerEndpoint = 350;
int steeringUpperEndpoint = 1022;
int throttleLowerEndpoint = 80;
int throttleUpperEndpoint = 1022;
bool autoCorrectSteering = false;

void reverseAccelerate(int throttle);

void accelerate(int throttle);

void steer_left(int throttle);

void steer_right(int throttle);

void check_steering(int steering);

void check_acceleration(int throttle);

void moveToInitialPosition(int motorDistance);

void setup() {
    // put your setup code here, to run once:
    pinMode(steeringInput, INPUT);
    pinMode(throttleInput, INPUT);
    pinMode(steeringMotorDistanceInput, INPUT);
    pinMode(steeringLeftOut, OUTPUT);
    pinMode(steeringRightOut, OUTPUT);
    pinMode(throttleOut, OUTPUT);
    pinMode(reverseOut, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    // Default values for motor endpoint resistance
    // int motorMiddle = 500;
    // Channel 2: Throttle 996 - 1984
    int throttle = pulseIn(throttleInput, HIGH);
    int steering = pulseIn(steeringInput, HIGH);
    int steeringMotorDistance = analogRead(steeringMotorDistanceInput);
    int throttleMotorDistance = analogRead(throttleMotorDistanceInput);
    Serial.print("STEERING_DISTANCE: ");
    Serial.println(steeringMotorDistance);
    Serial.print("ACCELERATION_DISTANCE: ");
    Serial.println(throttleMotorDistance);

    check_acceleration(throttle, throttleMotorDistance);
    check_steering(steering, steeringMotorDistance);

    if (throttle > 1380 && throttle < 1580) {
        //moveToInitialPosition(steeringMotorDistance);
    }
}

void check_acceleration(int throttle, int motorDistance) {
    if (motorDistance < throttleUpperEndpoint && throttle < 1380) {
        accelerate(throttle);
    } else if (motorDistance > throttleLowerEndpoint && throttle > 1580) {
        reverseAccelerate(throttle);
    } else {
        analogWrite(throttleOut, 0);
        analogWrite(reverseOut, 0);
    }
}

void check_steering(int steering, int motorDistance) {
    if (motorDistance < steeringUpperEndpoint && steering < 1380) {
        steer_left(steering);
    } else if (motorDistance > steeringLowerEndpoint && steering > 1580) {
        steer_right(steering);
    } else {
        analogWrite(steeringLeftOut, 0);
        analogWrite(steeringRightOut, 0);
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

}// H-Bridge: RPWM
void steer_right(int throttle) {
    Serial.print("STEER_RIGHT: ");
    Serial.println(throttle);
    int forwardPWM = map(throttle, 1580, 1990, 0, 255);
    analogWrite(reverseOut, 0);
    analogWrite(steeringRightOut, forwardPWM);
    autoCorrectSteering = true;
}

// H-Bridge: LPWM
void steer_left(int throttle) {
    Serial.print("STEER LEFT: ");
    Serial.println(throttle);
    int backwardPWM = map(throttle, 990, 1380, 255, 0);
    analogWrite(throttleOut, 0);
    analogWrite(steeringLeftOut, backwardPWM);
    autoCorrectSteering = true;
}

void moveToInitialPosition(int motorDistance) {
    if (motorDistance > 760 && motorDistance < 780) {
        analogWrite(steeringLeftOut, 0);
        analogWrite(steeringRightOut, 0);
        autoCorrectSteering = false;
    } else if (motorDistance > 770 && autoCorrectSteering) {
        steer_right(1020);
    } else if (motorDistance < 770 && autoCorrectSteering) {
        steer_left(1900);
    } else {
        analogWrite(steeringLeftOut, 0);
        analogWrite(steeringRightOut, 0);
    }
}