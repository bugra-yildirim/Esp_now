/*

*/

#include <esp_now.h>
#include <WiFi.h>

/* Structure example to receive data. */
/* Must match the sender structure. */
typedef struct struct_message {
    bool halit;
    bool emirhan;
    bool buse;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Halit evde mi:" );
  Serial.println(myData.halit);
  Serial.print("Emirhan evde mi:");
  Serial.println(myData.emirhan);
  Serial.print("Buse evde mi: ");
  Serial.println(myData.buse);
  Serial.print("-------------------- ");
}
 
void setup() {
  /* Initialize Serial Monitor. */
  Serial.begin(9600); // Serial.begin(115200);
  
  /* Set device as a Wi-Fi Station. */
  WiFi.mode(WIFI_STA);

  /* Init ESP-NOW. */
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  /* Once ESPNow is successfully Init, we will register for recv CB to */
  /* get recv packer info */
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  //memory functions
  //dedectors
  //alarm
  //situation codes

}
