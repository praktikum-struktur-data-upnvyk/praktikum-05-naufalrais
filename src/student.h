// =============================================================================
// student.h — Interface Praktikum
// Pertemuan 5: Stack (Tumpukan) dengan Linked List
// =============================================================================
// INSTRUKSI UNTUK MAHASISWA:
//   - JANGAN mengubah file ini.
//   - Implementasikan seluruh fungsi di dalam src/student.cpp.
//   - Nama struct, nama field, nama fungsi, tipe parameter, dan tipe kembalian
//     adalah KONTRAK: checker memanggilnya langsung, jadi tidak boleh diubah.
//   - Cara Anda memenuhi kontrak sepenuhnya bebas. Penilaian hanya melihat
//     perilaku yang teramati oleh pemanggil.
//
// -----------------------------------------------------------------------------
// STUDY CASE: APLIKASI EDITOR "TULIS"
// -----------------------------------------------------------------------------
// Pertemuan ini hanya punya SATU soal, yaitu study case aplikasi editor Tulis.
// Ceritanya lengkap ada di src/student.cpp. File ini hanya ringkasan kontraknya.
//
// Ada EMPAT pekerjaan yang dinilai:
//
//     Soal 1  push              satu perubahan dicatat ke riwayat   25 poin
//     Soal 2  pop               Ctrl+Z membatalkan yang terakhir    30 poin
//     Soal 3  clear             disimpan, riwayat undo dibuang      20 poin
//     Soal 4  kurungSeimbang    pemeriksa kurung pada kode          25 poin
//
// Empat fungsi lain SUDAH DISEDIAKAN lengkap di src/student.cpp dan tidak
// dinilai: `inisialisasi`, `isEmpty`, `peek`, dan `display`.
//
// -----------------------------------------------------------------------------
// KONVENSI UMUM
// -----------------------------------------------------------------------------
//   - Nilai yang disimpan bertipe `int`. Boleh negatif, boleh nol, dan boleh
//     muncul lebih dari sekali.
//   - Stack yang KOSONG adalah keadaan yang sah, bukan kesalahan.
//   - Stack ini berbasis linked list, jadi TIDAK ada batas banyaknya elemen dan
//     tidak ada keadaan "penuh".
//   - Tidak ada fungsi yang mencetak apa pun ke layar.
// =============================================================================

#ifndef STUDENT_H
#define STUDENT_H

#include <string>

using std::string;

/**
 * Satu catatan perubahan di dalam tumpukan riwayat.
 *
 *   `data`   nilai yang disimpan
 *   `next`   catatan DI BAWAHNYA, atau `nullptr` bila dia paling bawah
 */
struct Node {
    int   data;
    Node* next;
};

/**
 * Sebuah stack, diwakili sepenuhnya oleh penunjuk ke elemen teratasnya.
 *
 *   `top`   elemen PALING ATAS, atau `nullptr` bila stack sedang kosong
 *
 *       top
 *        |
 *       [30]  <- paling terakhir masuk, paling pertama keluar
 *        |
 *       [20]
 *        |
 *       [10]  <- paling pertama masuk, paling terakhir keluar
 *        |
 *      nullptr
 */
struct Stack {
    Node* top;
};

/**
 * SOAL 1 — Sebuah nilai baru diletakkan di posisi PALING ATAS tumpukan.
 *
 * Kontrak:
 *   - Setelah pemanggilan, nilai baru itulah elemen teratas, dan banyaknya
 *     elemen bertambah satu.
 *   - Seluruh nilai yang sudah ada sebelumnya tetap tersimpan dengan urutan
 *     yang sama persis, berada di bawah nilai baru.
 *   - Kembalian bernilai true bila nilai baru berhasil masuk.
 *   - Stack ini tidak punya kapasitas tetap, jadi penambahan tidak pernah
 *     ditolak karena kehabisan tempat. Berapa pun banyaknya data yang masuk,
 *     seluruhnya harus tersimpan.
 */
bool push(Stack& s, int nilai);

/**
 * SOAL 2 — Elemen PALING ATAS dikeluarkan dari stack, dan pemanggil menerima
 * nilainya.
 *
 * Kontrak:
 *   - Yang dikeluarkan selalu elemen teratas, yaitu nilai yang paling terakhir
 *     masuk di antara yang masih tersimpan.
 *   - `nilai` diisi dengan nilai elemen yang dikeluarkan.
 *   - Setelah pemanggilan yang berhasil, banyaknya elemen berkurang satu, dan
 *     seluruh elemen sisanya tetap tersimpan dengan urutan yang sama persis.
 *   - Node milik elemen yang keluar harus dilepas dengan `delete`, dan nilainya
 *     sudah diambil SEBELUM node itu dilepas.
 *   - Kembalian bernilai true bila ada elemen yang benar-benar dikeluarkan.
 *   - Bila stack sedang KOSONG (underflow), kembaliannya false, `nilai` tidak
 *     boleh diubah sama sekali, dan stack tetap kosong.
 */
bool pop(Stack& s, int& nilai);

/**
 * SOAL 3 — Seluruh isi stack dibuang sehingga stack kembali kosong.
 *
 * Kontrak:
 *   - Setelah pemanggilan, stack berada dalam keadaan kosong.
 *   - SELURUH node yang tadinya tersimpan harus dilepas dengan `delete`, satu
 *     per satu. Penunjuk ke node berikutnya harus sudah disimpan sebelum sebuah
 *     node dilepas, karena node yang sudah dilepas tidak boleh dibaca lagi.
 *   - Memanggil fungsi ini pada stack yang sudah kosong adalah sah dan tidak
 *     melakukan apa-apa.
 *   - Stack tetap dapat dipakai seperti biasa sesudahnya.
 */
void clear(Stack& s);

/**
 * SOAL 4 — Menentukan apakah tanda kurung di dalam sebuah ekspresi berpasangan
 * dengan seimbang.
 *
 * Tanda kurung yang diperiksa ada tiga macam: `(` dengan `)`, `[` dengan `]`,
 * dan `{` dengan `}`.
 *
 * Kontrak:
 *   - Kembalian bernilai true bila setiap tanda buka memiliki tanda tutup yang
 *     sejenis, setiap tanda tutup memiliki tanda buka yang sejenis, dan
 *     pasangan-pasangan itu tidak saling bersilangan.
 *   - Karakter selain keenam tanda kurung di atas diabaikan.
 *   - Ekspresi yang tidak memuat satu pun tanda kurung, termasuk teks kosong,
 *     bernilai seimbang.
 *   - Ekspresi sepanjang apa pun harus dapat diperiksa; tidak ada batas
 *     banyaknya tanda kurung yang boleh bersarang.
 */
bool kurungSeimbang(const string& ekspresi);

// =============================================================================
// SUDAH DISEDIAKAN — TIDAK DINILAI
// =============================================================================
// Keempat fungsi di bawah sudah ditulis lengkap di src/student.cpp. Anda tidak
// perlu mengerjakannya.
// =============================================================================

/** Menyiapkan sebuah stack baru menjadi kosong. Dipakai pada variabel yang
 *  belum pernah dipakai, jadi ia hanya menetapkan keadaan kosong dan TIDAK
 *  melepas node apa pun. */
void inisialisasi(Stack& s);

/** Apakah stack sedang kosong. Tidak mengubah stack. */
bool isEmpty(const Stack& s);

/** Melihat elemen teratas tanpa mengeluarkannya. Mengisi `nilai` dan
 *  mengembalikan true bila ada; pada stack kosong mengembalikan false tanpa
 *  menyentuh `nilai`. Disediakan sebagai PEMBANDING untuk Soal 2 — bedanya
 *  dengan `pop` cuma satu hal, yaitu `peek` tidak mengeluarkan apa pun. */
bool peek(Stack& s, int& nilai);

/** Seluruh isi stack dibaca dari elemen teratas ke elemen terbawah menjadi satu
 *  baris teks, dipisahkan satu spasi. Stack kosong menghasilkan "". */
string display(Stack& s);

#endif // STUDENT_H
