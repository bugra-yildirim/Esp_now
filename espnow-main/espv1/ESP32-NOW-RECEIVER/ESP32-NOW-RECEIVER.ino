/*

*/

#include <esp_now.h>
#include <WiFi.h>
#include<Preferences.h>

/* Structure example to receive data. */
/* Must match the sender structure. */
typedef struct struct_message {
    bool halit;
    bool emirhan;
    bool buse;
} struct_message;

Preferences preferences_nesnesi1;  //flash memory'ye yazilacak kod icin degisken
Preferences preferences_nesnesi2;
Preferences preferences_nesnesi3;

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
  preferences_nesnesi1.putBool("durum", myData.halit);
  preferences_nesnesi2.putBool("durum", myData.emirhan);
  preferences_nesnesi3.putBool("durum", myData.buse);  
  Serial.println("Durum FLASH hafızaya kaydedildi");
  Serial.println("-------------------- ");
}
 
void setup() {
  /* Initialize Serial Monitor. */
  Serial.begin(9600); // Serial.begin(115200);
  Serial.println();
  Serial.println("--- SYSTEM STARTING ---");
  Serial.println("--- PLEASE STAND BY ---");  
  
  
  
  
  //veri alani tanimlandi
  preferences_nesnesi1.begin("halit_flash", false);
  preferences_nesnesi2.begin("emirhan_flash", false);
  preferences_nesnesi3.begin("buse_flash", false);
  //evde olup olmama durumu flash memoriden cekiliyor, baglanti olmadiginda calisicak
  myData.halit = preferences_nesnesi1.getBool("durum", false);
  myData.emirhan = preferences_nesnesi2.getBool("durum", false);
  myData.buse = preferences_nesnesi3.getBool("durum", false);  
  Serial.print("Halit evde mi:" );
  Serial.println(myData.halit);
  Serial.print("Emirhan evde mi:");
  Serial.println(myData.emirhan);
  Serial.print("Buse evde mi: ");
  Serial.println(myData.buse);
  
  
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
  //memory functions-------- ok in setup
  //dedectors
  //alarm
  //situation codes

}
