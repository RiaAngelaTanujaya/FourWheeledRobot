#include <Ticker.h>
#include "esp_task_wdt.h"  
// Definisi pin
#define trigPin 13
#define echoPin 14
#define motorKiriMaju 25
#define motorKiriMundur 26
#define motorKananMaju 32
#define motorKananMundur 33

// Timer dan variabel sensor
Ticker timerSensor;
volatile int jarak = 100;

// Fungsi deklarasi lebih awal
void bacaSensor();
void maju();
void mundur();
void berhenti();
void belokKanan();

void setup() {
  Serial.begin(115200);

  // Inisialisasi pin
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(motorKiriMaju, OUTPUT);
  pinMode(motorKiriMundur, OUTPUT);
  pinMode(motorKananMaju, OUTPUT);
  pinMode(motorKananMundur, OUTPUT);

  // Timer interrupt untuk baca sensor tiap 100 ms
  timerSensor.attach_ms(100, bacaSensor);

  // Inisialisasi Watchdog dengan config struct
  esp_task_wdt_config_t wdt_config = {
    .timeout_ms = 5000,         // 5 detik timeout
    .idle_core_mask = 0b11,     // Core 0 dan 1
    .trigger_panic = true       // Reset jika tidak di-reset
  };
  esp_task_wdt_init(&wdt_config);  // Inisialisasi WDT
  esp_task_wdt_add(NULL);          // Tambah task utama (loop)
}

void loop() {
  // Reset watchdog agar tidak dianggap hang
  esp_task_wdt_reset();

  Serial.print("Jarak: ");
  Serial.print(jarak);
  Serial.println(" cm");

  if (jarak > 20) {
    maju();
  } else {
    berhenti();
    delay(100);
    mundur();
    delay(400);
    belokKanan();
    delay(200);
    berhenti();
    delay(100);
  }

  delay(50); // Delay utama
}

// Fungsi interrupt timer: membaca sensor ultrasonik
void bacaSensor() {
  long durasi;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  durasi = pulseIn(echoPin, HIGH, 20000); // Timeout 20ms

  int hasil = durasi * 0.034 / 2;
  if (hasil == 0 || hasil > 400) {
    hasil = 400;  // Batas maksimum agar tidak error
  }

  jarak = hasil;
}

// Fungsi kontrol motor
void maju() {
  digitalWrite(motorKiriMaju, HIGH);
  digitalWrite(motorKiriMundur, LOW);
  digitalWrite(motorKananMaju, HIGH);
  digitalWrite(motorKananMundur, LOW);
}

void mundur() {
  digitalWrite(motorKiriMaju, LOW);
  digitalWrite(motorKiriMundur, HIGH);
  digitalWrite(motorKananMaju, LOW);
  digitalWrite(motorKananMundur, HIGH);
}

void berhenti() {
  digitalWrite(motorKiriMaju, LOW);
  digitalWrite(motorKiriMundur, LOW);
  digitalWrite(motorKananMaju, LOW);
  digitalWrite(motorKananMundur, LOW);
}

void belokKanan() {
  digitalWrite(motorKiriMaju, HIGH);
  digitalWrite(motorKiriMundur, LOW);
  digitalWrite(motorKananMaju, LOW);
  digitalWrite(motorKananMundur, HIGH);
}