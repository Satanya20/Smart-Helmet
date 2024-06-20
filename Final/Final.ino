#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>

// Define the GPS and GSM pins
SoftwareSerial SIM900A(10, 11); // RX, TX for GSM
SoftwareSerial ss(4, 3); // RX, TX for GPS

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Pin definitions
const int forcePin = A4; // Force sensor pin
const int vibrationPin = A1; // Vibration sensor pin

// Threshold value to determine if helmet is worn
const int forceThreshold = 900;

unsigned long startTime = 0; // Time when the high vibration started
const unsigned long requiredDuration = 2000; // Duration in milliseconds for continuous high reading (2 seconds)
bool highReadingDetected = false; // Flag to indicate if the high reading is being detected

const char* remoteNumber = "+94759073291"; // Phone number to send SMS

void setup() {
  Serial.begin(9600);
  SIM900A.begin(9600);
  ss.begin(9600);

  Serial.println(F("GPS and GSM Module Example"));
}

void loop() {
  // Check if the helmet is being worn using the force sensor
  int forceValue = analogRead(forcePin);
  Serial.println(forceValue);
  if (forceValue < forceThreshold) {
    Serial.println("Helmet is being worn");

    // Check vibration sensor for confirmation of accident
    int vibrationLevel = analogRead(vibrationPin); // Read the analog value from the vibration sensor

    if (vibrationLevel > 512) { // Adjust the threshold value as needed
      if (!highReadingDetected) {
        // If high reading was not previously detected, start the timer
        highReadingDetected = true;
        startTime = millis();
      } else {
        // If high reading was already detected, check the duration
        if (millis() - startTime >= requiredDuration) {
          Serial.println("Accident confirmed by vibration sensor");

          // Fetch GPS location
          while (ss.available() > 0) {
            gps.encode(ss.read());
            if (gps.location.isUpdated()) {
              float latitude = gps.location.lat();
              float longitude = gps.location.lng();
              Serial.print("Latitude: ");
              Serial.println(latitude, 6);
              Serial.print("Longitude: ");
              Serial.println(longitude, 6);

              // Send a call via GSM module
              sendCall(remoteNumber);

              // Add a delay to prevent multiple calls
              delay(10000); // 10 seconds delay

              // Send SMS with location
              char message[160];
              snprintf(message, sizeof(message), "Accident detected. Please send help.\nLocation:\nLatitude: %.6f\nLongitude: %.6f", latitude, longitude);
              sendSMS(remoteNumber, message);

              // Small delay to prevent serial output flooding
              delay(500);
            }
          }
        }
      }
    } else {
      // Reset if the reading goes below the threshold
      highReadingDetected = false;
      Serial.println("Helmet is not being worn");
    }
  }
}

void sendCall(const char* number) {
  // Send AT command to initiate call
  SIM900A.println("ATD" + String(number) + ";");

  // Check for call status response from SIM900A
  if (waitForResponse("RING") || waitForResponse("NO CARRIER")) {
    Serial.println("Call initiated...");
  } else {
    Serial.println("Call Connected...");
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
