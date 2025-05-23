// Define pins
const int trigPin = 9;
const int echoPin = 10;
const int ledPins[] = {2, 3, 4, 5, 6};
const int buzzerPin = 7;
const int blinkLED = 2; // LED that will blink when the water level is low

// Constants for water level measurement
const float fullLevelDistance = 10.0; // Distance when the tank is full (cm)
const float emptyLevelDistance = 52.0; // Distance when the tank is empty (cm)
const int numLEDs = 5;
const float stepDistance = (emptyLevelDistance - fullLevelDistance) / numLEDs;

void setup() {
  // Initialize pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  for (int i = 0; i < numLEDs; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buzzerPin, OUTPUT);
  
  // Start serial communication
  Serial.begin(9600);
}

void loop() {
  // Measure distance
  float distance = measureDistance();
  
  // Determine the number of LEDs to light up
  int ledsToLight = determineLEDs(distance);
  
  // Control LEDs
  controlLEDs(ledsToLight);
  
  // Control buzzer and blinking LED
  controlBuzzerAndBlinkingLED(ledsToLight);
  
  // Print distance and status for debugging
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, LEDs: ");
  Serial.print(ledsToLight);
  Serial.print(", Buzzer: ");
  Serial.println(ledsToLight == 0 ? "BLINKING" : "OFF");
  
  // Wait before next measurement
  delay(1000);
}

float measureDistance() {
  // Send a pulse to trigger pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Read the echo pin
  long duration = pulseIn(echoPin, HIGH);
  
  // Calculate the distance in cm
  float distance = duration * 0.034 / 2;
  
  return distance;
}

int determineLEDs(float distance) {
  if (distance <= fullLevelDistance) {
    return numLEDs;
  } else if (distance >= emptyLevelDistance) {
    return 0;
  } else {
    return numLEDs - (distance - fullLevelDistance) / stepDistance;
  }
}

void controlLEDs(int ledsToLight) {
  for (int i = 0; i < numLEDs; i++) {
    if (i < ledsToLight) {
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}

void controlBuzzerAndBlinkingLED(int ledsToLight) {
  if (ledsToLight == 0) {
    digitalWrite(blinkLED, HIGH);
    tone(buzzerPin, 6000, 250);
    delay(500);
    digitalWrite(blinkLED, LOW);
    noTone(buzzerPin);
    delay(500);
  } else {
    // Make sure the blinking LED and buzzer are off
    digitalWrite(blinkLED, LOW);
    noTone(buzzerPin);
  }
}
