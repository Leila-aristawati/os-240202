# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi
**Semester**: Genap / Tahun Ajaran 2024–2025
**Nama**: `Leila Aristawati`
**NIM**: `240202901`
**Modul yang Dikerjakan**:
`Modul 5 - Audit dan Keamanan Sistem (xv6-public)`

---

## 📌 Deskripsi Singkat Tugas

* **Modul 5 – Audit dan Keamanan Sistem**:
  Menambahkan mekanisme pencatatan semua system call ke dalam struktur audit log internal kernel. Hanya proses dengan PID 1 yang diizinkan membaca log menggunakan system call get_audit_log().
---

## 🛠️ Rincian Implementasi

1. Menambahkan struktur audit_entry di syscall.c

2. Memodifikasi fungsi syscall() untuk mencatat setiap pemanggilan system call

3. Menambahkan system call baru get_audit_log():

         •Implementasi: sysproc.c

        •Deklarasi: user.h, syscall.h, defs.h, usys.S

4. Menambahkan validasi agar hanya proses PID 1 yang bisa mengakses audit log

5. Menambahkan program uji audit.c

6. Menyesuaikan Makefile agar program audit dibangun otomatis

## ✅ Uji Fungsionalitas

audit: untuk menguji system call get_audit_log() dan memastikan hanya bisa diakses oleh proses dengan PID 1

---

## 📷 Hasil Uji

Lampirkan hasil uji berupa screenshot atau output terminal. Contoh:

### 📍 Contoh Output  `audit` saat dijalankan sebagai PID 1:

```
=== Audit Log ===
[0] PID=1 SYSCALL=7 TICK=1
[1] PID=1 SYSCALL=15 TICK=3
[2] PID=1 SYSCALL=17 TICK=3
[3] PID=1 SYSCALL=15 TICK=5
[4] PID=1 SYSCALL=10 TICK=5
[5] PID=1 SYSCALL=10 TICK=5
[6] PID=1 SYSCALL=7 TICK=5
[7] PID=1 SYSCALL=28 TICK=6
```

Jika ada screenshot:

```
![hasil cowtest](./screenshots/cowtest_output.png)
```

---

## ⚠️ Kendala yang Dihadapi

1. Undefined Reference Error
Fungsi seperti memset, strcpy, stat, dll. tidak dikenali saat linking karena belum terdefinisi atau belum ditambahkan di ulib.c dan user.h.
2. Konflik Tipe Data
Perbedaan tipe parameter (misal int vs uint) antara deklarasi dan definisi menyebabkan error saat kompilasi.
3. Struct Tidak Dikenali
struct stat tidak dikenali di ulib.c karena belum meng-include stat.h.
4. Macro SYS_xxx Tidak Ditemukan
Karena #include "syscall.h" belum ditambahkan di ulib.c.
5. File Object Tidak Terbentuk
Error di ulib.c menyebabkan ulib.o tidak dibuat, dan program lain gagal saat build.
6. Akses Audit Ditolak
Program audit harus dijalankan oleh proses PID 1 agar bisa mengakses log.
---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---

