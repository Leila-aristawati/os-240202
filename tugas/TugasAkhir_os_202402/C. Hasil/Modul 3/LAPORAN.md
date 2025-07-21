# 📝 Laporan Tugas Akhir

**Mata Kuliah**: Sistem Operasi
**Semester**: Genap / Tahun Ajaran 2024–2025
**Nama**: `Leila Aristawati`
**NIM**: `240202901`
**Modul yang Dikerjakan**:
`Modul 3 - Manajemen Memori Tingkat Lanjut`

---

## 📌 Deskripsi Singkat Tugas
* **Modul 3 - Manajemen Memori Tingkat Lanjut**:
 Modul 3 – Manajemen Memori Tingkat Lanjut (xv6-public x86): Modul ini berfokus pada pengembangan manajemen memori lanjutan pada xv6, meliputi dua fitur utama:
1. Copy-on-Write (CoW) pada fork() untuk menghindari duplikasi halaman memori yang tidak perlu.
2. Shared Memory ala System V yang memungkinkan proses saling berbagi 1 halaman memori melalui kunci (key) dan reference count.
---

## 🛠️ Rincian Implementasi

Menambahkan ref_count[] di vm.c untuk melacak jumlah referensi setiap halaman fisik.
Menambahkan flag PTE_COW di mmu.h.
Membuat fungsi cowuvm() yang menggantikan copyuvm() saat fork.
Menangani page fault (T_PGFLT) di trap.c untuk menyalin halaman CoW saat proses melakukan penulisan.
Membuat tabel shared memory (shmtab) dan syscall shmget() serta shmrelease() di sysproc.c.
Registrasi syscall baru di syscall.h, syscall.c, user.h, dan usys.S.

---

## ✅ Uji Fungsionalitas

Program uji yang digunakan:

1. cowtest: untuk menguji fork dengan Copy-on-Write
2. shmtest: untuk menguji shmget() dan shmrelease()

---

## 📷 Hasil Uji

### 📸 Screenshot:
![cowtest.png](./screenshots/cowtest.png
### 📸 Screenshot:
![shmtest.png](./screenshots/shmtest.png

### 📍 Contoh Output `cowtest`:

```
Child sees: Y
Parent sees: X
```

### 📍 Contoh Output `shmtest`:

```
Child reads: A
Parent reads: B
```
Jika ada screenshot:

```
![hasil cowtest](./screenshots/cowtest_output.png)
```

---

## ⚠️ Kendala yang Dihadapi
1. Belum ada logika COW
cowuvm() seharusnya tidak menyalin halaman, tapi share halaman read-only antar proses.
2. Tidak ada reference count halaman
Perlu refcount untuk tahu berapa proses memakai satu halaman fisik.
3. Page fault belum tangani COW
Harus mendeteksi page fault karena write ke halaman read-only dan melakukan salinan.
4. freevm() bisa salah bebaskan halaman
Tanpa refcount, halaman bisa dibebaskan walau masih dipakai proses lain.

---

## 📚 Referensi

Tuliskan sumber referensi yang Anda gunakan, misalnya:

* Buku xv6 MIT: [https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf](https://pdos.csail.mit.edu/6.828/2018/xv6/book-rev11.pdf)
* Repositori xv6-public: [https://github.com/mit-pdos/xv6-public](https://github.com/mit-pdos/xv6-public)
* Stack Overflow, GitHub Issues, diskusi praktikum

---

