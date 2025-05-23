#define trigPin 10  // Ultrasonic sensor trigger pin
#define echoPin 11  // Ultrasonic sensor echo pin
#define relayPin 8   // Relay control pin
#define motorSpeed 100 // Adjust motor speed (0-255)
#define minDistance 10  // Minimum distance for activating pump (cm)
#define L2 9
#define idrSensorPin A0
#define LED 13
long duration; // Variable to store pulse duration
int idrValue;    // Variable to store IDR sensor reading
int threshold;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(LED,OUTPUT);
  pinMode(idrSensorPin, INPUT);
  // Set motor speed (optional, use analogWrite for variable speed)
  analogWrite(motorSpeed, motorSpeed); // Adjust based on your motor driver
  threshold = 130;
  Serial.begin(9600); // Optional: Enable serial communication for debugging
}

void loop() {
  int distance = measureDistance();
  idrValue = analogRead(idrSensorPin);
  // Check if object is within range and activate pump
  if (distance <= minDistance) {
    Serial.println("Object detected, activating pump...");
    digitalWrite(relayPin, LOW);
    digitalWrite(L2,HIGH);  // Turn on relay, activating motor
  } else {
    Serial.println("No object detected, pump inactive.");
    digitalWrite(relayPin, HIGH);  
    digitalWrite(L2,LOW); // Turn off relay, deactivating motor
  }

  delay(100); // Adjust delay between measurements (milliseconds)
    if (idrValue > threshold) {
    
    digitalWrite(LED, HIGH);  // Turn on LED
  } else {
    digitalWrite(LED, LOW);   // Turn off LED
    Serial.println(idrValue);
  }
  
  delay(100);
}

int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert duration to distance in cm
}
