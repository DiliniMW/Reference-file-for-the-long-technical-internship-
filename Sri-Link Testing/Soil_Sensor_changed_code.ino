#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PIN_RX 16 // RO (Receive Pin)
#define PIN_TX 17 // DI (Transmit Pin)

// RS485 setup with ESP32
#define RE 32 // Connect RE terminal with 32 of ESP (Receiver Enable)
#define DE 33 // Connect DE terminal with 33 of ESP (Driver Enable)
#define HARDWARE_SW_1 27 // Hardware switch control pin

SoftwareSerial mod(PIN_RX, PIN_TX);

const byte test[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08}; // Test command to send to the sensor

byte readings[20]; // Buffer to store received data
float avgReading[7]; // Array to store average sensor readings
unsigned long timeoutStartTime; // Declare the timeout start time variable

void setup() {
  Serial.begin(115200); // Initialize serial communication
  enableExternalPrep(); // Enable external preparation
  initModBus(); // Initialize ModBus communication
  Serial.println("Sensor Reading Started");
}

void initModBus() {
  mod.begin(9600); // Initialize SoftwareSerial communication with the sensor
  pinMode(RE, OUTPUT); // Set RE pin as output
  pinMode(DE, OUTPUT); // Set DE pin as output
  digitalWrite(RE, LOW); // Set RE pin low by default
  digitalWrite(DE, LOW); // Set DE pin low by default
  delay(1000); // Wait for a brief moment
}

boolean readSensor() {
  digitalWrite(DE, HIGH); // Enable data transmission
  digitalWrite(RE, HIGH); // Enable data reception
  delay(10); // Short delay for faster response
  timeoutStartTime = millis(); // Record the start time for timeout tracking
  if (mod.write(test, sizeof(test)) == 8) { // Send test command to sensor
    delay(10); // Short delay for faster response
    digitalWrite(DE, LOW); // Disable data transmission
    digitalWrite(RE, LOW); // Disable data reception
    byte bytesRead = 0;
    while (bytesRead < 19 && millis() - timeoutStartTime < 1000) {
      if (mod.available()) {
        readings[bytesRead] = mod.read(); // Read data byte by byte
        bytesRead++;
      }
    }
    return bytesRead == 19; // Return true if all expected bytes were read
  }
  return false; // Return false if communication was unsuccessful
}

void loop() {
  if (readSensor()) {
    printSensorDataHex(); // Debug: Print the received data in hex
    printTemp();
    printPH();
    printMoisture();
    printCon();
    printN();
    printP();
    printK();
  } else {
    Serial.println("Failed to read sensor data");
  }
  delay(5000); // Delay before next reading
}

void printSensorDataHex() {
  for (byte i = 0; i < 19; i++) {
    Serial.print(readings[i], HEX); // Print received data in hexadecimal format
    Serial.print(" ");
  }
  Serial.println();
}

// Functions to print specific sensor readings
void printTemp() {
  int result = (readings[5] * 256) + readings[6];
  Serial.print("Temp: ");
  Serial.println(float(result) / 10, 1);
}

void printPH() {
  int result = (readings[9] * 256) + readings[10];
  Serial.print("pH: ");
  Serial.println(float(result) / 10, 1);
}

void printMoisture() {
  int result = (readings[3] * 256) + readings[4];
  Serial.print("Moisture: ");
  Serial.println(float(result) / 10, 1);
}

void printCon() {
  int result = (readings[7] * 256) + readings[8];
  Serial.print("Conductivity: ");
  Serial.println(result);
}

void printN() {
  int result = (readings[11] * 256) + readings[12];
  Serial.print("N: ");
  Serial.println(float(result) / 10, 1);
}

void printP() {
  int result = (readings[13] * 256) + readings[14];
  Serial.print("P: ");
  Serial.println(float(result) / 10, 1);
}

void printK() {
  int result = (readings[15] * 256) + readings[16];
  Serial.print("K: ");
  Serial.println(float(result) / 10, 1);
}

// Enable external preparation
void enableExternalPrep() {
  pinMode(HARDWARE_SW_1, OUTPUT); // Set hardware switch control pin as output
  digitalWrite(HARDWARE_SW_1, HIGH); // Turn on the hardware switch
  delay(100); // Wait for a brief moment
}
