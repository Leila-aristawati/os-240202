# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi
**Semester**: Genap / Tahun Ajaran 2024–2025
**Nama**: `Leila Aristawati`
**NIM**: `240202901`
**Modul yang Dikerjakan**:
`Modul 2 – Penjadwalan CPU Lanjutan (Priority Scheduling Non-Preemptive `

---

## 📌 Deskripsi Singkat Tugas

Tuliskan deskripsi singkat dari modul yang Anda kerjakan. Misalnya:

* **Modul 2 – Penjadwalan CPU Lanjutan (Priority Scheduling Non-Preemptive**:
  Modul 2 – Penjadwalan CPU Lanjutan (Priority Scheduling Non-Preemptive): Modul ini bertujuan untuk memodifikasi algoritma penjadwalan proses default xv6 (Round Robin) menjadi Non-Preemptive Priority Scheduling. Perubahan utama meliputi:

1. Menambahkan field priority di setiap proses

2. Menambahkan system call set_priority(int)

3. Memodifikasi fungsi scheduler() untuk mengeksekusi proses RUNNABLE dengan prioritas tertinggi (nilai numerik paling kecil)
---

## 🛠️ Rincian Implementasi

Tuliskan secara ringkas namun jelas apa yang Anda lakukan:
Modifikasi yang Dilakukan: 
A. Tambahan Field priority di struct proc
File: proc.h
B. Scheduler Menggunakan Prioritas
File: proc.c
Modifikasi fungsi scheduler() agar memilih proses RUNNABLE dengan prioritas tertinggi (nilai paling kecil)
C. Menambahkan Syscall setpriority()
File: proc.c
D. Program Uji ptest.c
File: ptest.c
E. Hasil yang Diharapkan
Saat menjalankan ptest, output-nya harus menunjukkan proses dengan prioritas lebih tinggi selesai lebih dulu.

`
---

## ✅ Uji Fungsionalitas

Program uji yang digunakan:

ptest: menguji apakah proses dengan prioritas lebih tinggi dijalankan lebih dulu

Proses anak 1: set_priority(90)

Proses anak 2: set_priority(10)

Output menunjukkan urutan eksekusi berdasarkan prioritas
---

## 📷 Hasil Uji

Lampirkan hasil uji berupa screenshot atau output terminal. Contoh:

### 📍 Contoh Output `ptest`:

```
Child 2 selesai
Child 1 selesai
Parent selesai
```
Jika ada screenshot:

```
![hasil cowtest](./screenshots/cowtest_output.png)
```

---

## ⚠️ Kendala yang Dihadapi

1. Variabel proc Tidak Terdefinisi
Masalah: Saat menambahkan proc = p; di dalam scheduler(), muncul error:
error: ‘proc’ undeclared (first use in this function)
Penyebab: Variabel global proc tidak tersedia secara langsung di scheduler(), karena dalam desain xv6 proc adalah bagian dari struct cpu.
2. Lupa Mengatur Prioritas Awal Proses
Masalah: Proses default tetap menggunakan prioritas acak atau nol.
3. Panic di Fungsi sleep()
Masalah: Kernel panic muncul dengan pesan:
panic: sleep
Penyebab: Fungsi sleep() dipanggil tanpa memastikan proc sudah terdefinisi, atau dipanggil sebelum proses siap.
4. Urutan Proses Tidak Sesuai Harapan
Masalah: Meski sudah set priority, proses tetap tidak dijalankan sesuai urutan prioritas.
Penyebab: Fungsi scheduler() belum benar-benar memilih proses berdasarkan prioritas.
5. Kesulitan Verifikasi Hasil
Masalah: Output hanya menampilkan Child selesai, sulit membuktikan bahwa scheduler memang menjalankan proses sesuai prioritas.
6. Build Error setelah Menambah Syscall
Masalah: Setelah menambah setpriority(), muncul error undefined reference to 'setpriority'.
Penyebab: Lupa mendaftarkan syscall di beberapa tempat.
---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---

