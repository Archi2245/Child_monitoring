#include <TinyGPS.h>
#include <SoftwareSerial.h>

TinyGPS gps;
SoftwareSerial Gsm(7, 8);  // GSM module TX, RX
SoftwareSerial GPSModule(2, 3);  // GPS TX, RX

char phone_no[] = "+91"; // Replace with your phone number
int soundSensorPin = 6; // Sound sensor DO pin
int button1Pin = 4;     // Emergency SMS button pin (connected to D4)
int button2Pin = 5;     // Call button pin (connected to D5)

bool smsSent = false;   // To prevent continuous SMS

void setup() {
  Serial.begin(9600);
  Gsm.begin(9600);
  GPSModule.begin(9600);

  pinMode(soundSensorPin, INPUT);
  pinMode(button1Pin, INPUT);  // Button 1 connected to D4 with pull-down
  pinMode(button2Pin, INPUT);  // Button 2 connected to D5 with pull-down

  // Initialize GSM module
  sendCommand("AT", 1000, "OK");
  sendCommand("AT+CMGF=1", 1000, "OK"); // Set SMS mode
  sendCommand("AT+CNMI=2,2,0,0,0", 1000, "OK"); // Configure SMS notifications

  Serial.println("Setup complete. Waiting for input...");
}

void loop() {
  // Handle button 1 (SMS)
  if (digitalRead(button1Pin) == HIGH) {  // Button 1 pressed
    Serial.println("Button 1 pressed. Sending SMS...");
    sendSMS();
    smsSent = true; // Mark SMS as sent
  }

  // Handle sound sensor (detect heavy noise or cry)
  if (digitalRead(soundSensorPin) == HIGH && !smsSent) {
    if (detectHeavyNoise()) {  // Ensure it's a loud noise
      sendSMS();
      smsSent = true; // Prevent continuous SMS
    }
  } else if (digitalRead(soundSensorPin) == LOW) {
    smsSent = false; // Reset SMS sent status if noise stops
  }

  // Handle button 2 (Call)
  if (digitalRead(button2Pin) == HIGH) {  // Button 2 pressed
    Serial.println("Button 2 pressed. Making call...");
    handleCall();
  }
}

// Function to handle SMS logic
void sendSMS() {
  float flat, flon;
  unsigned long age;
  bool locationAvailable = false;

  // Try to get GPS location within 10 seconds
  unsigned long startTime = millis();
  while (millis() - startTime < 10000) {
    while (GPSModule.available()) {
      char c = GPSModule.read();
      if (gps.encode(c)) {
        locationAvailable = true;
        gps.f_get_position(&flat, &flon, &age);
        if (flat != TinyGPS::GPS_INVALID_F_ANGLE && flon != TinyGPS::GPS_INVALID_F_ANGLE) {
          break;
        }
      }
    }
  }

  // If no valid GPS data is available, use a fallback
  if (!locationAvailable || flat == TinyGPS::GPS_INVALID_F_ANGLE || flon == TinyGPS::GPS_INVALID_F_ANGLE) {
    flat = 0.0;
    flon = 0.0;
  }

  // Send SMS with GPS location
  Gsm.println("AT+CMGF=1");
  delay(100);
  Gsm.print("AT+CMGS=\"");
  Gsm.print(phone_no);
  Gsm.println("\"");
  delay(100);
  Gsm.print("Emergency! I need help. Location: ");
  Gsm.print("http://maps.google.com/maps?q=loc:");
  Gsm.print(flat, 6);
  Gsm.print(",");
  Gsm.print(flon, 6);
  Gsm.write(26); // End of message (Ctrl+Z)
  delay(5000);

  Serial.println("SMS sent.");
}

// Function to handle call logic
void handleCall() {
  Serial.println("Making a call...");

  // Send AT command to dial the number
  Gsm.print("ATD");
  Gsm.print(phone_no);
  Gsm.println(";");
  delay(10000); // Wait 10 seconds for the call to proceed

  // Hang up the call
  Gsm.println("ATH");
  delay(1000); // Ensure the call is properly ended
  Serial.println("Call ended.");
}

// Function to detect heavy noise (e.g., shouting or crying)
bool detectHeavyNoise() {
  int noiseLevel = analogRead(soundSensorPin); // Read analog value
  Serial.print("Noise level: ");
  Serial.println(noiseLevel);

  // Adjust the threshold based on your sound sensor
  return noiseLevel > 800;  // Set to a high threshold for loud noise
}

// Function to send AT commands to GSM and check for a specific response
bool sendCommand(const char* command, unsigned int timeout, const char* expectedResponse) {
  Gsm.println(command);
  unsigned long startTime = millis();
  while (millis() - startTime < timeout) {
    if (Gsm.available()) {
      String response = Gsm.readString();
      if (response.indexOf(expectedResponse) != -1) {
        return true; // Found the expected response
      }
    }
  }
  return false; // Timed out without finding the expected response
}