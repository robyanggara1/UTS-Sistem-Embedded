#include <Arduino.h>

/*
  UTS SISTEM EMBEDDED
  Nama : Roby Anggara
  NIM  : 2510614023

  BATAS_JARAK = 23 cm
  SUHU_ATAS   = 23 C
  SUHU_BAWAH  = 20 C

  SOAL 1 - PANEL INDIKATOR KAPASITAS RAK
*/

#define PIN_POT A0

#define LED_HIJAU 2
#define LED_KUNING 3
#define LED_MERAH 4
#define PIN_BUZZER 5

int kapasitasSebelumnya = 0;

// Fungsi membaca kapasitas rak
int bacaKapasitas() {
  int nilaiADC = analogRead(PIN_POT);

  int persen = map(nilaiADC, 0, 1023, 0, 100);

  return persen;
}

// Fungsi membunyikan buzzer 3 kali
void bunyiBuzzer() {
  for (int i = 0; i < 3; i++) {
    tone(PIN_BUZZER, 1000);
    delay(200);
    noTone(PIN_BUZZER);
    delay(200);
  }
}

// Fungsi mengatur indikator LED
void updateIndikator(int persen) {

  if (persen < 60) {
    digitalWrite(LED_HIJAU, HIGH);
    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_MERAH, LOW);

    Serial.println("Status: LONGGAR");
  }

  else if (persen <= 89) {
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_KUNING, HIGH);
    digitalWrite(LED_MERAH, LOW);

    Serial.println("Status: WASPADA");
  }

  else {
    digitalWrite(LED_HIJAU, LOW);
    digitalWrite(LED_KUNING, LOW);
    digitalWrite(LED_MERAH, HIGH);

    Serial.println("Status: KRITIS");

    if (kapasitasSebelumnya < 90) {
      bunyiBuzzer();
    }
  }

  kapasitasSebelumnya = persen;
}

void setup() {
  Serial.begin(9600);

  pinMode(LED_HIJAU, OUTPUT);
  pinMode(LED_KUNING, OUTPUT);
  pinMode(LED_MERAH, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  Serial.println("SISTEM INDIKATOR KAPASITAS RAK");
}

void loop() {
  int kapasitas = bacaKapasitas();

  Serial.print("Kapasitas Rak: ");
  Serial.print(kapasitas);
  Serial.println("%");

  updateIndikator(kapasitas);

  Serial.println("----------------------");

  delay(500);
}
