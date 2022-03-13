#include <Arduino.h>

bool autoCorrectSteering = false;
bool autoCorrectThrottle = false;

int steeringMotorSensorInput = A0;
int throttleMotorSensorInput = A1;
int throttleInput = 6;
int steeringInput = 5;
int steeringLeftOutput = 9;
int steeringRightOutput = 10;
int reverseOutput = 3;
int accelerationOutput = 11;

// Constants for motor endpoints
const int THROTTLE_LOWER_ENDPOINT = 50;
const int THROTTLE_UPPER_ENDPOINT = 1022;

const int STEERING_LOWER_ENDPOINT = 350;
const int STEERING_UPPER_ENDPOINT = 1022;

// Constants for remote control inputs THROTTLE
const int REMOTE_INPUT_BACKWARD = 1380;
const int REMOTE_INPUT_FORWARD = 1580;
const int MIN_REMOTE_INPUT_THROTTLE = 950;
const int MAX_REMOTE_INPUT_THROTTLE = 1900;

// Constants for remote control inputs STEERING
const int REMOTE_INPUT_LEFT = 1380;
const int REMOTE_INPUT_RIGHT = 1580;
const int MIN_REMOTE_INPUT_STEERING = 950;
const int MAX_REMOTE_INPUT_STEERING = 1900;

// Constants for the range for steering motor reset
const int STEERING_DEADLOCK_MIN = 450;
const int STEERING_DEADLOCK_MAX = 550;

// Constants for the range for throttle motor reset
const int THROTTLE_DEADLOCK_MIN = 450;
const int THROTTLE_DEADLOCK_MAX = 550;

void setup() {
    // put your setup code here, to run once:
    pinMode(steeringInput, INPUT);
    pinMode(throttleInput, INPUT);
    pinMode(steeringMotorSensorInput, INPUT);
    pinMode(throttleMotorSensorInput, INPUT);
    pinMode(steeringLeftOutput, OUTPUT);
    pinMode(steeringRightOutput, OUTPUT);
    pinMode(accelerationOutput, OUTPUT);
    pinMode(reverseOutput, OUTPUT);
    Serial.begin(57600);
}

void loop() {
    // int motorMiddle = 500;
    // Channel 2: Throttle 996 - 1984
    int throttle = pulseIn(throttleInput, HIGH);
    int steering = pulseIn(steeringInput, HIGH);
    int throttleMotorSensor = analogRead(throttleMotorSensorInput);
    int steeringMotorSensor = analogRead(steeringMotorSensorInput);

    Serial.print("steeringSensor: ");
    Serial.println(steeringMotorSensor);
    Serial.print("throttleSensor: ");
    Serial.println(throttleMotorSensor);

    if (throttleMotorSensor < THROTTLE_UPPER_ENDPOINT && throttle < REMOTE_INPUT_BACKWARD) {
        reverseAccelerate(throttle);
    } else if (throttleMotorSensor > THROTTLE_LOWER_ENDPOINT && throttle > REMOTE_INPUT_FORWARD) {
        accelerate(throttle);
    } else {
        analogWrite(accelerationOutput, 0);
        analogWrite(reverseOutput, 0);
    }

    if (steeringMotorSensor < STEERING_UPPER_ENDPOINT && steering < REMOTE_INPUT_LEFT) {
        steer_left(steering);
    } else if (steeringMotorSensor > STEERING_LOWER_ENDPOINT && steering > REMOTE_INPUT_RIGHT) {
        steer_right(steering);
    } else {
        analogWrite(steeringLeftOutput, 0);
        analogWrite(steeringRightOutput, 0);
    }

    if (steering > REMOTE_INPUT_LEFT && steering < REMOTE_INPUT_RIGHT) {
        //resetSteering(steeringMotorSensor);
    }
    if (throttle > REMOTE_INPUT_BACKWARD && throttle < REMOTE_INPUT_FORWARD) {
        //resetThrottle(throttleMotorSensor);
    }
}



// H-Bridge: RPWM
void accelerate(int throttle) {
    Serial.print("ACCELERATE: ");
    Serial.println(throttle);
    int forwardPWM = map(throttle, REMOTE_INPUT_FORWARD, MAX_REMOTE_INPUT_THROTTLE, 0, 255);
    analogWrite(reverseOutput, 0);
    analogWrite(accelerationOutput, forwardPWM);
    autoCorrectThrottle = true;
}

// H-Bridge: LPWM
void reverseAccelerate(int throttle) {
    Serial.print("REVERSE ACCELERATE: ");
    Serial.println(throttle);
    int backwardPWM = map(throttle, MIN_REMOTE_INPUT_THROTTLE, REMOTE_INPUT_BACKWARD, 255, 0);
    analogWrite(accelerationOutput, 0);
    analogWrite(reverseOutput, backwardPWM);
    autoCorrectThrottle = true;
}

// H-Bridge: RPWM
void steer_right(int steering) {
    Serial.print("STEER_RIGHT: ");
    Serial.println(steering);
    int forwardPWM = map(steering, REMOTE_INPUT_RIGHT, MAX_REMOTE_INPUT_STEERING, 0, 255);
    analogWrite(steeringLeftOutput, 0);
    analogWrite(steeringRightOutput, forwardPWM);
    autoCorrectSteering = true;
}

// H-Bridge: LPWM
void steer_left(int steering) {
    Serial.print("STEER LEFT: ");
    Serial.println(steering);
    int backwardPWM = map(steering, MIN_REMOTE_INPUT_STEERING, REMOTE_INPUT_LEFT, 255, 0);
    analogWrite(steeringRightOutput, 0);
    analogWrite(steeringLeftOutput, backwardPWM);
    autoCorrectSteering = true;
}

void resetSteering(int steeringMotorSensor) {
    if (steeringMotorSensor > STEERING_DEADLOCK_MIN && steeringMotorSensor < STEERING_DEADLOCK_MAX) {
        analogWrite(steeringLeftOutput, 0);
        analogWrite(steeringRightOutput, 0);
        autoCorrectSteering = false;
    } else if (steeringMotorSensor > STEERING_DEADLOCK_MAX) {
        steer_right(MIN_REMOTE_INPUT_STEERING);
    } else if (steeringMotorSensor < STEERING_DEADLOCK_MIN) {
        steer_left(MAX_REMOTE_INPUT_STEERING);
    } else {
        analogWrite(steeringRightOutput, 0);
        analogWrite(steeringLeftOutput, 0);
    }
}
void resetThrottle(int throttleMotorSensor) {
    if (throttleMotorSensor > THROTTLE_DEADLOCK_MIN && throttleMotorSensor < THROTTLE_DEADLOCK_MAX) {
        analogWrite(accelerationOutput, 0);
        analogWrite(reverseOutput, 0);
        autoCorrectThrottle = false;
    } else if (throttleMotorSensor > THROTTLE_DEADLOCK_MAX) {
        reverseAccelerate(MIN_REMOTE_INPUT_THROTTLE);
    } else if (throttleMotorSensor < THROTTLE_DEADLOCK_MIN) {
        accelerate(MAX_REMOTE_INPUT_THROTTLE);
    } else {
        analogWrite(accelerationOutput, 0);
        analogWrite(reverseOutput, 0);
    }
}