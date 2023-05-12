
/*

*/

#include <esp_now.h>     //haberleşme için gerekli kütüphaneler
#include <WiFi.h>
#include <SPI.h>       // spi haberleşme kütüphanesi
#include <MFRC522.h>
#include<Preferences.h>    // flash memory'ye kayıt fonk için kütüphane

#define led_halit 4  // kişileri gösteren led tanımları
#define led_emirhan 16
#define led_buse 17

#define SS_PIN  5  // ESP32 pin GIOP5           //rfid spi habeleşme pinleri
#define RST_PIN 27 // ESP32 pin GIOP27 
MFRC522 rfid(SS_PIN, RST_PIN);




/* RECEIVER MAC Address */
uint8_t broadcastAddress[] = {0xC8, 0xF0, 0x9E, 0x9E, 0x1A, 0x6C};   // gönderimi yaptığımız cihazın mac adresi

// göndereceğimiz verinin yapısı
typedef struct struct_message {
    bool halit;
    bool emirhan;
    bool buse;
} struct_message;

Preferences preferences_nesnesi1;  //flash memory'ye yazilacak kod icin degisken
Preferences preferences_nesnesi2;
Preferences preferences_nesnesi3;



// yeni tanımladığımız veri tipinde bir değişken tanımlıyoruz
struct_message myData;

esp_now_peer_info_t peerInfo;




/* Callback when data is sent. */
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");  // veri gönderilemediğinde yazdırılacak kod
}
 
void setup() {  
  /* Init Serial Monitor. */
  Serial.begin(9600); // Serial.begin(115200);
  
  SPI.begin();             //rfid haberleşme başlangıcı için gerekli kodlar
  rfid.PCD_Init(); // init MFRC522
  Serial.println("Tap an RFID/NFC tag on the RFID-RC522 reader");
  
  pinMode(led_halit, OUTPUT);               // led için ayarlanan pinler çıkış olarak ayarlanıyor
  pinMode(led_emirhan, OUTPUT);
  pinMode(led_buse, OUTPUT);




// wifi mode ayarı yapılıyor  esp_now kütüphanesinden
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

// veri gönderildiğinde çalıştırılacak kod için esp_now kütüphanesinden bir fonk ile register tanımı yapılıyor
  esp_now_register_send_cb(OnDataSent);
  

  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
 
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  
  //veri alani tanimlandi
  preferences_nesnesi1.begin("halit_flash", false);
  preferences_nesnesi2.begin("emirhan_flash", false);
  preferences_nesnesi3.begin("buse_flash", false);

  
  //evde olup olmama durumu flash memoriden cekiliyor, baglanti olmadiginda calisicak
  myData.halit = preferences_nesnesi1.getBool("durum", false);
  myData.emirhan = preferences_nesnesi2.getBool("durum", false);
  myData.buse = preferences_nesnesi3.getBool("durum", false);
  bool halit = myData.halit;
  bool emirhan = myData.emirhan;
  bool buse = myData.buse;
  
  digitalWrite(led_halit, halit);
  digitalWrite(led_emirhan, emirhan);
  digitalWrite(led_buse, buse);
  
}
 
void loop() {
  /* Send message via ESP-NOW. */
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));    // saniyede 1 defa veri gönderimi yapılıyor
  delay(1000);

      //gönderme durumunu belirten kod bloğu, seri monitör kalabalığından devre dışı bırakıldı
  // if (result == ESP_OK) {
  //   Serial.println("Sent with success");
  // }
  // else {
  //   Serial.println("Error sending the data");
  // }




                                  
  //halit id                 tanımlanan rfıd çiplerin id leri tanımlanıyor
  byte ID1[4] = {
  243,
  214,
  53,
  201
  };
  //emirhan id 
  byte ID2[4] = {
  99,
  176,
  214,
  52
  };
  //buse id 
  byte ID3[4] = {
  249,
  198,
  188,
  212
  };

  //rfid hex dönüştürücü                   hex olarak görüntüleme için eklenmişti devre dışı bırakıldı
  // if (rfid.PICC_IsNewCardPresent()) { // new tag is available 
  //   if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
  //     MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  //     Serial.print("RFID/NFC Tag Type: ");
  //     Serial.println(rfid.PICC_GetTypeName(piccType));

  //     // print UID in Serial Monitor in the hex format
  //     Serial.print("UID:");
  //     for (int i = 0; i < rfid.uid.size; i++) {
  //       Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
  //       Serial.print(rfid.uid.uidByte[i], HEX);
  //     }
  //     Serial.println();

  //     rfid.PICC_HaltA(); // halt PICC
  //     rfid.PCD_StopCrypto1(); // stop encryption on PCD
  //   }



  
      
  if (!rfid.PICC_IsNewCardPresent()) //Yeni kartın okunmasını bekliyoruz.
    return;

  if (!rfid.PICC_ReadCardSerial()) //Kart okunmadığı zaman bekliyoruz.
    return;

  if (rfid.uid.uidByte[0] == ID1[0] && //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
    rfid.uid.uidByte[1] == ID1[1] &&
    rfid.uid.uidByte[2] == ID1[2] &&
    rfid.uid.uidByte[3] == ID1[3]) {
    ekranaYazdir();
    if(myData.halit == 0){
      myData.halit = 1;
      Serial.println("Halit Buğra");           
    }
    else if(myData.halit == 1)
    {
      myData.halit = 0;
      Serial.println("Hoşçakal Halit Buğra");
    }  
  }        
  else if(rfid.uid.uidByte[0] == ID2[0] && //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
    rfid.uid.uidByte[1] == ID2[1] &&
    rfid.uid.uidByte[2] == ID2[2] &&
    rfid.uid.uidByte[3] == ID2[3]) {
    ekranaYazdir();
    if(myData.emirhan == 0)
    {
    myData.emirhan = 1;
    Serial.println("Hoşgeldin Emirhan");       
    }
    else if(myData.emirhan == 1)
    {
    myData.emirhan = 0;
    Serial.println("Hoşçakal Emirhan");    
    }
    }
    
  else if(rfid.uid.uidByte[0] == ID3[0] && //Okunan kart ID'si ile ID değişkenini karşılaştırıyoruz.
    rfid.uid.uidByte[1] == ID3[1] &&
    rfid.uid.uidByte[2] == ID3[2] &&
    rfid.uid.uidByte[3] == ID3[3]) {
    ekranaYazdir();
    if(myData.buse == 0)
    {
    myData.buse = 1;
    Serial.println("Hoşgeldin Buse");       
    }
    else if(myData.buse == 1){
    myData.buse = 0;
    Serial.println("Hoşçakal Buse");    
    }
  }
  
  else { //bilinmeyen kart durumu
    Serial.println("Bilinmeyen Kart");
    ekranaYazdir();
  }
  rfid.PICC_HaltA(); 
  preferences_nesnesi1.putBool("durum", myData.halit);
  preferences_nesnesi2.putBool("durum", myData.emirhan);
  preferences_nesnesi3.putBool("durum", myData.buse);  
  digitalWrite(led_halit, myData.halit);
  digitalWrite(led_emirhan, myData.emirhan);
  digitalWrite(led_buse, myData.buse); 
  delay(1); 

  // //durum kodları      kalabalığı engellemek için devre dışı bırakıldı                          
  // Serial.print("Bytes received: ");
  // Serial.println(len);
  // Serial.print("Halit evde mi:" );
  // Serial.println(myData.halit);
  // Serial.print("Emirhan evde mi:");
  // Serial.println(myData.emirhan);
  // Serial.print("Buse evde mi: ");
  // Serial.println(myData.buse); 
}

// id seri monitöre yazdırma kodu   tanımlama sırasında id görüntüleme için kullanılan kod
void ekranaYazdir() {
  Serial.print("ID Numarasi: ");
  for (int sayac = 0; sayac < 4; sayac++) {
    Serial.print(rfid.uid.uidByte[sayac]);
    Serial.print(" ");
  }
  Serial.println("");
}
  

