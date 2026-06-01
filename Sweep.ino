#include <Servo.h>
#include <DHT.h> 

Servo servoSagSolD2;       

const int trigPin = 14;     // D5 pini (Mesafe TRIG)
const int echoPin = 12;     // D6 pini (Mesafe ECHO)
const int ldrPin = A0;      // LDR pini (Işık Sensörü)

// Yiğit'in Efsane Düzeni (LOW = AÇIK, HIGH = KAPALI)
const int yesilLed = 16;    // D0 pini
const int buzzer1 = 5;      // D1 pini (Küçük Bazır)
const int sariLed = 0;      // D3 pini
const int kirmiziLed = 2;   // D4 pini
const int buzzer2 = 15;     // D8 pini (Büyük Bazır)

#define DHTPIN 13           // D7 pini (DHT22 Sıcaklık Sensörü)
#define DHTTYPE DHT22       
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(500); 
  
  servoSagSolD2.attach(D2); 
  
  pinMode(trigPin, OUTPUT); pinMode(echoPin, INPUT); pinMode(ldrPin, INPUT);
  pinMode(yesilLed, OUTPUT); pinMode(sariLed, OUTPUT); pinMode(kirmiziLed, OUTPUT);
  pinMode(buzzer1, OUTPUT); pinMode(buzzer2, OUTPUT);
  
  dht.begin();
  
  // Başlangıç testi (Açılışta her şey kapanır, ters mantık HIGH = KAPALI)
  digitalWrite(buzzer1, HIGH); digitalWrite(buzzer2, HIGH);
  digitalWrite(yesilLed, HIGH); digitalWrite(sariLed, HIGH); digitalWrite(kirmiziLed, HIGH);
  
  servoSagSolD2.write(90); // Kafayı tam karşıya al, düz baksın
  Serial.println("--- YIGIT ROBOTIK V4.1 SAKIN MOD AKTIF ---");
}

void loop() {
  // 1. Sensörleri Okuma İşlemleri
  long sure, mesafe;
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  sure = pulseIn(echoPin, HIGH, 26000); 
  mesafe = (sure / 2) / 29.1; 
  
  int isikDegeri = analogRead(ldrPin); 
  float sicaklik = dht.readTemperature(); 
  float nem = dht.readHumidity();
  
  // Sensör hata verirse kart kilitlenmesin diye koruma
  if (isnan(sicaklik)) {
    sicaklik = 24.5; 
  }
  
  // Bilgisayardan izlemek istersen Canlı Seri Port Yayını
  Serial.print("Mesafe: "); Serial.print(mesafe); Serial.print(" cm | ");
  Serial.print("Işık: "); Serial.print(isikDegeri); Serial.print(" | ");
  Serial.print("Sıcaklık: "); Serial.print(sicaklik); Serial.println(" *C");
  
  // ==================== AKILLI SENARYOLAR VE TEPKİLER ====================

  // SEVİYE 5: YANGIN VEYA AŞIRI SICAKLIK ALARMI (İstediğin gibi 33 dereceye yükseltildi kanka!)
  if (sicaklik > 33.0) {
    digitalWrite(yesilLed, HIGH); digitalWrite(sariLed, HIGH); 
    digitalWrite(kirmiziLed, LOW); // Kırmızı LED tehlike için yanar
    
    // İki bazır birden çılgınlar gibi öter, kafa panikle sağa sola sallanır
    digitalWrite(buzzer1, LOW); digitalWrite(buzzer2, LOW); servoSagSolD2.write(20); delay(150);
    digitalWrite(buzzer1, HIGH); digitalWrite(buzzer2, HIGH); servoSagSolD2.write(160); delay(150);
  }
  
  // SEVİYE 4: KRİTİK YAKINLIK - DİREKT BURUN BURUNA GELİNCE (Mesafe < 12 cm)
  else if (mesafe > 0 && mesafe < 12) {
    digitalWrite(yesilLed, HIGH); digitalWrite(sariLed, HIGH);
    digitalWrite(kirmiziLed, LOW); // Kırmızı LED flaşör yapar
    
    // Büyük bazır (Buzzer 2) "DIIIIIIV" diye bağırır, kafa korkup sertçe yana döner
    digitalWrite(buzzer2, LOW); 
    servoSagSolD2.write(30); delay(200);
    digitalWrite(kirmiziLed, HIGH);
    servoSagSolD2.write(150); delay(200);
    digitalWrite(buzzer2, HIGH); 
  }
  
  // SEVİYE 3: KARANLIK MODU - "KORKU VE TİTREME" (Işık Değeri < 250)
  // Elini LDR'ye kapatınca robot karanlıktan korkup titreyecek!
  else if (isikDegeri < 250) {
    digitalWrite(yesilLed, HIGH); digitalWrite(kirmiziLed, HIGH);
    digitalWrite(sariLed, LOW); // Sarı LED gece lambası gibi yanar
    
    // Robot karanlıkta tir tir titrer (Kafa hızlıca 5 derece sağa sola esner)
    // Küçük bazır (Buzzer 1) ise kalp atışı gibi kesik kesik tıklar kanka
    digitalWrite(buzzer1, LOW); servoSagSolD2.write(85); delay(70);
    digitalWrite(buzzer1, HIGH); servoSagSolD2.write(95); delay(70);
    digitalWrite(buzzer1, LOW); servoSagSolD2.write(87); delay(70);
    digitalWrite(buzzer1, HIGH); servoSagSolD2.write(93); delay(70);
  }
  
  // SEVİYE 2: ERKEN UYARI - BİRİ YAKLAŞIYOR (Mesafe 12-30 cm arası)
  else if (mesafe >= 12 && mesafe < 30) {
    digitalWrite(yesilLed, HIGH); digitalWrite(kirmiziLed, HIGH); digitalWrite(buzzer2, HIGH);
    digitalWrite(sariLed, LOW); // Sarı LED açık
    
    // Küçük bazır kibarca "bip... bip..." yapar, kafa merakla yaklaşanı süzer
    digitalWrite(buzzer1, LOW); servoSagSolD2.write(60); delay(350);
    digitalWrite(buzzer1, HIGH); servoSagSolD2.write(120); delay(350);
  }
  
  // SEVİYE 1: SAKİN DURUM / HER ŞEY YOLUNDA
  else {
    digitalWrite(yesilLed, LOW); // Sadece GÜVENLİ Yeşil LED yanar
    digitalWrite(sariLed, HIGH); digitalWrite(kirmiziLed, HIGH);
    digitalWrite(buzzer1, HIGH); digitalWrite(buzzer2, HIGH); // Bütün gürültü kesilir
    
    // Kafa tam 90 derecede, asilce karşıya bakar kanka
    servoSagSolD2.write(90);      
    delay(150);
  }
}
  }
}
