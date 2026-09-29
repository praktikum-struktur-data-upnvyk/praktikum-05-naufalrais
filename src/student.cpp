// =============================================================================
// student.cpp — Implementasi Mahasiswa
// Pertemuan 5: Stack (Tumpukan) dengan Linked List
// =============================================================================
// FILE YANG BOLEH DIEDIT      : src/student.cpp  ← HANYA FILE INI
// FILE YANG TIDAK BOLEH DIEDIT: src/student.h, tests/checker.cpp, tests/report.h
//
// -----------------------------------------------------------------------------
// DAFTAR PEKERJAAN DAN BOBOTNYA
// -----------------------------------------------------------------------------
//   Soal 1  push             perubahan dicatat ke puncak tumpukan     25 poin
//   Soal 2  pop              Ctrl+Z membatalkan yang paling terakhir  30 poin
//   Soal 3  clear            Ctrl+S membuang seluruh riwayat undo     20 poin
//   Soal 4  kurungSeimbang   pemeriksa kurung pada kode               25 poin
//
// -----------------------------------------------------------------------------
// SUDAH DISEDIAKAN, TIDAK DINILAI
// -----------------------------------------------------------------------------
//   inisialisasi   menyiapkan tumpukan baru menjadi kosong
//   isEmpty        apakah tumpukannya sedang kosong
//   peek           melihat puncak tanpa mengambilnya
//   display        membaca seluruh isi tumpukan menjadi satu baris teks
//
//   Keempatnya ada di bagian bawah file ini, sudah ditulis lengkap. Pakai
//   `display` sesering mungkin untuk memeriksa hasil kerja Anda sendiri.
//
// -----------------------------------------------------------------------------

#include "student.h"

#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

// =============================================================================
// SUDAH DISEDIAKAN — TIDAK DINILAI, TIDAK PERLU DIUBAH
// =============================================================================
// Keempat fungsi di bawah sudah ditulis lengkap.
//
// `peek` sengaja disediakan sebagai PEMBANDING untuk Soal 2. Perhatikan
// bentuknya baik-baik: `pop` yang Anda kerjakan punya kerangka yang sama
// persis, hanya saja ia juga memindahkan `s.top` dan membuang node-nya.

void inisialisasi(Stack& s) {
    s.top = nullptr;
}

bool isEmpty(const Stack& s) {
    return s.top == nullptr;
}

bool peek(Stack& s, int& nilai) {
    if (s.top == nullptr) return false;

    nilai = s.top->data;
    return true;
}

string display(Stack& s) {
    string hasil;
    for (Node* p = s.top; p != nullptr; p = p->next) {
        if (!hasil.empty()) hasil += " ";
        hasil += to_string(p->data);
    }
    return hasil;
}

// =============================================================================

// SOAL 1
bool push(Stack& s, int nilai) {
    return false;
}

// SOAL 2
bool pop(Stack& s, int& nilai) {
    return false;
}

// SOAL 3
void clear(Stack& s) {
}

// SOAL 4
bool kurungSeimbang(const string& ekspresi) {
    return false;
}

// =============================================================================
// MAIN() — memeragakan sesi mengetik. TIDAK dinilai, bebas diubah.
// =============================================================================
// Di bawah ini file ini menjadi program C++ biasa. Tekan Run di VS Code, atau
// jalankan lewat terminal:
//
//     g++ -std=c++17 src/student.cpp -o latihan
//     ./latihan
//
// Isinya menjalankan sesi mengetik di Tulis secara berurutan, dan menampilkan
// hasil tiap langkah berdampingan dengan jawaban yang benar — sehingga Anda
// bisa langsung membandingkan.
//
// SATU ATURAN YANG TIDAK BOLEH DILANGGAR
// --------------------------------------
// cin hanya boleh dipakai DI DALAM main() ini. JANGAN menaruh cin di dalam
// keempat fungsi yang dinilai. Saat menilai, checker memanggil fungsi-fungsi
// itu tanpa memberi masukan apa pun, jadi cin di sana akan membaca sampah — dan
// nilai Anda berubah-ubah setiap kali dinilai, dari kode yang sama persis.
//
// (Baris #ifndef di bawah hanya urusan teknis: saat menilai, checker memakai
//  main() miliknya sendiri, jadi main() Anda dilewati supaya tidak bentrok.)
// =============================================================================

#ifndef ADA_MAIN_LAIN

static const char* benarSalah(bool nilai) {
    return nilai ? "true" : "false";
}

static ostream& baris(const string& label) {
    return cout << "    " << left << setw(20) << label << ": ";
}

// Keadaan ringkas tumpukan, dibaca lewat fungsi yang sudah disediakan.
static void keadaan(Stack& s) {
    baris("display") << "\"" << display(s) << "\"\n";

    int atas = 0;
    if (peek(s, atas)) baris("puncak") << atas << "\n";
    else               baris("puncak") << "(tidak ada)\n";

    baris("isEmpty") << benarSalah(isEmpty(s)) << "\n";
}

// Satu percobaan Ctrl+Z, lengkap dengan nilai yang diterima.
static void cobaUndo(Stack& s) {
    int nilai = -999;
    bool berhasil = pop(s, nilai);
    baris("Ctrl+Z");
    if (berhasil) cout << "berhasil, yang dibatalkan = " << nilai << "\n";
    else          cout << "gagal (riwayat kosong), nilai tidak diubah ("
                       << nilai << ")\n";
}

static void langkah(const string& teks) {
    cout << "\n" << teks << "\n";
}

int main() {
    cout << "==================================================\n";
    cout << " Study Case — Aplikasi Editor \"Tulis\"\n";
    cout << " Memeragakan satu sesi mengetik\n";
    cout << " (bagian ini tidak ikut dinilai)\n";
    cout << "==================================================\n";

    Stack s;
    inisialisasi(s);

    langkah("[0] Dokumen baru dibuka, riwayat undo masih kosong");
    keadaan(s);

    langkah("[1] SOAL 1 — push: tiga perubahan diketik (10, 20, lalu 30)");
    baris("push 10") << benarSalah(push(s, 10)) << "\n";
    baris("push 20") << benarSalah(push(s, 20)) << "\n";
    baris("push 30") << benarSalah(push(s, 30)) << "\n";
    keadaan(s);
    cout << "\n    Yang benar: display \"30 20 10\", puncak 30, isEmpty false\n";

    langkah("[2] SOAL 1 — tidak ada batas kapasitas: 10 perubahan sekaligus");
    bool semuaMasuk = true;
    for (int i = 1; i <= 10; ++i) {
        if (!push(s, i * 100)) semuaMasuk = false;
    }
    baris("semua masuk") << benarSalah(semuaMasuk) << "\n";
    keadaan(s);
    cout << "\n    Yang benar: semua masuk true — linked list tidak pernah penuh\n";

    langkah("[3] SOAL 2 — pop: Ctrl+Z, yang dibatalkan harus 1000");
    cobaUndo(s);
    keadaan(s);
    cout << "\n    Yang benar: berhasil dengan nilai 1000\n";

    langkah("[4] SOAL 3 — clear: Ctrl+S, seluruh riwayat undo dibuang");
    clear(s);
    keadaan(s);
    cout << "\n    Yang benar: display \"\", puncak (tidak ada), isEmpty true\n";

    langkah("[5] SOAL 2 — Ctrl+Z pada dokumen yang baru disimpan (underflow)");
    cobaUndo(s);
    cout << "\n    Yang benar: gagal, dan nilainya tetap -999 (tidak disentuh)\n";

    langkah("[6] Tumpukan tetap bisa dipakai lagi sesudah dikosongkan");
    push(s, 7);
    push(s, 8);
    keadaan(s);
    cout << "\n    Yang benar: display \"8 7\"\n";

    langkah("[7] SOAL 4 — kurungSeimbang: pemeriksa kurung pada kode");
    const string contoh[] = {
        "( a + b ) * ( c - d )",   // seimbang
        "{[()]}",                  // seimbang, tiga jenis bersarang
        "",                        // seimbang, tidak ada kurung
        "( a + b ) * ( c - d",     // kurang tutup
        "( a + [ b ) ]",           // bersilangan
        ")("                       // tutup muncul lebih dulu
    };
    for (int i = 0; i < 6; ++i) {
        cout << "    \"" << contoh[i] << "\"";
        for (size_t j = contoh[i].size(); j < 24; ++j) cout << " ";
        cout << " -> " << benarSalah(kurungSeimbang(contoh[i])) << "\n";
    }
    cout << "\n    Yang benar: true, true, true, false, false, false\n";

    // -------------------------------------------------------------------------
    // Mau mencoba dengan teks yang Anda ketik sendiri? Hapus tanda // di bawah
    // ini, lalu jalankan lagi.
    // -------------------------------------------------------------------------
    // cout << "\nKetik satu ekspresi: ";
    // string punyaAnda;
    // getline(cin, punyaAnda);
    // cout << "seimbang? " << benarSalah(kurungSeimbang(punyaAnda)) << endl;

    clear(s);

    cout << "\n==================================================\n";
    cout << " Sesi selesai. Silakan ubah bagian ini untuk\n";
    cout << " mencoba percobaan Anda sendiri.\n";
    cout << "==================================================\n";

    return 0;
}
#endif
