/**
 * JY-MCU Bluetooth module communication example for Arduino.
 *
 * Connect RX_PIN to TX pin of the module,
 * Connect TX_PIN to RX pin of the module.
 *
 * Based on SoftwareSerial example by Tom Igoe and Mikal Hart.
 * Adapted for JY-MCU module by Tautvidas Sipavicius.
 *
 * This example code is in the public domain.
 */

#include <SoftwareSerial.h>

const int RX_PIN = 10;
const int TX_PIN = 11;
const int ledPin = 13;
const int BLUETOOTH_BAUD_RATE = 9600;

int state = 0;

SoftwareSerial bluetooth(RX_PIN, TX_PIN);

void setup() {
   Serial.begin(9600);
   bluetooth.begin(BLUETOOTH_BAUD_RATE);
   pinMode(ledPin,OUTPUT);
}

void loop() {
  if(bluetooth.available() > 0){ // Checks whether data is comming from the serial port
    state = bluetooth.read(); // Reads the data from the serial port
 }
 if (state == '0') {
  digitalWrite(ledPin, LOW); // Turn LED OFF
  Serial.println("LED: OFF"); // Send back, to the phone, the String "LED: ON"
  state = 0;
 }
 else if (state == '1') {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED: ON");;
  state = 0;
 } 
}
