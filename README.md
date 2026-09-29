# Praktikum Struktur Data C++ — Pertemuan 5

## Study Case: Aplikasi Editor "Tulis"

Repository ini dibuat otomatis oleh aplikasi praktikum. Setiap kali Anda
melakukan **push**, GitHub Actions akan mengompilasi kode Anda, menjalankan
test, dan mengirim nilainya ke aplikasi.

---

# Study Case

Pertemuan ini hanya punya **satu soal**, yaitu study case di bawah ini. Empat
pekerjaan yang Anda kerjakan semuanya berasal dari cerita yang sama — tidak ada
cerita baru lagi sesudahnya.

## Latar

Anda diminta membuat bagian dalam sebuah aplikasi editor teks bernama **Tulis**.
Jendelanya sudah jadi, menunya sudah lengkap, tetapi dua fiturnya masih mati
total: tombol **Undo** tidak melakukan apa-apa, dan **pemeriksa kurung** selalu
menjawab "tidak tahu".

Yang menarik, kedua fitur itu ternyata membutuhkan bentuk penyimpanan yang sama
persis — dan itulah materi pertemuan ini: **stack**.

## Fitur pertama — tombol Undo

Setiap kali Anda mengetik sesuatu, editor mencatat perubahan itu. Ketika Anda
menekan Ctrl+Z, yang dibatalkan adalah perubahan yang **paling terakhir** Anda
lakukan — bukan yang paling awal.

Coba bayangkan kalau aturannya terbalik. Anda mengetik "halo", lalu menghapus
satu huruf, lalu menebalkan sebuah kata. Menekan Ctrl+Z seharusnya mengembalikan
penebalan tadi. Kalau yang dibatalkan justru pengetikan "halo" yang paling awal,
tulisan Anda akan berantakan.

Aturan "yang terakhir masuk, dialah yang pertama keluar" itu disebut **LIFO** —
*Last In, First Out*. Bentuk penyimpanan yang mengikuti aturan itu disebut
**stack**, atau tumpukan.

Namanya tumpukan karena persis seperti menumpuk piring: piring baru selalu
diletakkan di **atas**, dan piring yang bisa diambil juga selalu yang paling
**atas**. Tidak ada cara mengambil piring dari tengah tanpa membongkar yang di
atasnya.

```
      top
       |
      [30]  <- paling terakhir masuk, paling pertama keluar
       |
      [20]
       |
      [10]  <- paling pertama masuk, paling terakhir keluar
       |
     nullptr
```

| Istilah | Artinya |
|---|---|
| `top` | Penunjuk ke elemen **paling atas**. `nullptr` berarti tumpukannya **kosong**. |
| `next` | Penunjuk ke elemen **di bawahnya**. Elemen paling bawah `next`-nya `nullptr`. |

Perhatikan: seluruh tumpukan hanya dikenali dari **satu** penunjuk saja, yaitu
`top`. Anda tidak pernah perlu tahu di mana dasarnya, karena semua operasi hanya
terjadi di puncak.

> **Tidak ada kapasitas.** Tumpukan ini dibangun dari linked list, jadi tidak ada
> batas banyaknya elemen dan tidak ada keadaan "penuh". Sebanyak apa pun
> perubahan yang Anda ketik, seluruhnya harus tertampung. Yang tetap ada adalah
> keadaan **kosong** — dan itu sah, bukan kesalahan. Menekan Ctrl+Z pada dokumen
> yang baru dibuka memang seharusnya tidak melakukan apa-apa.

## Fitur kedua — pemeriksa kurung

Tulis juga dipakai untuk menulis kode, jadi ia punya pemeriksa kurung: setiap
`(`, `[`, dan `{` harus punya pasangan penutup yang sejenis, dan pasangannya
tidak boleh bersilangan.

```
( a + b ) * ( c - d )      seimbang
{ [ ( ) ] }                seimbang, tiga jenis bersarang rapi
( a + [ b ) ]              TIDAK — pasangannya bersilangan
( a + b                    TIDAK — ada buka tanpa penutup
) (                        TIDAK — penutup muncul lebih dulu
```

Sekilas ini soal yang sama sekali berbeda. Tetapi coba perhatikan barisan
`{ [ ( ) ] }`. Ketika Anda bertemu `)`, yang harus dipasangkan dengannya adalah
`(` — yaitu tanda buka yang **paling terakhir** dibuka dan belum tertutup.
Sesudah itu, ketika bertemu `]`, pasangannya `[` yang sekarang menjadi yang
terakhir belum tertutup.

Itu aturan LIFO yang sama persis. Jadi fitur kedua ini memakai tumpukan yang sama
dengan fitur pertama — dan itulah sebabnya keduanya ada di satu study case.

## Satu sesi mengetik

Berikut satu sesi pemakaian Tulis. **Empat langkah bertanda Soal adalah
pekerjaan yang harus Anda kerjakan.**

| # | Yang terjadi | Pekerjaan |
|---:|---|---|
| 1 | Anda mengetik. Setiap perubahan dicatat ke tumpukan riwayat, dan yang baru selalu diletakkan di **puncak**. | **Soal 1** `push` |
| 2 | Status bar menampilkan perubahan terakhir tanpa membatalkannya, dan panel riwayat menampilkan seluruh tumpukan. | *sudah disediakan* (`peek`, `display`) |
| 3 | Anda menekan **Ctrl+Z**. Perubahan paling terakhir dibatalkan, dan editor perlu tahu perubahan apa itu. Ctrl+Z pada dokumen yang belum diapa-apakan tidak boleh membuat aplikasi berhenti tidak wajar. | **Soal 2** `pop` |
| 4 | Anda menekan **Ctrl+S**. Dokumen tersimpan, dan seluruh riwayat undo dibuang sekaligus. | **Soal 3** `clear` |
| 5 | Anda beralih menulis kode. Editor memeriksa apakah tanda kurung sudah berpasangan dengan seimbang. | **Soal 4** `kurungSeimbang` |

### Yang sudah disediakan (tidak dinilai)

| Fungsi | Gunanya |
|---|---|
| `inisialisasi` | menyiapkan tumpukan baru menjadi kosong |
| `isEmpty` | apakah tumpukannya sedang kosong |
| `peek` | melihat puncak tanpa mengambilnya — **pembanding untuk Soal 2** |
| `display` | membaca seluruh isi tumpukan menjadi satu baris teks |

Keempatnya sudah ditulis lengkap di `src/student.cpp`. Pakai `display` sesering
mungkin untuk memeriksa hasil kerja Anda sendiri.

---

## Tujuan Praktikum

Setelah pertemuan ini Anda diharapkan mampu:

- menjelaskan aturan LIFO dan menyebutkan contoh nyatanya;
- membangun stack dari linked list, tanpa batas kapasitas;
- menambah dan mengeluarkan elemen hanya lewat satu ujung;
- melaporkan dua hal sekaligus lewat kembalian dan parameter keluaran;
- menangani keadaan kosong (underflow) dengan wajar;
- membebaskan seluruh node stack dengan benar; dan
- menerapkan stack pada persoalan nyata, yaitu pemeriksaan tanda kurung.

---

## Materi

| Konsep | Yang perlu Anda kuasai |
|---|---|
| LIFO | yang terakhir masuk, dialah yang pertama keluar |
| `top` | satu-satunya penunjuk; `nullptr` berarti kosong |
| Tanpa kapasitas | linked list tidak pernah "penuh" |
| Underflow | mengambil dari tumpukan kosong harus gagal dengan wajar |
| Parameter keluaran | `int&` untuk melaporkan nilai selain kembalian `bool` |
| Pembebasan | node yang keluar harus di-`delete` |
| Penerapan | pemeriksaan tanda kurung memakai LIFO yang sama |

---

## Untuk Mahasiswa

### File yang Harus Dikerjakan

**Satu-satunya file yang dinilai adalah:**

```
src/student.cpp
```

Di bagian paling bawah `src/student.cpp` ada `main()`. Bagian itu memeragakan
seluruh sesi mengetik, sehingga Anda bisa langsung melihat hasil kerja Anda
berjalan sebagai satu cerita utuh. `main()` tersebut **tidak ikut dinilai** dan
**bebas Anda ubah** sesukanya.

Jangan mengubah file lain. Perubahan pada `src/student.h`, `tests/checker.cpp`,
`tests/report.h`, atau `.github/workflows/` tidak akan membuat nilai Anda naik
dan dapat menyebabkan penilaian gagal.

### Contract / API

```cpp
struct Node {
    int   data;
    Node* next;
};

struct Stack {
    Node* top;
};

bool push(Stack& s, int nilai);                 // Soal 1
bool pop(Stack& s, int& nilai);                 // Soal 2
void clear(Stack& s);                           // Soal 3
bool kurungSeimbang(const string& ekspresi);    // Soal 4

// Sudah disediakan, TIDAK dinilai
void   inisialisasi(Stack& s);
bool   isEmpty(const Stack& s);
bool   peek(Stack& s, int& nilai);
string display(Stack& s);
```

Yang **wajib sama**: nama kedua struct beserta field-nya, nama fungsi, tipe
parameter, dan tipe kembalian.

### Aturan yang berlaku untuk seluruh pekerjaan

- Nilai yang disimpan bertipe `int`. Boleh negatif, boleh nol, boleh berulang.
- Tumpukan yang **kosong** adalah keadaan yang sah, bukan kesalahan.
- Tidak ada kapasitas dan tidak ada keadaan "penuh".
- Node dibuat dengan `new` dan yang keluar dilepas dengan `delete`.
- Tidak ada satu pun fungsi yang mencetak ke layar.
- Soal 1, 2, dan 3 dinilai sendiri-sendiri: checker menyiapkan tumpukan ujinya
  tanpa memakai fungsi Anda.
- **Soal 4 adalah penerapan.** Kalau Anda mengerjakannya memakai `push` dan `pop`
  buatan Anda sendiri, pastikan kedua soal itu sudah benar lebih dulu. Anda juga
  boleh mengerjakannya dengan cara lain — yang dinilai hanya hasilnya.

---

# Soal 1 — `push` (25 poin)

> Langkah 1 pada cerita · perubahan dicatat ke puncak tumpukan

Letakkan sebuah nilai baru di posisi **paling atas** tumpukan. Seluruh nilai yang
sudah ada tetap tersimpan dengan urutan yang sama persis, hanya saja sekarang
mereka semua berada di bawah nilai baru itu.

| Parameter | Artinya |
|---|---|
| `s` | Stack milik pemanggil. Bertanda `&` |
| `nilai` | Nilai yang mau dimasukkan |
| *kembalian* | `true` bila nilai baru berhasil masuk |

| Sebelum | Operasi | Sesudah |
|---|---|---|
| `Top -> 20 -> 10` | `push(s, 30)` | `Top -> 30 -> 20 -> 10` |
| `(kosong)` | `push(s, 10)` | `Top -> 10` |

**Yang perlu diingat.**
- Sesudah pemanggilan, `s.top` harus menunjuk node yang baru.
- Node baru harus tersambung ke isi lama: `next` miliknya menunjuk elemen yang
  tadinya di puncak. Lupa menyambung ini membuat seluruh isi lama hilang
  sekaligus bocor di memori.
- Pada tumpukan yang tadinya kosong, `next` milik node baru bernilai `nullptr` —
  dan itu terjadi **dengan sendirinya** kalau Anda menyambungkannya ke `s.top`
  yang memang sedang bernilai `nullptr`. Tidak perlu cabang khusus.
- **Tidak pernah ada penolakan karena penuh.** Kembaliannya selalu `true` selama
  node barunya berhasil dibuat. Ini beda pokok dengan stack berbasis array.
- Setiap pemanggilan menyediakan **tepat satu** node baru.

---

# Soal 2 — `pop` (30 poin)

> Langkah 3 pada cerita · Ctrl+Z membatalkan yang paling terakhir

Keluarkan elemen **paling atas**, beri tahu pemanggil nilainya, lalu buang
node-nya dari memori.

Fungsi ini harus melaporkan **dua** hal sekaligus: berhasil atau tidak, dan nilai
apa yang keluar. Karena satu fungsi hanya bisa mengembalikan satu nilai, yang
kedua disampaikan lewat parameter `nilai` yang bertanda `&`.

> **Petunjuk.** Bandingkan dengan `peek` yang sudah disediakan. Keduanya
> sama-sama mengisi `nilai` dan sama-sama mengembalikan `bool`. Bedanya cuma
> satu: `peek` hanya melihat, sedangkan `pop` benar-benar mengeluarkan dan
> membuang node-nya.

| Parameter | Artinya |
|---|---|
| `s` | Stack milik pemanggil. Bertanda `&` |
| `nilai` | Tempat pemanggil menerima nilai yang keluar. Bertanda `&` |
| *kembalian* | `true` bila ada elemen yang dikeluarkan, `false` bila kosong |

| Sebelum | Operasi | Sesudah |
|---|---|---|
| `Top -> 30 -> 20 -> 10` | `int n; pop(s, n);` | `Top -> 20 -> 10`, `n` = 30 |
| `(kosong)` | `int n = -999; pop(s, n);` | `(kosong)`, `n` **tetap** -999 |

**Yang perlu diingat.**
- Yang keluar selalu elemen **paling atas**.
- `nilai` harus sudah diisi **sebelum** node-nya di-`delete`. Node yang sudah
  dilepas tidak boleh dibaca lagi.
- Node yang keluar harus dibuang dengan `delete`, **tepat satu** per pemanggilan
  yang berhasil.
- **Underflow:** bila tumpukan sedang kosong, kembaliannya `false` dan `nilai`
  **tidak boleh disentuh sama sekali**. Ini ikut diuji — checker mengisi variabel
  penerima dengan penanda lebih dulu, lalu memastikan penanda itu masih utuh.
- Mengeluarkan elemen terakhir membuat tumpukan kosong, dan tumpukan itu harus
  tetap bisa dipakai lagi sesudahnya.

---

# Soal 3 — `clear` (20 poin)

> Langkah 4 pada cerita · Ctrl+S membuang seluruh riwayat undo

Buang seluruh isi tumpukan sekaligus, sampai benar-benar kosong.

Perhatikan baik-baik kata "dibuang". Memutus sambungannya saja tidak cukup. Anda
memang bisa langsung membuat `s.top` bernilai `nullptr`, dan sekilas tumpukannya
akan terlihat kosong — tetapi seluruh node-nya masih menumpuk di memori, dan
sekarang tidak ada satu pun yang bisa mencapainya lagi. Keadaan seperti itu
disebut **kebocoran memori**, dan itu ikut dinilai.

| Parameter | Artinya |
|---|---|
| `s` | Stack milik pemanggil. Bertanda `&` |
| *kembalian* | Tidak ada (`void`) |

**Yang perlu diingat.**
- Sesudah selesai, `s.top` bernilai `nullptr`.
- **Seluruh** node harus dibuang dengan `delete`, bukan hanya yang paling atas.
  Banyaknya node yang dibuang ikut dihitung checker secara **tepat**.
- Alamat node berikutnya harus sudah disimpan **sebelum** sebuah node di-`delete`.
- Memanggilnya pada tumpukan yang sudah kosong, atau dua kali berturut-turut,
  harus aman.
- Sesudah dikosongkan, tumpukan harus tetap bisa **dipakai lagi** seperti biasa.

---

# Soal 4 — `kurungSeimbang` (25 poin)

> Langkah 5 pada cerita · pemeriksa kurung pada kode

Tentukan apakah tanda kurung di dalam sebuah teks sudah berpasangan dengan
seimbang. Ada tiga jenis: `(` dengan `)`, `[` dengan `]`, dan `{` dengan `}`.

Seimbang berarti **tiga hal sekaligus**: setiap tanda buka punya penutup yang
sejenis, setiap tanda tutup punya pembuka yang sejenis, dan pasangan-pasangannya
tidak saling bersilangan.

> **Petunjuk cara berpikirnya** sudah ada di bagian cerita: ketika bertemu sebuah
> tanda tutup, yang harus dipasangkan dengannya selalu tanda buka yang **paling
> terakhir** dibuka dan belum tertutup.

| Parameter | Artinya |
|---|---|
| `ekspresi` | Teks yang mau diperiksa. Boleh sepanjang apa pun, boleh kosong |
| *kembalian* | `true` bila seimbang, `false` bila tidak |

| Ekspresi | Hasil | Kenapa |
|---|---|---|
| `( a + b ) * ( c - d )` | `true` | |
| `{[()]}` | `true` | tiga jenis, bersarang rapi |
| `""` | `true` | tidak ada kurung sama sekali |
| `halo dunia` | `true` | karakter lain diabaikan |
| `( a + b ) * ( c - d` | `false` | ada buka tanpa penutup |
| `( a + [ b ) ]` | `false` | pasangannya bersilangan |
| `)(` | `false` | penutup muncul lebih dulu |
| `(]` | `false` | penutupnya tidak sejenis |

**Yang perlu diingat.**
- Karakter selain keenam tanda kurung **diabaikan**.
- Teks kosong bernilai seimbang.
- Tanda tutup yang muncul saat tidak ada tanda buka yang menunggu berarti tidak
  seimbang — contohnya `)(`, walaupun jumlahnya sama-sama satu.
- **Menghitung jumlah saja tidak cukup.** Pada `( a + [ b ) ]` jumlah bukanya dua
  dan tutupnya dua, tetapi pasangannya bersilangan. Yang menentukan adalah
  **urutannya**.
- Di akhir pemeriksaan, tidak boleh ada tanda buka yang masih menunggu pasangan.
- Tidak ada batas banyaknya tanda kurung yang boleh bersarang.
- Bila Anda memakai stack sendiri, jangan lupa membereskan node-nya sebelum
  fungsi ini selesai.

---

## Batasan

- Hanya **empat** fungsi di `src/student.cpp` yang dinilai: `push`, `pop`,
  `clear`, dan `kurungSeimbang`. Empat fungsi lain sudah disediakan dan tidak
  dinilai. `main()` di bagian paling bawah file itu bebas Anda ubah.
- `cin` hanya boleh dipakai di dalam `main()` tersebut.
- Jangan mengubah `struct Node`, `struct Stack`, maupun signature fungsi di
  `src/student.h`.
- Materi pertemuan ini terbatas pada **stack dengan linked list**. Tidak
  diperlukan array sebagai penyimpanan, queue, pohon, maupun graf.
- Tidak perlu memakai container pustaka standar (`stack`, `vector`, `list`, dan
  sejenisnya) — isi stack harus benar-benar tersimpan sebagai rantai `Node` yang
  Anda kelola sendiri.

---

## Penilaian Otomatis

Total **20 test case**, masing-masing bernilai **5 poin**:

| Soal | Fungsi | Test | Bobot |
|---|---|---:|---:|
| 1 | `push` | 5 | 25 |
| 2 | `pop` | 6 | 30 |
| 3 | `clear` | 4 | 20 |
| 4 | `kurungSeimbang` | 5 | 25 |
| | **Total** | **20** | **100** |

| Kondisi | Score |
|---|---|
| Gagal compile | 0 |
| Sebagian test lolos | jumlah test lolos × 5 |
| Semua test lolos | 100 |

Tumpukan untuk pengujian disiapkan sendiri oleh checker, bukan lewat fungsi Anda.
Karena itu Soal 1 yang belum benar **tidak** ikut menjatuhkan nilai Soal 2 dan 3.
Pengecualiannya Soal 4, yang memang penerapan — lihat catatan pada Aturan di
atas.

### Membaca Hasil

| Status | Artinya |
|---|---|
| ✅ hijau | Semua test berhasil |
| ❌ merah | Ada test yang gagal, compile error, atau program berhenti tidak wajar |

Isi tumpukan ditulis dari **bawah ke atas**, jadi elemen paling kanan adalah
puncaknya. Contoh:

```
Expected: ret=true nilai=30 isi(bawah->atas)=[10] banyak=1 dibuang=1/1
Got     : ret=true nilai=30 isi(bawah->atas)=[10] banyak=1 dibuang=0/1
```

Contoh ini berarti nilainya sudah benar dan tumpukannya sudah berkurang, tetapi
node yang keluar tidak dibuang dari memori.

| Step yang gagal | Penyebab |
|---|---|
| `Periksa penggunaan cin` | Ada `cin`/`scanf` di dalam fungsi yang dinilai |
| `Compile student.cpp` | Ada syntax/compile error di `student.cpp` |
| `Compile checker` | Nama atau signature fungsi tidak sesuai `student.h`, atau `main()` Anda keluar dari blok `#ifndef ADA_MAIN_LAIN` |
| `Jalankan test & hitung score` | Kode berhasil dikompilasi tetapi perilakunya belum sesuai |

---

## Cara Menjalankan Program Anda

`src/student.cpp` adalah program C++ utuh. Tekan **F5** di VS Code, atau:

```bash
g++ -std=c++17 src/student.cpp -o latihan
./latihan
```

`main()` bawaan menjalankan **sesi mengetik secara berurutan**, dan menampilkan
hasil tiap langkah berdampingan dengan jawaban yang benar.

> **Saran urutan pengerjaan.** Kerjakan Soal 1 lebih dulu, karena seluruh
> percobaan di `main()` memerlukan tumpukan yang sudah terisi.

> **Penting: `cin` hanya di dalam `main()`.**
> Jangan pernah menaruh `cin` di dalam keempat fungsi yang dinilai. Saat menilai,
> checker memanggil fungsi-fungsi itu tanpa memberi masukan apa pun, sehingga
> `cin` di sana membaca sampah — dan nilai Anda berubah-ubah setiap kali dinilai.

### Memeriksa Kebocoran Memori (opsional, sangat disarankan)

Karena pertemuan ini memakai `new` dan `delete`, ada satu kesalahan yang tidak
terlihat dari keluaran program: **node yang tidak pernah dilepas**.

```bash
g++ -std=c++17 -fsanitize=address,leak -g src/student.cpp -o latihan_periksa
./latihan_periksa
```

Bila ada node yang bocor, program melaporkannya di akhir dengan keterangan
`LeakSanitizer: detected memory leaks`.

---

## Cara Menjalankan Test di Komputer Sendiri

Butuh `g++` yang mendukung C++17, dan sistem berbasis Linux/macOS (atau WSL di
Windows) karena checker menjalankan setiap test sebagai proses terpisah:

```bash
chmod +x scripts/run_tests.sh
./scripts/run_tests.sh
```

Pada starter code yang belum diisi, compiler memunculkan peringatan
*unused parameter*. Itu wajar dan **tidak** mengurangi nilai.

---

## Cara Mengumpulkan

Tidak ada tombol "submit". **Push adalah pengumpulan.**

```bash
git clone https://github.com/<ORG>/praktikum-05-<username>.git
cd praktikum-05-<username>
# edit src/student.cpp
git add src/student.cpp
git commit -m "Kerjakan pertemuan 5"
git push
```

Anda boleh push berkali-kali. **Setiap percobaan tersimpan**, misalnya
25 → 75 → 100.

---

## Struktur Repository

```
.
├── .github/workflows/test.yml     ← workflow penilaian (jangan diubah)
├── .vscode/                       ← setelan tombol Run (jangan diubah)
├── src/
│   ├── student.h                  ← kontrak/interface (jangan diubah)
│   └── student.cpp                ← KERJAKAN DI SINI ← (main() ada di bawahnya)
├── tests/
│   ├── checker.cpp                ← test instruktur (jangan diubah)
│   └── report.h                   ← penulis result.json (jangan diubah)
├── scripts/
│   ├── run_tests.sh               ← uji lokal
│   ├── periksa_masukan.py         ← menolak cin di dalam fungsi yang dinilai
│   ├── anotasi_gcc.py             ← terjemahan error compiler ke bahasa Indonesia
│   ├── job_summary.py             ← Job Summary dari result.json
│   └── write_error_result.sh      ← result.json saat compile error
└── README.md
```
