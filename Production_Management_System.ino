// The Production Management System is an Arduino-based project designed to monitor and control a production environment.
// It uses a DHT11 sensor to measure temperature, an ultrasonic sensor to detect object proximity, and a stepper motor for mechanical tasks like moving components.
// The system displays real-time temperature and distance data on a 20x4 LCD, changes an RGB LED's color based on distance ranges, and triggers a buzzer if the temperature falls below 33.3°C. 
// Ideal for small-scale manufacturing


#include <Wire.h>                 // Library for I2C communication
#include <LiquidCrystal_I2C.h>    // Library for I2C LCD display
#include <Adafruit_Sensor.h>      // Base library for Adafruit sensors
#include <DHT.h>                  // Library for DHT11 temperature and humidity sensor
#define Type DHT11               // Define the DHT sensor type as DHT11
#include <Stepper.h>             // Library for controlling stepper motors

// Define the number of steps per revolution for the 28BYJ-48 stepper motor
const int stepsPerRevolution = 2048;  // Number of steps for one full rotation of the stepper motor

// Initialize the stepper motor object with the number of steps and pins connected to the ULN2003 driver
// Pins 9, 8, 7, 6 are connected to the ULN2003 driver for controlling the stepper motor
Stepper myStepper(stepsPerRevolution, 9, 8, 7, 6);

// Define pin for the DHT11 sensor
int sensePin = 2;                 // Pin connected to the DHT11 sensor
DHT HT(sensePin, Type);           // Initialize DHT sensor object with pin and type
float humidity;                   // Variable to store humidity (not used in the code)
float tempC;                      // Variable to store temperature in Celsius
float tempF;                      // Variable to store temperature in Fahrenheit (not used in the code)

// Define pins for the buzzer and RGB LED
int buzzerPin = 13;               // Pin connected to the buzzer
int redPin = 3;                   // Pin for the red channel of the RGB LED
int greenPin = 4;                 // Pin for the green channel of the RGB LED
int bluePin = 5;                  // Pin for the blue channel of the RGB LED

// Variables to store distance measurements
float cm = 0;                     // Distance in centimeters
float inches = 0;                 // Distance in inches

// Initialize the LCD with I2C address 0x3F for a 20x4 display
LiquidCrystal_I2C lcd(0x3F, 20, 4);

// Function to read distance using an ultrasonic sensor
// Parameters: triggerPin (sends the ultrasonic pulse), echoPin (receives the echo)
long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);    // Set trigger pin as output
  digitalWrite(triggerPin, LOW);  // Ensure trigger is low initially
  delayMicroseconds(2);           // Short delay for stability
  digitalWrite(triggerPin, HIGH); // Send a 10us pulse to trigger the sensor
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);        // Set echo pin as input
  // Return the time (in microseconds) for the sound wave to return
  return pulseIn(echoPin, HIGH);
}

// Function to set the color of the RGB LED
// Parameters: redValue, greenValue, blueValue (0-255 for PWM intensity)
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);    // Set red LED intensity
  analogWrite(greenPin, greenValue); // Set green LED intensity
  analogWrite(bluePin, blueValue);   // Set blue LED intensity
}

// Setup function: Runs once at startup
void setup() {
  lcd.init();                     // Initialize the LCD
  lcd.backlight();                // Turn on the LCD backlight

  myStepper.setSpeed(10);         // Set stepper motor speed (RPM)

  pinMode(buzzerPin, OUTPUT);     // Set buzzer pin as output
  Serial.begin(9600);             // Initialize serial communication at 9600 baud

  HT.begin();                     // Initialize the DHT11 sensor
  delay(10);                      // Short delay for sensor stabilization
}

// Main loop: Runs repeatedly after setup
void loop() {
  // Read temperature from DHT11 sensor
  tempC = HT.readTemperature();   // Get temperature in Celsius
  Serial.print("Temperature: ");  // Print temperature to Serial Monitor
  Serial.println(tempC);
  delay(100);                     // Short delay for stability

  // If temperature is below 33.3°C, activate buzzer
  for (int i = 0; i <= 2; i++) {
    if (tempC < 33.30) {
      digitalWrite(buzzerPin, HIGH); // Turn buzzer on
      delay(200);                    // Buzzer on for 200ms
      digitalWrite(buzzerPin, LOW);  // Turn buzzer off
      delay(100);                    // Pause for 100ms between beeps
    }
  }

  // Measure distance using ultrasonic sensor (pins 12 for trigger, 11 for echo)
  cm = 0.01723 * readUltrasonicDistance(12, 11); // Convert ping time to centimeters
  inches = (cm / 2.54);                          // Convert centimeters to inches

  // Set RGB LED color based on distance
  if (inches <= 2) {              // If distance is 2 inches or less
    setColor(255, 0, 0);          // Set RGB LED to red
  } else if (inches <= 5 && inches > 2) { // If distance is between 2 and 5 inches
    setColor(0, 255, 0);          // Set RGB LED to green
  } else if (inches <= 9 && inches > 5) { // If distance is between 5 and 9 inches
    setColor(0, 0, 255);          // Set RGB LED to blue
  } else {                        // If distance is greater than 9 inches
    setColor(0, 0, 0);            // Turn off RGB LED
  }

  // Display information on the LCD
  lcd.setCursor(0, 0);            // Set cursor to top-left
  lcd.print("Production System"); // Display system title
  lcd.setCursor(0, 2);            // Set cursor to third row
  lcd.print("Distance:");         // Display distance label
  lcd.print(inches);              // Display distance in inches
  lcd.setCursor(14, 2);           // Move cursor to align units
  lcd.print("in");                // Display inches unit
  lcd.setBacklight(255);          // Ensure backlight is on
  lcd.setCursor(0, 3);            // Set cursor to fourth row
  lcd.print("Temperature:");      // Display temperature label
  lcd.print(tempC);               // Display temperature in Celsius
  lcd.print(char(176));           // Display degree symbol
  lcd.print('C');                 // Display Celsius unit
  lcd.setBacklight(255);          // Ensure backlight is on
  delay(10);                      // Short delay for display stability

  // Control the stepper motor
  int motDir = 1;                 // Motor direction (1 for clockwise)
  motDir = motDir * 1;            // Redundant multiplication (no effect)
  myStepper.step(motDir);         // Move motor one step clockwise
  myStepper.step(stepsPerRevolution); // Rotate motor one full revolution clockwise
  delay(500);                     // Delay for 500ms

  // Rotate motor one full revolution counterclockwise
  myStepper.step(-stepsPerRevolution);
  delay(500);                     // Delay for 500ms
}





