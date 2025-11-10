
# 🧠 Task — Koleksi Implementasi Peripheral ESP32-S3 dengan FreeRTOS

Repository ini berisi kumpulan tugas implementasi berbagai **peripheral pada ESP32-S3** menggunakan sistem operasi **FreeRTOS**.  
Setiap proyek mencakup:

- File program utama (`.ino`)
- Diagram rangkaian (wiring)
- Video penjelasan (pengujian pada core0 / core1)

---

## 📁 Struktur Direktori

```

Task/
├── button/
├── buzzer/
├── LED/
├── motor stepper/
├── oled/
├── potensio/
├── rotary/
├── servo/
└── Gabungan semua peripheral/

````

---

## 🧩 Deskripsi Umum

Setiap folder berisi:
- `sketch.ino` — kode program utama  
- `wiring.png` — diagram sambungan rangkaian  
- `README.md` — berisi penjelasan singkat dan tautan video demonstrasi  

Folder **“Gabungan semua peripheral”** merupakan proyek integrasi dari seluruh perangkat menjadi satu sistem terkoordinasi.

---

## ⚙️ Cara Penggunaan

1. Masuk ke folder salah satu peripheral.  
2. Buka file `sketch.ino` menggunakan **WOKWI**, **Arduino IDE**, atau **PlatformIO**.  
3. Pilih board: **ESP32S3 Dev Module**.  
4. Pastikan library berikut sudah terpasang:
   - Adafruit SSD1306  
   - Adafruit GFX  
   - ESP32Servo  
   - AccelStepper  
5. Upload program ke board dan sambungkan rangkaian sesuai diagram wiring.

---

## 🧠 Kesimpulan Praktikum FreeRTOS dengan ESP32-S3

Dari percobaan yang dilakukan, diperoleh pemahaman bahwa **FreeRTOS pada ESP32-S3** mampu mengelola berbagai peripheral secara bersamaan tanpa konflik, selama pengaturan **prioritas task** dan **delay** dilakukan dengan benar.  

Setiap perangkat seperti LED, tombol, buzzer, servo, OLED, rotary encoder, hingga motor stepper dijalankan pada **task terpisah**, membuat sistem tetap responsif dan stabil.  
Penjadwalan task diatur oleh **scheduler FreeRTOS**, yang memastikan tiap perangkat bekerja sesuai waktu yang ditentukan.

Task dibuat dengan fungsi berikut:

```cpp
xTaskCreatePinnedToCore(
    taskFunction,   // fungsi yang dijalankan sebagai task
    "TaskName",     // nama task
    stackSize,      // ukuran stack
    NULL,           // parameter (opsional)
    priority,       // prioritas eksekusi
    NULL,           // handle task (opsional)
    coreID          // core tempat task dijalankan (0 atau 1)
);
````

Dengan `xTaskCreatePinnedToCore()`, setiap task dapat dikunci pada core tertentu agar tidak berpindah antar-core.
Pendekatan ini menjaga kestabilan timing, terutama untuk perangkat yang memerlukan waktu eksekusi presisi seperti **servo**, **rotary encoder**, dan **sensor real-time**.

---