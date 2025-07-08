#include <HardwareSerial.h>
#include <ESP32Servo.h>

HardwareSerial loraSerial(1);
Servo servo1, servo2;

const int IN1 = 33;
const int IN2 = 32;
const int PWM_PIN = 25;  // 🆕 Chân PWM điều khiển tốc độ
const int servo1Pin = 18;
const int servo2Pin = 22; 

void setup() {
    Serial.begin(9600);
    loraSerial.begin(9600, SERIAL_8N1, 16, 17);

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(PWM_PIN, OUTPUT); // 🆕 Khai báo chân PWM

    servo1.setPeriodHertz(50);
    servo2.setPeriodHertz(50);

    servo1.attach(servo1Pin, 500, 2400);
    servo2.attach(servo2Pin, 500, 2400);

    Serial.println("ESP32 đã khởi động!");
}

void setMotorSpeed(int speed) {
    speed = constrain(speed, 0, 155); // Giới hạn tốc độ từ 0 - 255
    analogWrite(PWM_PIN, speed);
}

void loop() {
    if (loraSerial.available()) {
        int receivedData = loraSerial.read();

        Serial.print("Nhận dữ liệu: ");
        Serial.println(receivedData);

        if (receivedData < 10) { // Điều khiển động cơ
            switch (receivedData) {
                case 1:
                    digitalWrite(IN1, HIGH);
                    digitalWrite(IN2, LOW);
                    Serial.println("Quay trái");
                    break;
                case 2:
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, HIGH);
                    Serial.println("Quay phải");
                    break;
                case 0:
                    digitalWrite(IN1, LOW);
                    digitalWrite(IN2, LOW);
                    Serial.println("Dừng");
                    break;
            }
        } else if (receivedData == 100) { // Servo 1 về 0°
            servo1.write(0);
            Serial.println("Servo 1 về 0°");
        } else if (receivedData == 101) { // Servo 1 đến 30°
            servo1.write(30);
            Serial.println("Servo 1 đến 30°");
        } else if (receivedData == 102) { // Servo 1 đến 57°
            servo1.write(57);
            Serial.println("Servo 1 đến 57°");
        } else if (receivedData == 200) { // Servo 2 mở ra 180°
            servo2.write(130);
            Serial.println("Servo 2 mở ra 180°");
        } else if (receivedData == 201) { // Servo 2 về 0°
            servo2.write(0);
            Serial.println("Servo 2 về 0°");
        } else if (receivedData >= 10 && receivedData <= 155) { // Nhận dữ liệu tốc độ
            setMotorSpeed(receivedData); 
            Serial.print("Tốc độ động cơ: ");
            Serial.println(receivedData);
        }
    }
}
