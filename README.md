# YİBOT: ÇOK KADEMELİ VE REAKSİYONEL ÇEVRE ANALİZ SİSTEMİ

Bu depo, bağımsız bir çoklu sensör masaüstü asistan prototipi olarak geliştirilen **YiBot V4.1** gömülü robotik platformunun yazılım kaynak kodlarını ve yapısal mimari loglarını barındırmaktadır.

## 🚀 Yapısal Mimari ve Pin Eşleşmeleri
Sistemin donanım altyapısı, bloklamasız ve asenkron bir sonlu durum makinesi (FSM) çalıştıran Wi-Fi destekli NodeMCU (ESP8266) ana çekirdeği üzerine kurulmuştur. Endüktif yüklerden kaynaklanan geçici elektromanyetik parazitleri ve akım çökmelerini önlemek adına tüm elektriksel hatlar optimize edilmiştir:

* **Ana İşlemci:** NodeMCU V3 (ESP8266 Çekirdeği)
* **Aktüatörler:** SG90 Mikro Servo (GPIO4 / D2 pini üzerinden PWM kontrolü), Active-Low Sinyal LED'leri ve Senkron Akustik Piezo Buzzer Düzenekleri.
* **Sensör Matrisi:** DHT22 Mikroklimatik Ortam Analizörü (GPIO13 / D7), Ultrasonik Mesafe Sensörü (HC-SR04) ve Analog Işık Sensörü (LDR / A0).

---

## ⚙️ Çekirdek Algoritma Mantığı (Durum Makinesi Hiyerarşisi)

Gömülü yazılım, kesintisiz ana döngüsü içerisinde çevresel telemetri verilerini birbiriyle çakışmayan hiyerarşik bir mantık sırasına göre işlemektedir:
[BAŞLANGIÇ: Çoklu Sensör Veri Okuması]
|
+---> Sıcaklık > 33°C? ------> EVET: [SEVİYE 5: TERMAL ACİL DURUM ALARMI]
|       | HAYIR
|       v
+---> Mesafe < 12 cm? ---------> EVET: [SEVİYE 4: KRİTİK YAKINLIK MODU]
|       | HAYIR
|       v
+---> Işık Yoğunluğu < 250? ----> EVET: [SEVİYE 3: KARANLIK / KORKU REAKSİYONU]
|       | HAYIR
|       v
+---> Mesafe 12-30 cm? --------> EVET: [SEVİYE 2: ERKEN UYARI VE TARAMA]
| HAYIR
v
[SEVİYE 1: STABİL DURUM / GÜVENLİ RADAR TARAMASI]
## 🛠️ Deneysel Optimizasyon Özetleri
Donanımsal stres testleri ve prototiplendirme aşamalarında karşılaşılan kritik gömülü sistem zafiyetleri, uygulanan mühendislik çözümleriyle tamamen giderilmiştir:

1. **Active-Low Sinyalizasyon Ayarı:** Sinyal LED'lerinin ters mantık (sink akımı) mimarisinde çalışmasından kaynaklanan çelişki, fiziksel devre bozulmadan yazılım katmanında lojik komutlar tersine çevrilerek çözülmüştür.
2. **Geçici Voltaj Düşüşlerinin (Brownout) Önlenmesi:** Servo motorun kalkış anında yarattığı ani akım piklerinin NodeMCU'yu resetlemesi; besleme kablosu kalınlaştırılarak ve kod blokları arasına 70 ms'lik asenkron mikro geciktirme filtreleri eklenerek engellenmiştir.
3. **Mikroklimatik Isı Tuzağı İzolasyonu:** Kapalı kutu hacminde işlemci ve bataryanın yaydığı ısının DHT22'yi yanıltması; sensör gövdesi oda atmosferine çıkarılarak ve yazılımsal yangın alarmı eşiği Ankara iklim koşullarına uygun olarak 33.0°C seviyesine kalibre edilerek çözülmüştür.
4. **Mekanik Gövde ve Hacim Düzenlemesi:** 10 cm derinliğindeki muhafaza kutusunda jumper kabloların sıkışması ve taşması; kutu tabanına özel bir yükseltici tampon platform inşa edilerek ve kablolar yan tarafa preslenerek kontrol altına alınmıştır.

---
*Not: Ankara Üniversitesi Akademik Değerlendirme Kriterleri doğrultusunda hazırlanmıştır. Çift Kör Hakem (Masked Peer-Review) süreçlerine uyum amacıyla kişisel kimlik bilgileri dökümandan arındırılmıştır.*
