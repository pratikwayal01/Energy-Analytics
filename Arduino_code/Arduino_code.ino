#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Replace '0x27' with your LCD's I2C address

// Define analog inputs
#define VOLTAGE_PIN A0
#define CURRENT_PIN A1

// Floats for ADC voltage & input voltage
float adc_voltage = 0.0;
float in_voltage = 0.0;

// Floats for resistor values in divider (in ohms)
float R1 = 30000.0;
float R2 = 7500.0;

// Float for reference voltage
float ref_voltage = 5.0;

// Integer for ADC values
int voltage_adc_value = 0;
int current_adc_value = 0;

Servo myservo;  // Create a Servo object

int pos = 0;    // Initialize the initial servo position
int increment = 15; // Number of degrees to rotate each time (15 degrees)

void setup() {
  Wire.begin();
  lcd.init();       // Initialize the LCD
  lcd.backlight();  // Turn on the backlight
  lcd.setCursor(0, 0);
  lcd.print("Voltage & Current");
  lcd.setCursor(0, 1);
  lcd.print("V: 0.00V  I: 0.00A");

  Serial.begin(9600);

  myservo.attach(9);  // Attaches the servo on pin 9
}

void loop() {
  // Read the Analog Inputs
  voltage_adc_value = analogRead(VOLTAGE_PIN);
  current_adc_value = analogRead(CURRENT_PIN);

  // Determine voltage at ADC input
  adc_voltage = (voltage_adc_value * ref_voltage) / 1024.0;

  // Calculate voltage at divider input
  in_voltage = adc_voltage * (R1 + R2) / R2;

  // Display the voltage on the LCD
  lcd.setCursor(3, 1);
  lcd.print(in_voltage, 2); // Display voltage with 2 decimal places
  lcd.print("V");

  // Calculate current from analog input A1
  float current = (.044 * current_adc_value - 3.78);

  // Display the current on the LCD
  lcd.setCursor(12, 1);
  lcd.print(current, 2); // Display current with 2 decimal places
  lcd.print("A");

  // Print results to the serial monitor
  Serial.print("Voltage: ");
  Serial.print(in_voltage, 2); // Display voltage with 2 decimal places
  Serial.print("V   Current: ");
  Serial.print(current, 2); // Display current with 2 decimal places
  Serial.println("mA");

  // Rotate the servo by the specified increment
  myservo.write(pos);

  // Increment the position by 15 degrees
  pos += increment;

  // Wrap around to 0 degrees if we've gone beyond 180 degrees
  if (pos > 110) {
    pos = 0;
  }

  // Delay for 10 seconds
  delay(8000); // Adjust the delay as needed for your application
}
