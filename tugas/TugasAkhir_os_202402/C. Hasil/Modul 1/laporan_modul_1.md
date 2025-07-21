# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi
**Semester**: Genap / Tahun Ajaran 2024–2025
**Nama**: `<Leila Aristawati>`
**NIM**: `<240202901>`
**Modul yang Dikerjakan**:
`(Modul 1 – System Call dan Instrumentasi Kernel)`

---

## 📌 Deskripsi Singkat Tugas

Tuliskan deskripsi singkat dari modul yang Anda kerjakan. Misalnya:

* **Modul 1 – System Call dan Instrumentasi Kernel**:
  Menambahkan dua system call baru, yaitu `getpinfo()` untuk melihat proses yang aktif dan `getReadCount()` untuk menghitung jumlah pemanggilan `read()` sejak boot.
---

## 🛠️ Rincian Implementasi

Tuliskan secara ringkas namun jelas apa yang Anda lakukan:

### Contoh untuk Modul 1:

* Menambahkan dua system call baru di file `sysproc.c` dan `syscall.c`
* Mengedit `user.h`, `usys.S`, dan `syscall.h` untuk mendaftarkan syscall
* Menambahkan struktur `struct pinfo` di `proc.h`
* Menambahkan counter `readcount` di kernel
* Membuat dua program uji: `ptest.c` dan `rtest.c`
---

## ✅ Uji Fungsionalitas

Tuliskan program uji apa saja yang Anda gunakan, misalnya:

1. Program ptest – Menguji getpinfo() Program ini memanggil getpinfo() dan mencetak daftar proses aktif di sistem. Data yang ditampilkan meliputi PID, ukuran memori, dan nama proses. Hasil pengujian menunjukkan informasi proses berhasil ditampilkan dengan benar.
2. Program rtest – Menguji getreadcount() Program ini memanggil getreadcount() sebelum dan sesudah fungsi read() dijalankan. Tujuannya untuk memverifikasi apakah counter readcount meningkat sesuai jumlah pemanggilan read(). Hasilnya, nilai readcount bertambah 1 setelah pemanggilan read(), menandakan counter bekerja dengan benar

---

## 📷 Hasil Uji

Lampirkan hasil uji berupa screenshot atau output terminal. Contoh:

### 📍 Contoh Output `ptest`:

```
PID     MEM      NAME
1       12288    init
2       16384    sh
3       12288    ptest
```

### 📍 Contoh Output `rtest`:

```
Read Count Sebelum: 12
hello
Read Count Setelah: 13
```

## ⚠️ Kendala yang Dihadapi

Tuliskan kendala (jika ada), misalnya:

1. Redefinisi Fungsi main()
Masalah: Terjadi error redefinition of 'main' saat kompilasi.
Penyebab: Dua program (getpinfo_test dan priority_test) diletakkan dalam satu file .c (ptest.c), masing-masing punya main().
getpinfo_test.c
priority_test.c
2. Duplikasi Nama Program di Makefile
Masalah: Nama _ptest muncul dua kali di variabel UPROGS pada Makefile.
Dampak: Berpotensi menyebabkan konflik atau hasil build yang tidak konsisten.
3. Penambahan Program ke Makefile Tidak Lengkap
Masalah: Program getpinfo_test dan priority_test tidak otomatis ter-compile.
Penyebab: Belum ditambahkan ke daftar UPROGS pada Makefile.
4. Kebingungan Letak Penambahan Rule di Makefile
Masalah: Bingung menambahkan:
getpinfo_test: getpinfo_test.c
	$(CC) getpinfo_test.c -o getpinfo_test
5. Potensi Error Nama File Salah atau Tidak Ditemukan
Masalah Potensial: Bila file getpinfo_test.c atau priority_test.c tidak berada di lokasi yang benar atau salah penamaan, akan muncul error file not found.

---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---

