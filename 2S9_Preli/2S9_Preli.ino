#include <esp_now.h>
#include <WiFi.h>
#include <TinyGPS++.h> // Invoke custom library

// Define the RX and TX pins for Serial 2
#define RXD2 16
#define TXD2 17
#define GPS_BAUD 9600

// The TinyGPS++ object
TinyGPSPlus gps;

// Create an instance of the HardwareSerial class for Serial 2
HardwareSerial gpsSerial(2);

#define LED1_PIN 33  // Define the first LED pin (GPIO 2 for example)
#define LED2_PIN 25   // Define the second LED pin (GPIO 4 for example)
#define LED3_PIN 32


uint8_t broadcastAddress[] = {0x78, 0xEE, 0x4C, 0x00, 0xA3, 0xB4};

typedef struct {
  int id;
  float lat;
  float lng;
  int ledCommand;  // Add LED command (0: both off, 1: LED1 on, 2: LED2 on)
} struct_message;

struct_message myData;
struct_message mode;

esp_now_peer_info_t peerInfo;


void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
   char macStr[18];
  Serial.print("Packet received from: ");
snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
         info->src_addr[0], info->src_addr[1], info->src_addr[2], 
         info->src_addr[3], info->src_addr[4], info->src_addr[5]);
  //
  memcpy(&mode, incomingData, sizeof(mode));  // Copy the incoming data to 'mode' struct

  Serial.print("Bytes received: ");
  Serial.println(len);
  
  // Handle LED commands
  if (mode.ledCommand == 1) {
    Serial.println("Turning LED1 ON, LED2 OFF");
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, LOW);  
    digitalWrite(LED3_PIN, LOW);
  } else if (mode.ledCommand == 2) {
    Serial.println("Turning LED2 ON, LED1 OFF");
    digitalWrite(LED1_PIN, LOW);   
    digitalWrite(LED2_PIN, HIGH);
    digitalWrite(LED3_PIN, LOW) ; 
  }  else if (mode.ledCommand == 3) {
    Serial.println("Turning LED2 ON, LED1 OFF");
    digitalWrite(LED1_PIN, LOW);   
    digitalWrite(LED2_PIN, LOW); 
    digitalWrite(LED3_PIN, HIGH); 
  }
  else {
    Serial.println("Turning both LEDs OFF");
    digitalWrite(LED1_PIN, LOW);  
    digitalWrite(LED2_PIN, LOW); 
    digitalWrite(LED3_PIN, LOW);  
  }
}

void setup() {
  Serial.begin(115200);
    // Start Serial 2 with the defined RX and TX pins and a baud rate of 9600
  gpsSerial.begin(GPS_BAUD, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Serial 2 started at 9600 baud rate");

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);  // Register receive callback

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  unsigned long start = millis();

  while (millis() - start < 1000) {
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    if (gps.location.isUpdated()) {
      Serial.print("LAT: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("LONG: "); 
      Serial.println(gps.location.lng(), 6);
    float lat = (gps.location.lat());
    float lng = (gps.location.lng());

  myData.id = 2;
  myData.lat = lat;
  myData.lng=lng;
  
  Serial.print("Sending: ");
  Serial.println(myData.lat);
  Serial.println(myData.lng);

  }
  }
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  
  delay(1000);
}
