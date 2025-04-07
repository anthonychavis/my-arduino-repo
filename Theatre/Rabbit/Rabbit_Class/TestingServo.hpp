#ifndef TESTING_SERVO
#define TESTING_SERVO

// serial print servoObj position in microseconds
inline
void printServoPos(Servo servoObj) {
    int microsec = servoObj.readMicroseconds();
    int ang = servoObj.read();
    Serial.println("---");
    Serial.print("Servo position in microseconds: "); Serial.println(microsec);
    Serial.print("Servo position in degrees: "); Serial.println(ang);
    delay(2000);
};
// don't use while main program is active; serial print servoObj position in microseconds at specified angle
inline
void printServoPos(Servo servoObj, int angle) {
    if(angle > 180 || angle < 0) {
        Serial.print("adhere to 0 <= angle <= 180; you entered: "); Serial.println(angle);
        delay(2000);
        return;
    }
    servoObj.write(angle);
    int microsec = servoObj.readMicroseconds();
    int ang = servoObj.read();
    Serial.println("---");
    Serial.print("Servo position in microseconds: "); Serial.println(microsec);
    Serial.print("Servo position in degrees: "); Serial.println(ang);
    delay(2000);
};

#endif
