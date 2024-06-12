#include <Wire.h>
#include <SoftwareSerial.h>

// GSM module phone number
const char* remoteNumber = "+94772244484";

// Initialize SoftwareSerial for GSM module
SoftwareSerial SIM900A(10, 11); // RX, TX

// Pin definitions
const int forcePin = A4; // Force sensor pin
const int vibrationPin = A1; // Vibration sensor pin

// Threshold value to determine if helmet is worn
const int forceThreshold = 100;

void setup() {
  Serial.begin(9600);
  SIM900A.begin(9600);

  Wire.begin();
  // Initialize vibration sensor pin
  pinMode(vibrationPin, INPUT);
}

void loop() {
  // Check if the helmet is being worn using the force sensor
  int forceValue = analogRead(forcePin);
  Serial.println(forceValue);
  if (forceValue < forceThreshold) {
    Serial.println("Helmet is being worn");

    // Check vibration sensor for confirmation of accident
    int vibrationLevel = analogRead(vibrationPin); // Read the analog value from the vibration sensor

  // Print the vibration level to the serial monitor
    Serial.print("Vibration Level: ");
    Serial.println(vibrationLevel);

    if (vibrationLevel > 100 ) {
      Serial.println("Accident confirmed by vibration sensor");

      // Send a call via GSM module
      sendCall(remoteNumber);

      // Add a delay to prevent multiple calls
      delay(10000); // 10 seconds delay
      sendSMS(remoteNumber, "Accident detected. Please send help.");
    }
  } else {
    Serial.println("Helmet is not being worn");
  }

  // Small delay to prevent serial output flooding
  delay(500);
}

void sendCall(const char* number) {
  // Send AT command to initiate call
  SIM900A.println("ATD" + String(number) + ";");

  // Check for call status response from SIM900A
  if (waitForResponse("RING") || waitForResponse("NO CARRIER")) {
    Serial.println("Call initiated...");
  } else {
    Serial.println("Call failed!");
  }

  // Wait for call to end (replace with logic for call duration control if needed)
  delay(10000); // Adjust delay based on expected call duration

  // Hang up the call (optional)
  SIM900A.println("ATH");
}

void sendSMS(const char* number, const char* message) {
  Serial.println("Sending Message");
  SIM900A.println("AT+CMGF=1"); // Sets the GSM Module in Text Mode
  delay(1000);
  Serial.println("Set SMS Number");
  SIM900A.println("AT+CMGS=\"" + String(number) + "\"\r"); // Mobile phone number to send message
  delay(1000);
  Serial.println("Set SMS Content");
  SIM900A.println(message); // Message content
  delay(100);
  Serial.println("Finish");
  SIM900A.println((char)26); // ASCII code of CTRL+Z
  delay(1000);
  Serial.println("Message has been sent");
}

// Function to wait for specific response from SIM900A
bool waitForResponse(const char* response) {
  int timeout = 1000; // Adjust timeout value as needed (milliseconds)
  while (timeout-- > 0) {
    if (SIM900A.available()) {
      String data = SIM900A.readStringUntil('\n');
      if (data.indexOf(response) >= 0) {
        return true;
      }
    }
    delay(1);
  }
  return false;
}
