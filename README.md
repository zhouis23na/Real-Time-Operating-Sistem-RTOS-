**Nama  :** Akmal Rafiuddin  
**NRP   :** 3223600041  
**Kelas :** 3 - D4 Teknik Komputer B  

---

# RTOS-Task 2 — Multitasking pada ESP32-S3 (PlatformIO)

## Deskripsi
Program ini mendemonstrasikan penggunaan **FreeRTOS multitasking** pada mikrokontroler **ESP32-S3** menggunakan **PlatformIO di Visual Studio Code**.  
Setiap peripheral dikendalikan melalui **task terpisah** yang dijalankan secara **paralel di dua core ESP32-S3**.

Perangkat yang digunakan dalam proyek ini:
- OLED Display (SSD1306)
- LED
- Push Button
- Buzzer
- Servo Motor
- Stepper Motor (A4988)
- Potensiometer
- Rotary Encoder

---

## Fitur Task
| Task Name | Peripheral | Core | Deskripsi |
|------------|-------------|------|------------|
| `TaskOLED` | OLED SSD1306 | Core 0 | Menampilkan teks "Hello World" pada OLED |
| `TaskBlink` | LED1–LED3 | Core 0 | Menyalakan LED secara bergantian |
| `TaskPushButton` | Button, LED4, LED5 | Core 1 | Mengubah status LED ketika tombol ditekan |
| `TaskBuzzer` | Buzzer | Core 0 | Mengeluarkan suara bergantian |
| `TaskServo` | Servo Motor | Core 0 | Menggerakkan servo ke sudut 0°–90° |
| `TaskStepper` | Motor Stepper (A4988) | Core 0 | Memutar stepper bolak-balik |
| `TaskPotensio` | Potensiometer | Core 1 | Membaca nilai ADC dan menampilkan hasil di Serial Monitor |
| `TaskEncoder` | Rotary Encoder | Core 1 | Membaca posisi putaran encoder dan menampilkannya di Serial Monitor |

---

## Kebutuhan Hardware
| Komponen | Keterangan |
|-----------|-------------|
| ESP32-S3 | Board utama |
| OLED SSD1306 (I2C) | Resolusi 128x64, alamat 0x3C |
| LED x5 | Output indikator |
| Push Button | Input digital |
| Buzzer | Output suara |
| Potensiometer | Input analog (ADC) |
| Servo Motor | Motor posisi (PWM) |
| A4988 Driver + Stepper | Motor putar presisi |
| Rotary Encoder | Input posisi (interrupt) |

---

## Wiring Dasar

| Peripheral | Pin ESP32-S3 | Keterangan |
|-------------|--------------|------------|
| OLED SDA | GPIO 7 | I2C Data |
| OLED SCL | GPIO 6 | I2C Clock |
| LED1 | GPIO 4 | Output |
| LED2 | GPIO 5 | Output |
| LED3 | GPIO 8 | Output |
| LED4 | GPIO 13 | Output |
| LED5 | GPIO 16 | Output |
| Push Button | GPIO 17 | Input Pull-Up |
| Buzzer | GPIO 9 | Output |
| Potensiometer | GPIO 1 | Analog Input |
| Servo | GPIO 10 | PWM Output |
| Stepper STEP | GPIO 18 | Output ke A4988 |
| Stepper DIR | GPIO 19 | Output ke A4988 |
| Encoder A | GPIO 36 | Input Interrupt |
| Encoder B | GPIO 37 | Input Interrupt |

> **Catatan:** Pin `ENABLE` pada A4988 dihubungkan ke **GND** agar aktif.

---

## Library yang Digunakan
Pastikan library berikut sudah ada di **PlatformIO** (`platformio.ini`) atau diinstal secara manual:

## Cara Setup di VS Code (PlatformIO)

### 1️⃣ Instalasi PlatformIO
1. Buka **Visual Studio Code**  
2. Pergi ke **Extensions** → cari **“PlatformIO IDE”** → klik **Install**  
3. Setelah instalasi selesai, **restart VS Code**

---

### 2️⃣ Buat Proyek Baru
1. Klik ikon **PlatformIO (logo alien)** di sidebar kiri  
2. Pilih **New Project**  
3. Isi data proyek:
   - **Project Name:** `RTOS_Task2`
   - **Board:** `ESP32-S3 DevKitC-1`
   - **Framework:** `Arduino`
4. Klik **Finish** dan tunggu hingga setup selesai

---

### 3️⃣ Tambahkan Library
1. Buka file `platformio.ini` di root folder proyek  
2. Tambahkan baris berikut di bagian bawah file:

```ini
[env:esp32-s3-devkitc-1]
platform = espressif32 @6.7.0
board = esp32-s3-devkitc-1
framework = arduino
lib_deps = 
	Wire
    ESP32Servo
	adafruit/Adafruit GFX Library@^1.12.3
	adafruit/Adafruit SSD1306@^2.5.15
	arduino-libraries/Servo@^1.3.0
	madhephaestus/ESP32Servo@^3.0.9
```
3. Tekan Ctrl + S untuk menyimpan konfigurasi
4. PlatformIO akan otomatis mengunduhmlibrary tersebut saat kompilasi

---

### 4️⃣ Tambahkan Kode Program
- Masuk ke folder src/
- Buka file main.cpp
- Ganti seluruh isi file dengan kode program RTOS kamu (kode di atas)
- Simpan dengan Ctrl + S

---

### 5️⃣ Upload ke Board
- Sambungkan ESP32-S3 ke komputer via kabel USB
- Pastikan port sudah terdeteksi (lihat di kanan bawah VS Code, misal: COM3)
- Klik ikon (→) atau pilih Upload
- Tunggu hingga proses kompilasi dan upload selesai tanpa error

---

### 6️⃣ Buka Serial Monitor
- Klik ikon Monitor (colokan) di bagian bawah VS Code
- Pastikan baud rate = 115200
- Amati hasil multitasking yang muncul di Serial Monitor dan perangkat:
LED berkedip bergantian

✅ OLED menampilkan “Hello World”

✅ Tombol mengubah status LED4 & LED5

✅ Buzzer berbunyi bergantian

✅ Servo bergerak 0°–90°

✅ Stepper berputar dua arah

✅ Serial Monitor menampilkan nilai potensiometer & posisi encoder

---

## Dokumentasi Hasil Percobaan


https://github.com/user-attachments/assets/55fa8429-c3e7-4a14-b4de-a7bb1461bb57

