#include <Arduino.h>

bool autoCorrectSteering = false;
bool autoCorrectThrottle = false;

char steeringMotorSensorInput = A2;
char throttleMotorSensorInput = A1;
int throttleInput = 6;
int steeringInput = 5;
int steeringLeftOutput = 10;
int steeringRightOutput = 9;
int reverseOutput = 11;
int accelerationOutput = 3;

// Constants for motor endpoints
const int THROTTLE_LOWER_ENDPOINT = 300;
const int THROTTLE_UPPER_ENDPOINT = 1000;

const int STEERING_LOWER_ENDPOINT = 550;
const int STEERING_UPPER_ENDPOINT = 940;

// Constants for remote control inputs THROTTLE
const int REMOTE_INPUT_BACKWARD = 1380;
const int REMOTE_INPUT_FORWARD = 1580;
const int MIN_REMOTE_INPUT_THROTTLE = 950;
const int MAX_REMOTE_INPUT_THROTTLE = 1900;

// Constants for remote control inputs STEERING
const int REMOTE_INPUT_LEFT = 1380;
const int REMOTE_INPUT_RIGHT = 1580;
const int MIN_REMOTE_INPUT_STEERING_RIGHT = 1500;
const int MAX_REMOTE_INPUT_STEERING_RIGHT = 2000;
const int MIN_REMOTE_INPUT_STEERING_LEFT = 1500;
const int MAX_REMOTE_INPUT_STEERING_LEFT = 900;

// Constants for the range for steering motor reset
const int STEERING_DEADLOCK_MIN = 790;
const int STEERING_DEADLOCK_MAX = 800;

// Constants for the range for throttle motor reset
const int THROTTLE_DEADLOCK_MIN = 580;
const int THROTTLE_DEADLOCK_MAX = 600;

void setup() {
    // put your setup code here, to run once:
    pinMode(steeringInput, INPUT);
    pinMode(steeringMotorSensorInput, INPUT);
    pinMode(throttleInput, INPUT);
    pinMode(throttleMotorSensorInput, INPUT);
    pinMode(steeringLeftOutput, OUTPUT);
    pinMode(steeringRightOutput, OUTPUT);
    pinMode(accelerationOutput, OUTPUT);
    pinMode(reverseOutput, OUTPUT);
    Serial.begin(57600);
}


// H-Bridge: RPWM
void accelerate(int throttle) {
    Serial.print("ACCELERATE: ");
    Serial.println(throttle);
    int forwardPWM = map(throttle, REMOTE_INPUT_FORWARD, MAX_REMOTE_INPUT_THROTTLE, 0, 255);
    analogWrite(reverseOutput, 0);
    analogWrite(accelerationOutput, forwardPWM);
}

// H-Bridge: LPWM
void reverseAccelerate(int throttle) {
    Serial.print("REVERSE ACCELERATE: ");
    Serial.println(throttle);
    int backwardPWM = map(throttle, MIN_REMOTE_INPUT_THROTTLE, REMOTE_INPUT_BACKWARD, 255, 0);
    analogWrite(accelerationOutput, 0);
    analogWrite(reverseOutput, backwardPWM);
}

// H-Bridge: RPWM
void steer_right(int steering) {
    int rightPWM = map(steering, MIN_REMOTE_INPUT_STEERING_RIGHT, MAX_REMOTE_INPUT_STEERING_RIGHT, 0, 255);
    Serial.print("STEER_RIGHT: ");
    Serial.println(steering);
    Serial.print("RIGHT_PWM: ");
    Serial.println(rightPWM);
    analogWrite(steeringLeftOutput, 0);
    analogWrite(steeringRightOutput, rightPWM);
}

// H-Bridge: LPWM
void steer_left(int steering) {
    int leftPWM = map(steering, MAX_REMOTE_INPUT_STEERING_LEFT, MIN_REMOTE_INPUT_STEERING_LEFT, 255, 0);
    Serial.print("STEER LEFT: ");
    Serial.println(steering);
    Serial.print("LEFT_PWM: ");
    Serial.println(leftPWM);
    analogWrite(steeringRightOutput, 0);
    analogWrite(steeringLeftOutput, leftPWM);
}

void resetSteering(int steeringMotorSensor) {
    if (steeringMotorSensor > STEERING_DEADLOCK_MIN && steeringMotorSensor < STEERING_DEADLOCK_MAX) {
        analogWrite(steeringLeftOutput, 0);
        analogWrite(steeringRightOutput, 0);
        autoCorrectSteering = false;
    } else if (steeringMotorSensor < STEERING_DEADLOCK_MAX) {
        steer_right(MAX_REMOTE_INPUT_STEERING_RIGHT);
    } else if (steeringMotorSensor > STEERING_DEADLOCK_MIN) {
        steer_left(MAX_REMOTE_INPUT_STEERING_LEFT);
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

void loop() {
    // int motorMiddle = 500;
    // Channel 2: Throttle 996 - 1984
    int throttle = pulseIn(throttleInput, HIGH);
    int throttleMotorSensor = analogRead(throttleMotorSensorInput);
    int steering = pulseIn(steeringInput, HIGH);
    int steeringMotorSensor = analogRead(steeringMotorSensorInput);

    Serial.print("steeringSensor: ");
    Serial.println(steeringMotorSensor);
    Serial.print("throttleSensor: ");
    Serial.println(throttleMotorSensor);
    Serial.print("steeringInput: ");
    Serial.println(steering);
    Serial.print("throttleInput: ");
    Serial.println(throttle);
    if (throttle != 0) {
        if (throttleMotorSensor > THROTTLE_LOWER_ENDPOINT && throttle < REMOTE_INPUT_BACKWARD) {
            reverseAccelerate(throttle);
            autoCorrectThrottle = true;
        } else if (throttleMotorSensor < THROTTLE_UPPER_ENDPOINT && throttle > REMOTE_INPUT_FORWARD) {
            accelerate(throttle);
            autoCorrectThrottle = true;
        } else {
            analogWrite(accelerationOutput, 0);
            analogWrite(reverseOutput, 0);
        }
    } else {
        resetThrottle(throttleMotorSensor);
    }

    if (steering != 0) {
        if (steeringMotorSensor < STEERING_UPPER_ENDPOINT && steering > REMOTE_INPUT_RIGHT) {
            steer_right(steering);
            autoCorrectSteering = true;
        } else if (steeringMotorSensor > STEERING_LOWER_ENDPOINT && steering < REMOTE_INPUT_LEFT) {
            steer_left(steering);
            autoCorrectSteering = true;
        } else {
            analogWrite(steeringLeftOutput, 0);
            analogWrite(steeringRightOutput, 0);
        }
    }

    if (steering > REMOTE_INPUT_LEFT && steering < REMOTE_INPUT_RIGHT && autoCorrectSteering) {
        resetSteering(steeringMotorSensor);
    }
    if (throttle > REMOTE_INPUT_BACKWARD && throttle < REMOTE_INPUT_FORWARD && autoCorrectThrottle) {
        resetThrottle(throttleMotorSensor);
    }
}

