#include <LiquidCrystal.h>

// Initialize the LCD (using the same pins as requested)
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// Define ultrasonic sensor pins
const int trigPin1 = 7, echoPin1 = 6;
const int trigPin2 = 5, echoPin2 = 4;
const int trigPin3 = 3, echoPin3 = 2;

// Speed of sound in cm/us
const float speedOfSound = 0.0343;

void setup() {
  // Initialize the LCD
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();

  // Initialize the Serial Monitor
  Serial.begin(9600);

  // Configure the trig pins as OUTPUT and echo pins as INPUT
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  Serial.println("Ultrasonic Sensor Test Started");
}

void loop() {
  // Get distance from the three sensors
  float distance1 = getDistance(trigPin1, echoPin1);
  delay(333);
  float distance2 = getDistance(trigPin2, echoPin2);
  delay(333);
  float distance3 = getDistance(trigPin3, echoPin3);
  delay(333);

  // Print the distances to the Serial Monitor
  Serial.print("Sensor 1: ");
  Serial.print(distance1);
  Serial.print(" cm");

  Serial.print("\t Sensor 2: ");
  Serial.print(distance2);
  Serial.print(" cm");

  Serial.print("\t Sensor 3: ");
  Serial.print(distance3);
  Serial.println(" cm");

  // Check conditions and display on the LCD
  lcd.clear();

  evaluatePose(distance1, distance2, distance3);
}

void evaluatePose(float distance1, float distance2, float distance3) {
  lcd.clear();

  // Handle sensor failure cases
  if (distance1 == -1 && distance2 == -1 && distance3 == -1) {
    lcd.print("Dangerous Pose");
    Serial.println("Dangerous Pose");
  } else if ((distance1 == -1 && distance2 == -1) || 
             (distance1 == -1 && distance3 == -1) || 
             (distance2 == -1 && distance3 == -1)) {
    lcd.print("Dangerous Pose");
    Serial.println("Dangerous Pose");
  } else if (distance1 == -1 || distance2 == -1 || distance3 == -1) {
    lcd.print("Bad Pose");
    Serial.println("Bad Pose");
  } else {
    // Handle difference conditions
    float diff12 = abs(distance1 - distance2);
    float diff13 = abs(distance1 - distance3);
    float diff23 = abs(distance2 - distance3);

    if (diff12 <= 5 && diff13 <= 5 && diff23 <= 5) {
      lcd.print("Good Pose");
      Serial.println("Good Pose");
    } else if ((diff12 > 5 && diff12 <= 10) ||
               (diff13 > 5 && diff13 <= 10) ||
               (diff23 > 5 && diff23 <= 10)) {
      lcd.print("Bad Pose");
      Serial.println("Bad Pose");
    } else if (diff12 > 10 && diff13 > 10 && diff23 > 10) {
      lcd.print("Dangerous Pose");
      Serial.println("Dangerous Pose");
    }
  }
}

float getDistance(int trigPin, int echoPin) {
  const float speedOfSound = 0.0343;  // Speed of sound in cm/us

  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Send a 10-microsecond pulse to the trigPin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the echo time with a 30ms timeout (avoiding indefinite block)
  long duration = pulseIn(echoPin, HIGH, 30000); // Timeout = 30ms

  // Handle cases where no valid echo was received
  if (duration == 0 || ((duration * speedOfSound) / 2)>50) {
    return -1;  // Indicates an error
  }

  // Calculate and return distance in cm
  return (duration * speedOfSound) / 2;
}
