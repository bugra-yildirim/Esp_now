#include <Preferences.h>//zaten yüklü gelir yüklemeye gerek yok

Preferences preferences_nesnesi;//verileri saklamak için bir preferences nesnesi oluşturmalıyız

//gerekli led ve buton pinlerini tanımlayalım
const int butonPini = 4;    
const int ledPini = 5;      

//durum takibi için gerekli değişkenler
bool ledDurumu;         
bool butonDurumu;             
int sonButonDurumu = LOW;

//butona basarken yaşanabilen birden fazla okumayı engellemek için gerekli değişkenler.
unsigned long sonBasmaZamani = 0;  // pin durumunun son değiştiği zaman
unsigned long basmaGecikmesi = 50;    // birden fazla algılamaması için gerekli gecikme değeri.

void setup() { 
  Serial.begin(115200);

  //oluşturduğumuz nesne içinde "dijitalpin" adında bir veri alanı tanımlıyoruz. ismi max 15 karakter olabilir.
  preferences_nesnesi.begin("dijitalpin", false); //false demek hem okuma hem yazma izni vermek demektir.

 //GPIO pinlerimizin türünü tanımlamalıyız
  pinMode(butonPini, INPUT);
  pinMode(ledPini, OUTPUT);

  // LED'in son bilgisini FLASH memory'den okuyoruz. Mevcutta böyle bir değişken yoksa zaten FALSE döner. (mesela ilk çalışmada)
  ledDurumu = preferences_nesnesi.getBool("state", false); //çekilen veritipi boolean - yazılan da aynı tip olmalı
  Serial.printf("LED'in son durumu: %d \n", ledDurumu);
  // okunan son durum bilgisine göre LED'i yak veya söndür
  digitalWrite(ledPini, ledDurumu);
}

void loop() {
  int butonOkunan = digitalRead(butonPini); //butonun durumunu takip ediyoruz.

//////////////bu bölüm bir basışta birden fazla okuma algılamaması için//////////////////
  if (butonOkunan != sonButonDurumu) { //değişiklik varsa son basma zamanına bakıyor eğer basma gecikmesi kadar zaman geçmemişse kaale almıyor geçmişse değişiklik var mı diye bakıyor
    sonBasmaZamani = millis();
  }
  if ((millis() - sonBasmaZamani) > basmaGecikmesi) {
    if (butonOkunan != butonDurumu) {
      butonDurumu = butonOkunan;
      if (butonDurumu == HIGH) {
        ledDurumu = !ledDurumu;
      }
    }
  }
  sonButonDurumu = butonOkunan; //son buton durumu bilgisi güncelleniyor.
////////////////////////////////////////////

///////LED'in yanıp sönme durumundaki değişiklik serial ekrandan bildiriliyor ve LED pini uygun duruma çekiliyor.
  if (digitalRead(ledPini)!= ledDurumu) {  
    Serial.println("LED'in durumu değişti");
    // yeni durumu uygula
    digitalWrite(ledPini, ledDurumu);
    
    // yeni durumu FLASH hafızaya yazıyoruz. Dikkat edelim veri tipi okunan veri tipi ile aynı olmalı.
    preferences_nesnesi.putBool("state", ledDurumu);
    
    Serial.printf("Durum FLASH hafızaya kaydedildi: %d \n", ledDurumu);
  }
}
