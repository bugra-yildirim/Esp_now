
//ESP8266 ESP-NOW KULLANIMI
//Alıcı Kodu

#include <ESP8266WiFi.h>
#include <espnow.h>
#include <Wire.h>


    float t1;
    float h1;
    float t2;
    float h2;
    float t3;
    float h3;
    
int butondurum=0; //butonu takip için

// Alıncak veri yapısı için structure tanımlama
// Gönderici kartlar ile aynı yapı olmalı
typedef struct struct_message {
  int time; //zaman
  char name[]; //isim
  long tc; //tc

} struct_message;

// Bu yapıdan myData isimli bir veri oluşturduk
struct_message myData;

// VERİ ALIMI OLDUĞUNDA BU FONKSİYON ÇALIŞACAK
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Byte veri alındı: ");
  Serial.println(len);
  Serial.print("Zaman: ");
  Serial.println(myData.time);
  Serial.print("İsim: ");
  Serial.println(myData.name[]);
  Serial.print("TC: ");
  Serial.println(myData.tc);

//   Serial.println();
//   if (myData.time==1)
//     {  t1=myData.b;
//       h1=myData.c;
//     }
//  else if (myData.a==2)
//   {
//       t2=myData.b;
//       h2=myData.c;
//   }
//  else if (myData.a==3)
//   {
//       t3=myData.b;
//       h3=myData.c;
//   }
//   else {Serial.println("Bağlı kart yok");}

}
 
void setup() {
  
  Serial.begin(115200);
  pinMode(buton, INPUT_PULLUP);
  // Alıcı cihazı WIFI STATION olarak tanımlamamız gerekiyor
  WiFi.mode(WIFI_STA);

  // ESP NOW özelliğini başlatıyoruz
  if (esp_now_init() != 0) {
    Serial.println("ESP-NOW başlatılamadı");
    return;
  }
  
  // ESPNOW başlatıldığında 
  // kartın rolünü tanımladık ve veri alındığında çağrılacak fonksiyonu ilişkilendiriyoruz.
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  
   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { //Eğer ekran başlatılamazsa hata mesajı verecek
    Serial.println(F("SSD1306 ekran hatası"));
    for(;;);//sonsuza kadar döner buradan çıkmaz
  }
  delay(2000);
  display.clearDisplay();//ekranı temizliyoruz.
  display.setTextColor(WHITE);//ekran yazı rengini beyaza ayarladık


   
}

void loop() {

  
 
  
}
