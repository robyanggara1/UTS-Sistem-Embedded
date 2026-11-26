
#include <DHT.h>
#include <Servo.h>

// ======================================
// UTS SISTEM EMBEDDED - SOAL 2
// Nama: Roby Anggara
// NIM: 2510614023
// BATAS_JARAK = 23 cm
// SUHU_ATAS = 23 C
// SUHU_BAWAH = 20 C
// ======================================

// Pin komponen
#define POT A0
#define LED_HIJAU 2
#define LED_KUNING 3
#define LED_MERAH 4
#define BUZZER 5

#define TRIG 6
#define ECHO 7
#define DHTPIN 8
#define SERVO_PIN 9
#define RELAY 10
#define LED_PENDINGIN 11

#define DHTTYPE DHT22

// Parameter NIM
const int BATAS_JARAK = 23;
const float SUHU_ATAS = 23.0;
const float SUHU_BAWAH = 20.0;

DHT dht(DHTPIN, DHTTYPE);
Servo pintu;

bool pendinginAktif = false;
bool statusKritisSebelumnya = false;

int kapasitas = 0;
float suhu = 0;
float kelembapan = 0;
float jarak = 0;

void bacaSensor() {
  int nilaiADC = analogRead(POT);
  kapasitas = map(nilaiADC, 0, 1023, 0, 100);

  long durasi;
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  durasi = pulseIn(ECHO, HIGH, 30000);

  if (durasi == 0) {
    jarak = 400;
  } else {
    jarak = durasi * 0.0343 / 2;
  }

  float bacaSuhu = dht.readTemperature();
  float bacaKelembapan = dht.readHumidity();

  if (!isnan(bacaSuhu)) suhu = bacaSuhu;
  if (!isnan(bacaKelembapan)) kelembapan = bacaKelembapan;
}

void updateIndikator() {
  digitalWrite(LED_HIJAU, LOW);
  digitalWrite(LED_KUNING, LOW);
  digitalWrite(LED_MERAH, LOW);

  if (kapasitas < 60) {
    digitalWrite(LED_HIJAU, HIGH);
  } 
  else if (kapasitas < 90) {
    digitalWrite(LED_KUNING, HIGH);
  } 
  else {
    digitalWrite(LED_MERAH, HIGH);

    if (!statusKritisSebelumnya) {
      for (int i = 0; i < 3; i++) {
        tone(BUZZER, 1000, 200);
        delay(300);
      }
      noTone(BUZZER);
    }
  }

  statusKritisSebelumnya = (kapasitas >= 90);
}

void kontrolPintu() {
  if (jarak < BATAS_JARAK && kapasitas >= 90) {
    pintu.write(0);
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_MERAH, HIGH);

    tone(BUZZER, 1000);
    delay(1000);
    noTone(BUZZER);
  } 
  else if (jarak < BATAS_JARAK && kapasitas < 90) {
    pintu.write(90);
  } 
  else {
    pintu.write(0);
  }
}

void kontrolPendingin() {
  if (suhu >= SUHU_ATAS) {
    pendinginAktif = true;
  } 
  else if (suhu <= SUHU_BAWAH) {
    pendinginAktif = false;
  }

  digitalWrite(RELAY, pendinginAktif ? HIGH : LOW);
  digitalWrite(LED_PENDINGIN, pendinginAktif ? HIGH : LOW);
}

void tampilkanSerial() {
  Serial.println("======================");
  Serial.print("Kapasitas Rak: ");
  Serial.print(kapasitas);
  Serial.println("%");

  Serial.print("Jarak Forklift: ");
  Serial.print(jarak);
  Serial.println(" cm");

  Serial.print("Suhu: ");
  Serial.print(suhu);
  Serial.println(" C");

  Serial.print("Kelembapan: ");
  Serial.print(kelembapan);
  Serial.println("%");

  Serial.print("Pintu: ");
  Serial.println(pintu.read() == 90 ? "TERBUKA" : "TERTUTUP");

  Serial.print("Pendingin: ");
  Serial.println(pendinginAktif ? "AKTIF" : "MATI");
}

void setup() {
  Serial.begin(9600);

  pinMode(POT, INPUT);
  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LED_PENDINGIN, OUTPUT);

  pintu.attach(SERVO_PIN);
  pintu.write(0);

  dht.begin();

  digitalWrite(RELAY, LOW);
  digitalWrite(LED_PENDINGIN, LOW);

  Serial.println("SISTEM INDIKATOR KAPASITAS RAK");
  Serial.println("UTS SOAL 2");
}

void loop() {
  bacaSensor();
  updateIndikator();
  kontrolPintu();
  kontrolPendingin();
  tampilkanSerial();

  delay(1000);
}
