
https://github.com/user-attachments/assets/b93c024e-d355-4cf0-a410-885a0c05196c
Berikut versi **parafrase** dari README kamu dalam format **Markdown (.md)** dengan gaya yang tetap teknis, rapi, dan mudah dipahami 👇

---

````markdown
# 🧠 Program Multitasking Peripheral ESP32-S3 (Uji Prioritas Task)

Dokumentasi ini menjelaskan bagaimana **ESP32-S3** menjalankan berbagai *peripheral* secara **paralel** menggunakan **FreeRTOS dual-core**.  
Eksperimen ini juga membahas **pengaruh pengaturan prioritas task** terhadap stabilitas sistem, serta disertai **video demo** di akhir bagian.

---

## ✅ 1. Deskripsi Proyek

Proyek ini menguji kemampuan ESP32-S3 dalam menangani beberapa *peripheral* sekaligus menggunakan multitasking. Semua komponen dijalankan dalam sistem FreeRTOS dan dibagi ke dua core.

**Peripheral yang digunakan:**
- 3 LED
- 2 Tombol
- OLED SSD1306
- Servo (PWM)
- Stepper (AccelStepper)
- Encoder
- Potensiometer
- Buzzer (PWM manual)

---

## ✅ 2. Pembagian Task per Core

| Core 0 | Core 1 |
|--------|--------|
| LED1   | Servo  |
| LED2   | Buzzer |
| LED3   | Potensiometer |
| Button1 | Encoder |
| Button2 | Stepper |
| OLED Display |   |

---

## ✅ 3. Percobaan 1 – Semua Prioritas Sama (Nilai = 1)

Pada percobaan pertama, seluruh task diberi **prioritas yang sama (1)**.  
Konfigurasi ini membuat semua peripheral dapat berjalan dengan baik dan stabil.

Contoh kode:
```cpp
xTaskCreatePinnedToCore(TaskBuzzer, "BUZZER", 2048, NULL, 1, NULL, 1);
````

**Hasil pengamatan:**

* ✅ Buzzer berjalan normal
* ✅ Stepper, encoder, servo, dan potensiometer tetap aktif
* ✅ Semua peripheral bekerja serempak tanpa gangguan visual

---

## ❌ 4. Percobaan 2 – Prioritas POT dan SERVO = 10

Pada percobaan kedua, **task Potensiometer dan Servo** diberikan **prioritas tinggi (10)** untuk menguji dampaknya terhadap task lain.

Contoh kode:

```cpp
xTaskCreatePinnedToCore(TaskPot, "POT", 2048, NULL, 10, NULL, 1);
xTaskCreatePinnedToCore(TaskServo, "SERVO", 4096, NULL, 10, NULL, 1);
```

**Hasil pengamatan:**

* ⚠️ Stepper mulai melambat atau berhenti
* ⚠️ Encoder tidak terbaca dengan stabil
* ⚠️ Task di Core 1 menjadi kurang responsif
* ⚠️ Beberapa peripheral tampak “mati”

---

## ⚠️ 5. Analisis Masalah

Terdapat dua faktor utama penyebab gangguan:

### 1. Task Potensiometer dan Servo memonopoli CPU

Keduanya dijalankan terus-menerus dalam loop aktif tanpa `vTaskDelay()` yang cukup, sehingga:

* CPU tidak sempat memberikan waktu ke task lain (*no yield time*).
* FreeRTOS kesulitan menjadwalkan task dengan prioritas lebih rendah.

### 2. Prioritas Terlalu Tinggi

Nilai prioritas `10` terlalu besar dibanding task lain (`1`), akibatnya:

* Scheduler selalu memilih task prioritas tinggi.
* Task lain seperti stepper, buzzer, dan encoder tidak mendapat jatah CPU.

---

## ✅ 6. Kesimpulan

| Percobaan | Konfigurasi                | Hasil                                            |
| --------- | -------------------------- | ------------------------------------------------ |
| 1         | Semua task prioritas = 1   | ✅ Semua peripheral berjalan stabil               |
| 2         | POT & SERVO prioritas = 10 | ❌ Beberapa peripheral berhenti / tidak responsif |

**Kesimpulan akhir:**

* Sistem multitasking FreeRTOS membutuhkan **penyeimbangan prioritas** agar tidak terjadi blok CPU.
* Gunakan `vTaskDelay()` atau bentuk *non-blocking delay* agar setiap task mendapat jatah waktu eksekusi.
* Prioritas tinggi sebaiknya hanya digunakan untuk task yang benar-benar *critical*.

---

🎥 **Demo Video:**
Percobaan 1


https://github.com/user-attachments/assets/6e0a4aa5-566f-4fed-bc55-a15e5837d1e6

Percobaan 2


https://github.com/user-attachments/assets/e0664e42-88cf-469a-aa9c-f0bcfce21c87


```
