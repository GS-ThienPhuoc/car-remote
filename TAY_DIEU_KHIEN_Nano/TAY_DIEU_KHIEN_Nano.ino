#include <SoftwareSerial.h>

SoftwareSerial loraSerial(2, 3);  // RX=2, TX=3 cho LoRa

const int joystickX = A1;  // Trục X joystick (điều khiển động cơ)
const int joystickY = A2;  // Trục Y joystick (điều khiển servo 1)
const int buttonPin = A0;   // 🆕 Nút nhấn điều khiển servo 2

int command = 0;
bool buttonState = false;
bool lastButtonState = false;

void setup() {
    Serial.begin(9600);
    loraSerial.begin(9600);
    pinMode(joystickX, INPUT);
    pinMode(joystickY, INPUT);
    pinMode(buttonPin, INPUT_PULLUP);  // 🆕 Kéo lên để tránh nhiễu

    Serial.println("Arduino Nano đã khởi động!");
}

void loop() {
    int xValue = analogRead(joystickX);
    int yValue = analogRead(joystickY);
    bool currentButtonState = digitalRead(buttonPin) == LOW; // 🆕 Đọc trạng thái nút nhấn

    // Điều khiển động cơ từ joystick X
    if (xValue < 400) {
        command = 1;  // Quay trái
    } else if (xValue > 600) {
        command = 2;  // Quay phải
    } else {
        command = 0;  // Dừng
    }
    loraSerial.write(command);
    Serial.print("Gửi lệnh động cơ: ");
    Serial.println(command);
    delay(100);

    // Điều khiển servo 1 từ joystick Y
    if (yValue < 300) {
        loraSerial.write(100);
        Serial.println("Gửi lệnh servo 1: 0°");
    } else if (yValue > 500 && yValue < 700) {
        loraSerial.write(101);
        Serial.println("Gửi lệnh servo 1: 30°");
    } else if (yValue > 800) {
        loraSerial.write(102);
        Serial.println("Gửi lệnh servo 1: 57°");
    }
    delay(100);

    // 🆕 Điều khiển servo 2 từ nút nhấn
    if (currentButtonState && !lastButtonState) {
        buttonState = !buttonState; // Đảo trạng thái servo 2
        if (buttonState) {
            loraSerial.write(200); // Mở servo 2 ra 180°
            Serial.println("Gửi lệnh servo 2: 180°");
        } else {
            loraSerial.write(201); // Đóng servo 2 về 0°
            Serial.println("Gửi lệnh servo 2: 0°");
        }
        delay(200); // Tránh rung phím
    }
    lastButtonState = currentButtonState;
}
