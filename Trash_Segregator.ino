#include <Servo.h>
#include <Wire.h>
#include <LCD_I2C.h>

const int soilSensorPin = A0;   // Soil moisture sensor pin
const int irSensorPin = 8;      // Infrared sensor pin for detecting trash
const int servoMotorPin = 6;    // Servo motor control pin

LCD_I2C lcd(0x27);              // I2C LCD address
Servo servoMotor;

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();

  pinMode(irSensorPin, INPUT);  // Set the IR sensor pin as input
  servoMotor.attach(servoMotorPin);  // Attach the servo motor
}

void loop() {
  slideToCenter();  // Move the servo to the center (neutral) position
  int soilMoisture = analogRead(soilSensorPin);  // Read the moisture sensor value
  int irSensorValue = digitalRead(irSensorPin);  // Read the IR sensor value (HIGH when trash is detected)

  if (irSensorValue == HIGH) {  // If the IR sensor detects trash
    Serial.println("Trash detected");
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Trash");
    lcd.setCursor(4, 1);
    lcd.print("Detected");
    delay(2000);

    // Check if the detected trash is dry or wet based on soil moisture sensor reading
    if (soilMoisture > 1000) {
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Dry");
      lcd.setCursor(5, 1);
      lcd.print("Waste");
      slideToRight();  // Move servo to the right (dry waste bin)
      delay(1000);
      slideToCenter();  // Return the servo to the center
      Serial.println("Dry - Sliding to the right");
    } else {
      lcd.clear();
      lcd.setCursor(6, 0);
      lcd.print("Wet");
      lcd.setCursor(5, 1);
      lcd.print("Waste");
      slideToLeft();  // Move servo to the left (wet waste bin)
      delay(1000);
      slideToCenter();  // Return the servo to the center
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

  delay(500);  // Small delay to prevent excessive looping
}

// Function to move the servo to the center (neutral) position
void slideToCenter() {
  servoMotor.write(97);  // Adjust the angle for the neutral position
}

// Function to move the servo to the left (wet waste bin)
void slideToLeft() {
  for (int pos = 97; pos >= 7; pos--) {
    servoMotor.write(pos);
    delay(10);  // Adjust the delay to control the speed of the movement
  }
}

// Function to move the servo to the right (dry waste bin)
void slideToRight() {
  for (int pos = 97; pos <= 187; pos++) {
    servoMotor.write(pos);
    delay(10);  // Adjust the delay to control the speed of the movement
  }
}
