This Arduino-based Production Management System is designed to monitor and control aspects of a production environment using various sensors and actuators. 
The system integrates a DHT11 temperature sensor, an ultrasonic distance sensor, a stepper motor, an RGB LED, a buzzer, and a 20x4 I2C LCD display to provide real-time monitoring and feedback.

The Components Used:
*DHT11 Sensor:If the temperature drops below 33.3°C(depends on environmental requirements), the buzzer is triggered to alert operators (e.g., to indicate a cooling issue).

*Ultrasonic Sensor (HC-SR04):Measures the distance to an object in inches (converted from centimeters)

*RGB LED:Provides visual feedback about the proximity of objects, making it easy for operators to assess the situation at a glance.
Controls the RGB LED color based on distance ranges:
Red (≤ 2 inches): Indicates an object is very close.
Green (2–5 inches): Indicates a moderate distance.
Blue (5–9 inches): Indicates a farther distance.
Off (> 9 inches): No object detected in the relevant range

*20x4 I2C LCD Display:Displays the system title ("Production System"), the measured distance (in inches), and the temperature (in Celsius) with appropriate units.
Provides a clear, real-time interface for operators to monitor system status

*Stepper Motor (28BYJ-48 with ULN2003 Driver):Performs mechanical tasks by rotating clockwise and counterclockwise in full revolutions (2048 steps per revolution).
Likely used to move parts, open/close mechanisms, or drive a conveyor in the production process.

*Buzzer:Emits a beeping sound (200ms on, 100ms off, repeated three times) when the temperature is below 33.3°C, alerting operators to potential environmental issues
