/*

*/

#include <esp_now.h>
#include <WiFi.h>
#include<Preferences.h>
#include<MPU6050_tockn.h>
#include <Wire.h>

int buzzer = 32;
MPU6050 mpu6050(Wire);

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
  
  //MPU sensör başlatma ayarları. İlk başlamada kartı stabil tutun. Gyro ofsetleri belirler.
  Wire.begin();
  mpu6050.begin();
  Serial.println("MPU-6050 sensör bulundu kartı oynatmadan bekleyin");
  mpu6050.calcGyroOffsets(true);

  pinMode(buzzer,OUTPUT);
  
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

  int situation_code=0;
  //0 normal
  //1 alarm
  //2 collapse
  
  
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
  int x_eks1,y_eks1,z_eks1,x_eks2,y_eks2,z_eks2,degisim = 0;
  mpu6050.update();//verileri güncellemek için gerekli
  Serial.print("açı_X : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("\açı_Y : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("\açı_Z : ");
  Serial.println(mpu6050.getAngleZ());
  x_eks1= mpu6050.getAngleX();
  y_eks1= mpu6050.getAngleY();
  z_eks1= mpu6050.getAngleZ();
  delay(10); 
  mpu6050.update();//verileri güncellemek için gerekli
  Serial.print("açı_X : ");
  Serial.print(mpu6050.getAngleX());
  Serial.print("açı_Y : ");
  Serial.print(mpu6050.getAngleY());
  Serial.print("açı_Z : ");
  Serial.println(mpu6050.getAngleZ());
  x_eks2= mpu6050.getAngleX();
  y_eks2= mpu6050.getAngleY();
  z_eks2= mpu6050.getAngleZ();
  degisim = degisim +abs(x_eks2 - x_eks1);
  degisim = degisim +abs(y_eks2 - y_eks1);
  degisim = degisim +abs(z_eks2 - z_eks1);
  Serial.print("degisim : ");
  Serial.println(degisim);

  if(degisim>1)
  {
    digitalWrite(buzzer, HIGH);
    }
    else{
    digitalWrite(buzzer, LOW); 
     }  
}
