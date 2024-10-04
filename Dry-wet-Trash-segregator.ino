#include <Servo.h>
#include <Wire.h>
#include <LCD_I2C.h>

const int soilSensorPin = A0;
const int irSensorPin = 8;
const int servoMotorPin = 6;

LCD_I2C lcd(0x27);
Servo servoMotor;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  pinMode(irSensorPin, INPUT);
  servoMotor.attach(servoMotorPin);
}

void loop() {
  slideToCenter();
  int soilMoisture = analogRead(soilSensorPin);
  int irSensorValue = digitalRead(irSensorPin);

  if (irSensorValue == HIGH) {
    Serial.println("Trash detected");
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Trash");
    lcd.setCursor(4, 1);
    lcd.print("Detected");
    delay(2000);

    if (soilMoisture > 1000) {
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Dry");
      lcd.setCursor(5, 1);
      lcd.print("Waste");
      slideToRight();
      delay(1000);
      slideToCenter();
      Serial.println("Dry - Sliding to the right");
    } else {
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Wet");
      lcd.setCursor(5, 1);
      lcd.print("Waste");
      slideToLeft();
      delay(1000);
      slideToCenter();
      Serial.println("Wet - Sliding to the left");
    }

    Serial.print("Moisture: ");
    Serial.println(soilMoisture);
  } else {
    Serial.println("No trash detected");
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("Wet and Dry");
    lcd.setCursor(2, 1);
    lcd.print(" Segregator");
  }

  delay(500);
}

void slideToCenter() {
  servoMotor.write(97);
}

void slideToLeft() {
  for (int pos = 97; pos >= 7; pos--) {
    servoMotor.write(pos);
    delay(10);
  }
}

void slideToRight() {
  for (int pos = 97; pos <= 187; pos++) {
    servoMotor.write(pos);
    delay(10);
  }
}
