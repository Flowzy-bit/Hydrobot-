#include <esp_now.h>
#include <WiFi.h>
#include "SPI.h"
#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();

// MAC addresses of the two slaves
uint8_t slave1MacAddress[] = {0x1C, 0x69, 0x20, 0x31, 0x06, 0x20};
uint8_t slave2MacAddress[] = {0x1C, 0x69, 0x20, 0x30, 0xF9, 0x20};

// Structure for ESP-NOW communication
typedef struct struct_message {
  int id;
  float lat;
  float lng;
  int ledCommand;  // Added to handle LED commands
} struct_message;

struct_message myData;
struct_message board1;
struct_message board2;
struct_message mode;

struct_message boardsStruct[2] = {board1, board2};

esp_now_peer_info_t peerInfo;

// Callback function that will be executed when data is received
void OnDataRecv(const esp_now_recv_info *info, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           info->src_addr[0], info->src_addr[1], info->src_addr[2], 
           info->src_addr[3], info->src_addr[4], info->src_addr[5]);

  // Copy the received data into the myData struct
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);

  // Update the appropriate board's distance in the array
  boardsStruct[myData.id - 1].lat = myData.lat;
  boardsStruct[myData.id - 1].lng = myData.lng;

  // Debugging distance readings
  Serial.println("Board 1 Distance: ");
  Serial.println(boardsStruct[0].lat,6);
  Serial.println("Board 2 Distance: ");
  Serial.println(boardsStruct[1].lat,6);

  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println();
  delay(1000);
}

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t ");
  Serial.println(status);
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Message sent" : "Message failed");
}

// Function to display distances on the screen
void display() {
  float board1lat = boardsStruct[0].lat;
  float board2lat = boardsStruct[1].lat;
  float board1lng = boardsStruct[0].lng;
  float board2lng = boardsStruct[1].lng;
  

  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0);
  tft.println("1S9");
  tft.print("Latitude: ");
  tft.println(board1lat, 6);
  tft.print("Longitude: ");
  tft.println(board1lng, 6);
  tft.setCursor(0,90);
  tft.println("2S9");
  tft.print("Latitude: ");
  tft.println(board2lat, 6);
  tft.print("Longitude: ");
  tft.println(board2lng, 6);


  if (mode.ledCommand == 1){
    tft.setCursor(0,150);
  tft.setTextColor(TFT_YELLOW);
  tft.print("Mode: ");
  tft.println("Autonomous Enabled");
}
else if (mode.ledCommand == 2){
   tft.setCursor(0,150);
  tft.setTextColor(TFT_YELLOW);
  tft.print("Mode: ");
  tft.println("S-Pattern Enabled");
}
else{
   tft.setCursor(0,150);
  tft.setTextColor(TFT_YELLOW);
  tft.print("Mode: ");
  tft.println("Standing by");
}
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESPNow/Basic/Slave Example");
//Set pins functions
  pinMode(16, INPUT);
  pinMode(17, INPUT);
//Initialize TFT Display
tft.init();
  tft.setRotation(1);
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);
//initialize WIFI and ESP Now Protocol
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register the first peer
  memcpy(peerInfo.peer_addr, slave1MacAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }

  // Register the second peer
  memcpy(peerInfo.peer_addr, slave2MacAddress, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_send_cb(OnDataSent); //Register send callback
  esp_now_register_recv_cb(OnDataRecv);  // Register receive callback
}

void loop() {
  // Check if button 16 is pressed (LED1 ON)
  if (digitalRead(16) == HIGH) {
    mode.ledCommand = 1;  // Set LED1 ON command
    esp_err_t result = esp_now_send(0, (uint8_t *) &mode, sizeof(mode));

    if (result == ESP_OK) {
      Serial.println("Sent mode 1 with LED1 ON success");
    } else {
      Serial.println("Error sending the data");
    }
    delay(50);  // Debounce delay
  }

  // Check if button 17 is pressed (LED2 ON)
  if (digitalRead(17) == HIGH) {
    mode.ledCommand = 2;  // Set LED2 ON command
    esp_err_t result = esp_now_send(0, (uint8_t *) &mode, sizeof(mode));

    if (result == ESP_OK) {
      Serial.println("Sent mode 2 with LED2 ON success");
    } else {
      Serial.println("Error sending the data");
    }
    delay(50);  // Debounce delay
  }

    display();  // Update the display
  delay(1000);
}
