#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// LCD setup: 0x27 address, 16 columns, 4 rows
LiquidCrystal_I2C lcd(0x27, 16, 4);

// Servo motor
Servo myservo;
int servoPos = 90;  // Initial center position

// LDR pin setup
#define LDR_LEFT A1  // East-facing LDR
#define LDR_RIGHT A0  // West-facing LDR
int ldrTolerance = 50; // Adjust for sensitivity

// Voltage sensor setup
#define VOLT_SENSOR A2
float vOUT = 0.0;
float vIN = 0.0;
float R1 = 30000.0; // 30k ohm
float R2 = 7500.0;  // 7.5k ohm

void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("    WELCOME  ");
  delay(2000);
   lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Solar Tracking");
  lcd.setCursor(0, 1);
  lcd.print("System in AI");
  delay(2000);
  lcd.clear();

  myservo.attach(3);  // Servo on D3
  myservo.write(servoPos);

  pinMode(LDR_LEFT, INPUT);
  pinMode(LDR_RIGHT, INPUT);
}

void loop() {
  // ---- Voltage Reading ----
  int rawValue = analogRead(VOLT_SENSOR);
  vOUT = (rawValue * 5.0) / 1024.0;
  vIN = vOUT / (R2 / (R1 + R2));  // Voltage divider formula

  // ---- LDR Reading ----
  int ldrLeft = analogRead(LDR_LEFT);
  int ldrRight = analogRead(LDR_RIGHT);

  // ---- Tracking Logic ----
  if (abs(ldrLeft - ldrRight) <= ldrTolerance) {
    // No major difference, do nothing
  } else if (ldrLeft > ldrRight) {
    servoPos++;
  } else {
    servoPos--;
  }

  // ---- Servo Control ----
  servoPos = constrain(servoPos, 0, 180);
  myservo.write(servoPos);

  // ---- LCD Display ----
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voltage: ");
  lcd.print(vIN, 2);
  lcd.print(" V");

  lcd.setCursor(0, 1);
  if (abs(ldrLeft - ldrRight) <= ldrTolerance) {
    lcd.print("Tracking: Stable");
    Serial.println("Stable");
  } else if (ldrLeft > ldrRight) {
    lcd.print("Tracking: East ");
      Serial.println("East");
  } else {
    lcd.print("Tracking: West ");
     Serial.println("West");
  }

  // ---- Serial Monitor ----
  Serial.print("Voltage: ");
  Serial.print(vIN, 2);
  Serial.print(" V | ");
  Serial.print("LDR Left: ");
  Serial.print(ldrLeft);
  Serial.print(" | LDR Right: ");
  Serial.print(ldrRight);
  Serial.print(" | Direction: ");
  delay(500);
}
