// =============================================================================
// checker.cpp — Test Harness Instruktur
// Pertemuan 5: Stack (Tumpukan) dengan Linked List
// =============================================================================
// File ini adalah milik instruktur dan TIDAK boleh diubah mahasiswa.
//
// PRINSIP PENILAIAN (behavior-based):
//   Checker hanya memanggil fungsi mahasiswa lalu memeriksa akibatnya: isi dan
//   urutan stack sesudah pemanggilan, banyaknya elemen, nilai yang diterima
//   pemanggil lewat parameter keluaran, dan nilai yang dikembalikan. Nama
//   variabel, gaya penulisan, jenis loop, urutan kode, komentar, dan formatting
//   tidak pernah diperiksa. Implementasi apa pun yang memenuhi kontrak di
//   student.h akan PASS.
//
// BOBOT:
//   20 test case, bobot rata 100 / 20 = 5 poin per test (lihat report.h).
//
//   Pertemuan ini hanya punya SATU soal, yaitu study case aplikasi editor
//   "Tulis". Keempat fungsi di bawah adalah pekerjaan di dalam study case yang
//   sama, bukan soal yang berdiri sendiri-sendiri. Masing-masing tetap diberi
//   suite terpisah supaya nilai parsial tetap dapat.
//
//     Soal 1  push             5 test = 25   perubahan dicatat ke puncak
//     Soal 2  pop              6 test = 30   Ctrl+Z membatalkan yang terakhir
//     Soal 3  clear            4 test = 20   Ctrl+S membuang riwayat undo
//     Soal 4  kurungSeimbang   5 test = 25   pemeriksa kurung pada kode
//
//   Empat fungsi lain (inisialisasi, isEmpty, peek, display) SUDAH DISEDIAKAN
//   lengkap di src/student.cpp dan TIDAK dinilai. Checker memakainya hanya
//   sebagai alat baca saat memeriksa soal lain.
//
//   CATATAN KETERKAITAN: Soal 4 adalah PENERAPAN stack. Mahasiswa boleh
//   mengerjakannya memakai push/pop buatannya sendiri, sehingga Soal 1 atau 2
//   yang rusak bisa ikut menjatuhkannya. Itu wajar dan disebutkan di soal —
//   mereka juga boleh memakai cara lain.
//
// TASK SALING BEBAS:
//   Stack untuk pengujian dibangun langsung oleh checker (lihat buatStack) yang
//   merangkai sendiri node-nya dengan `new`, bukan lewat fungsi mahasiswa.
//   Bentuk penyimpanan `Node` dan `Stack` sudah ditetapkan sebagai kontrak di
//   student.h, sehingga checker dapat menyiapkan keadaan apa pun — kosong,
//   berisi satu elemen, atau berisi banyak elemen — tanpa bergantung pada satu
//   pun fungsi mahasiswa. Dengan begitu `push` yang salah tidak ikut
//   menjatuhkan nilai `pop`, `peek`, maupun `display`.
//
//   Pengecualian yang disengaja adalah dua test LIFO pada Soal 2, yang memang menguji rangkaian
//   operasi campuran dan karena itu memakai fungsi mahasiswa sendiri.
//
// CARA MEMERIKSA "TIDAK BOLEH MENGUBAH STACK":
//   Karena checker membaca isi `Stack` secara langsung, kontrak seperti "peek
//   tidak boleh mengubah stack" dan "display tidak boleh mengubah stack"
//   diperiksa dengan membandingkan keadaan stack sesudah pemanggilan terhadap
//   keadaan yang seharusnya. Implementasi display yang mengosongkan stack demi
//   membaca isinya akan langsung tertangkap.
//
// ISOLASI PROSES:
//   Setiap test dijalankan di dalam PROSES ANAK hasil fork() dengan batas waktu
//   dan batas memori. Anak menyiapkan stacknya sendiri, memanggil fungsi
//   mahasiswa, lalu menuliskan RINGKASAN KEADAAN AKHIR sebagai teks; induk
//   hanya membandingkan teks itu dengan teks yang diharapkan. Akibatnya:
//     - penunjuk yang tidak sah / node yang sudah dilepas -> FAIL biasa
//     - rantai node yang menunjuk balik ke dirinya sendiri -> "WAKTU HABIS"
//       atau tertangkap lebih dulu oleh BATAS_TELUSUR di gambarStack
//     - ketiganya TIDAK menghentikan test-test berikutnya
//
//   Isolasi ini murni soal ketahanan checker. Yang dinilai tetap perilaku
//   fungsi mahasiswa, dan mekanisme grading global (report.h, scripts/,
//   workflow, skema result.json) tidak diubah sama sekali.
//
// KETAHANAN TERHADAP CRASH:
//   Checker menyimpan snapshot result.json setiap kali satu test selesai,
//   dengan test yang belum sempat berjalan dicatat sebagai FAIL. Nilai parsial
//   yang sudah diperoleh tetap tercatat dan tidak berubah menjadi 0.
//
// CATATAN KEAMANAN:
//   Mahasiswa dapat membaca file ini. Mitigasi:
//   - Setiap kontrak diuji dengan beberapa keadaan stack (kosong, satu
//     elemen, banyak elemen) dan beberapa nilai (positif, nol, negatif)
//   - Repository mahasiswa bersifat privat
// =============================================================================

#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <new>
#include <sstream>
#include <string>
#include <vector>

#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>

#include "report.h"
#include "../src/student.h"

using namespace std;

// =============================================================================
// Penghitung blok memori dinamis yang masih hidup
// =============================================================================
// Mengganti operator new/delete global adalah cara yang sah dalam C++ dan
// berlaku untuk seluruh program, termasuk `new Node` di dalam student.cpp.
// Angka ini hanya dibaca sebagai SELISIH pada potongan kode yang sangat pendek,
// sehingga alokasi milik checker sendiri tidak ikut terhitung.

static long long g_blokHidup = 0;

void* operator new(size_t ukuran) {
    if (ukuran == 0) ukuran = 1;
    void* blok = malloc(ukuran);
    if (blok == nullptr) throw bad_alloc();
    ++g_blokHidup;
    return blok;
}

void* operator new[](size_t ukuran) {
    return ::operator new(ukuran);
}

void operator delete(void* blok) noexcept {
    if (blok != nullptr) {
        --g_blokHidup;
        free(blok);
    }
}

void operator delete[](void* blok) noexcept {
    ::operator delete(blok);
}

void operator delete(void* blok, size_t) noexcept {
    ::operator delete(blok);
}

void operator delete[](void* blok, size_t) noexcept {
    ::operator delete(blok);
}

static long long blokHidup() {
    return g_blokHidup;
}

// =============================================================================
// Test Framework (sederhana, tanpa dependency eksternal)
// =============================================================================

// ANSI color codes untuk output terminal yang jelas
#define COLOR_GREEN "\033[32m"
#define COLOR_RED   "\033[31m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_CYAN  "\033[36m"
#define COLOR_RESET "\033[0m"
#define COLOR_BOLD  "\033[1m"

static int total_tests = 0;
static int passed_tests = 0;
static int failed_tests = 0;

// Banyaknya test yang direncanakan pada pertemuan ini. Dipakai untuk menghitung
// skor snapshot supaya test yang belum berjalan tetap terhitung sebagai gagal.
static const int TOTAL_TEST_DIRENCANAKAN = 20;

// Menulis result.json versi sementara setelah setiap test selesai.
// Test yang belum dijalankan ditambahkan sebagai FAIL, lalu dilepas kembali,
// sehingga isi rekaman sebenarnya tidak terpengaruh.
static void simpanHasilSementara() {
    vector<TestRecord>& rekaman = test_records();
    const size_t jumlahAsli = rekaman.size();

    for (size_t i = jumlahAsli;
         i < static_cast<size_t>(TOTAL_TEST_DIRENCANAKAN); ++i) {
        rekaman.push_back(TestRecord{
            "(test #" + to_string(i + 1) + " belum dijalankan)",
            "FAIL",
            "Checker berhenti sebelum test ini sempat dijalankan."});
    }

    const int skor = passed_tests * 100 / TOTAL_TEST_DIRENCANAKAN;
    write_result_json("result.json", skor);

    rekaman.resize(jumlahAsli);
}

// Mencatat satu hasil test. Dipakai oleh makro UJI di bawah.
static void catatHasil(const string& nama, bool lulus,
                       const string& keterangan) {
    total_tests++;
    if (lulus) {
        passed_tests++;
        cout << COLOR_GREEN << "  [PASS]" << COLOR_RESET << " " << nama
                  << endl;
    } else {
        failed_tests++;
        cout << COLOR_RED << "  [FAIL]" << COLOR_RESET << " " << nama
                  << endl;
        cout << "         Keterangan: " << keterangan << endl;
    }
    record_test(nama, lulus, lulus ? "" : keterangan);
    simpanHasilSementara();
}

// =============================================================================
// Menjalankan satu test di dalam proses anak dengan batas waktu
// =============================================================================

// Batas waktu satu test. Implementasi yang benar selesai dalam hitungan
// milidetik; batas ini semata-mata menangkap perulangan yang tidak berhenti.
static const int BATAS_DETIK = 5;

// Batas pemakaian memori proses anak (512 MB), supaya perulangan yang terus
// menumpuk teks tidak menghabiskan memori runner sebelum batas waktu tercapai.
static const rlim_t BATAS_MEMORI = static_cast<rlim_t>(512) * 1024 * 1024;

// Batas panjang teks hasil, jauh di bawah kapasitas pipe (64 KB) sehingga anak
// tidak pernah terhalang saat menulis.
static const size_t BATAS_KELUARAN = 8000;

enum KeadaanAnak { ANAK_SELESAI, ANAK_WAKTU_HABIS, ANAK_BERHENTI };

struct HasilAnak {
    KeadaanAnak keadaan;
    string teks;
    int penyebab;   // nomor sinyal atau kode keluar, sesuai keadaan
};

typedef string (*FungsiUji)();

static HasilAnak jalankanTerisolasi(FungsiUji uji) {
    HasilAnak hasil;
    hasil.keadaan = ANAK_BERHENTI;
    hasil.penyebab = 0;

    int pipa[2];
    if (pipe(pipa) != 0) {
        hasil.teks = "(checker gagal menyiapkan pipe)";
        return hasil;
    }

    cout.flush();
    cerr.flush();

    pid_t anak = fork();
    if (anak < 0) {
        close(pipa[0]);
        close(pipa[1]);
        hasil.teks = "(checker gagal membuat proses anak)";
        return hasil;
    }

    if (anak == 0) {
        // ---------------------------------------------------------------
        // Proses anak: di sinilah fungsi mahasiswa benar-benar dipanggil.
        // ---------------------------------------------------------------
        close(pipa[0]);

        struct rlimit batas;
        batas.rlim_cur = BATAS_MEMORI;
        batas.rlim_max = BATAS_MEMORI;
        setrlimit(RLIMIT_AS, &batas);

        string keluaran;
        try {
            keluaran = uji();
        } catch (const exception& e) {
            keluaran = string("(program melempar exception: ") + e.what() + ")";
        } catch (...) {
            keluaran = "(program melempar exception)";
        }
        if (keluaran.size() > BATAS_KELUARAN) {
            keluaran.resize(BATAS_KELUARAN);
            keluaran += "...(dipotong)";
        }

        const char* data = keluaran.c_str();
        size_t sisa = keluaran.size();
        while (sisa > 0) {
            ssize_t ditulis = write(pipa[1], data, sisa);
            if (ditulis <= 0) break;
            data += ditulis;
            sisa -= static_cast<size_t>(ditulis);
        }
        close(pipa[1]);
        _exit(0);
    }

    // -------------------------------------------------------------------
    // Proses induk: menunggu anak, dengan batas waktu.
    // -------------------------------------------------------------------
    close(pipa[1]);

    int status = 0;
    bool berakhir = false;
    for (int i = 0; i < BATAS_DETIK * 100; ++i) {
        pid_t hasilTunggu = waitpid(anak, &status, WNOHANG);
        if (hasilTunggu == anak) { berakhir = true; break; }
        if (hasilTunggu < 0) { berakhir = true; break; }
        struct timespec jeda;
        jeda.tv_sec = 0;
        jeda.tv_nsec = 10L * 1000L * 1000L;   // 10 ms
        nanosleep(&jeda, nullptr);
    }

    if (!berakhir) {
        kill(anak, SIGKILL);
        waitpid(anak, &status, 0);
    }

    string teks;
    char penyangga[4096];
    ssize_t dibaca;
    while ((dibaca = read(pipa[0], penyangga, sizeof(penyangga))) > 0) {
        teks.append(penyangga, static_cast<size_t>(dibaca));
    }
    close(pipa[0]);

    if (!berakhir) {
        hasil.keadaan = ANAK_WAKTU_HABIS;
        hasil.penyebab = BATAS_DETIK;
    } else if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        hasil.keadaan = ANAK_SELESAI;
    } else if (WIFSIGNALED(status)) {
        hasil.keadaan = ANAK_BERHENTI;
        hasil.penyebab = WTERMSIG(status);
    } else {
        hasil.keadaan = ANAK_BERHENTI;
        hasil.penyebab = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
    }
    hasil.teks = teks;
    return hasil;
}

// Menerjemahkan hasil proses anak menjadi teks "Got" yang dibaca manusia.
static string bacaHasil(const HasilAnak& hasil) {
    switch (hasil.keadaan) {
        case ANAK_SELESAI:
            return hasil.teks;
        case ANAK_WAKTU_HABIS: {
            ostringstream out;
            out << "(operasi tidak pernah berhenti — dihentikan setelah "
                << hasil.penyebab << " detik; kemungkinan perulangan yang "
                   "tidak menemukan titik berhenti)";
            return out.str();
        }
        default: {
            ostringstream out;
            out << "(program berhenti tidak wajar";
            if (hasil.penyebab == SIGSEGV) {
                out << ": mengakses memori yang tidak sah (SIGSEGV), mis. "
                       "menulis di luar jangkauan array";
            } else if (hasil.penyebab == SIGABRT) {
                out << ": program dihentikan paksa (SIGABRT)";
            } else if (hasil.penyebab != 0) {
                out << ": sinyal/kode " << hasil.penyebab;
            }
            out << ")";
            if (!hasil.teks.empty()) out << " keluaran sebagian: " << hasil.teks;
            return out.str();
        }
    }
}

// Makro utama: jalankan `fungsi` secara terisolasi, bandingkan teks hasilnya
// dengan `harapan`.
#define UJI(nama, fungsi, harapan) do { \
    HasilAnak _h = jalankanTerisolasi(fungsi); \
    string _dapat = bacaHasil(_h); \
    string _harap = (harapan); \
    bool _ok = (_h.keadaan == ANAK_SELESAI) && (_dapat == _harap); \
    ostringstream _pesan; \
    _pesan << "Expected: " << _harap << " | Got: " << _dapat; \
    catatHasil((nama), _ok, _pesan.str()); \
} while (0)

// =============================================================================
// Utilitas stack milik checker (dipakai di dalam proses anak)
// =============================================================================

static const char* bo(bool nilai) { return nilai ? "true" : "false"; }

// Batas penelusuran rantai node. Implementasi yang salah dapat membentuk rantai
// yang menunjuk balik ke dirinya sendiri; batas ini membuat keadaan itu
// dilaporkan sebagai FAIL yang terbaca, bukan sebagai penelusuran tanpa henti.
static const int BATAS_TELUSUR = 200;

// Menyiapkan stack uji secara langsung dengan merangkai node sendiri, tanpa
// memakai satu pun fungsi mahasiswa. `nilai` diurutkan dari elemen paling BAWAH
// ke elemen paling ATAS, sehingga nilai[n-1] menjadi elemen teratas.
static void buatStack(Stack& s, const int* nilai, int n) {
    s.top = nullptr;
    for (int i = 0; i < n; ++i) {
        Node* baru = new Node;
        baru->data = nilai[i];
        baru->next = s.top;
        s.top = baru;
    }
}

// Ringkasan keadaan sebuah stack: isi dari bawah ke atas beserta banyaknya
// elemen. Inilah teks yang dibandingkan dengan harapan.
static string gambarStack(const Stack& s) {
    int nilai[BATAS_TELUSUR];
    int banyak = 0;

    for (Node* p = s.top; p != nullptr; p = p->next) {
        if (banyak >= BATAS_TELUSUR) {
            ostringstream out;
            out << "isi(bawah->atas)=[?] banyak=lebih dari " << BATAS_TELUSUR
                << " (rantai node tidak berujung — kemungkinan menunjuk balik "
                   "ke node sebelumnya)";
            return out.str();
        }
        nilai[banyak] = p->data;
        banyak++;
    }

    ostringstream out;
    out << "isi(bawah->atas)=[";
    for (int i = banyak - 1; i >= 0; --i) {
        if (i < banyak - 1) out << " ";
        out << nilai[i];
    }
    out << "] banyak=" << banyak;
    return out.str();
}

// Nilai yang ditaruh di variabel penerima sebelum pop/peek dipanggil. Bila
// operasinya gagal, nilai ini harus masih utuh sesudahnya.
static const int NILAI_AWAL = -999;

// =============================================================================
// SOAL — push  (4 test = 20 poin)
// =============================================================================

// Menambah pada stack kosong, lalu menambah sekali lagi di atasnya.
static string uji01() {
    Stack s;
    buatStack(s, nullptr, 0);

    ostringstream out;
    bool r1 = push(s, 10);
    out << "push10: ret=" << bo(r1) << " " << gambarStack(s);
    bool r2 = push(s, 20);
    out << " | push20: ret=" << bo(r2) << " " << gambarStack(s);
    return out.str();
}

// Menambah berturut-turut dari kosong, termasuk nilai negatif dan nol.
static string uji02() {
    Stack s;
    buatStack(s, nullptr, 0);

    const int urutan[] = {10, 20, 30, -5, 0};
    bool semua = true;
    for (int i = 0; i < 5; ++i) {
        if (!push(s, urutan[i])) semua = false;
    }

    ostringstream out;
    out << "semuaBerhasil=" << (semua ? "ya" : "tidak") << " " << gambarStack(s);
    return out.str();
}

// TIDAK ADA KAPASITAS: penambahan sebanyak apa pun harus tetap diterima dan
// tersimpan seluruhnya. Test ini menangkap implementasi yang masih membawa
// batas kapasitas dari stack berbasis array.
static string uji03() {
    Stack s;
    buatStack(s, nullptr, 0);

    bool semua = true;
    for (int i = 1; i <= 12; ++i) {
        if (!push(s, i)) semua = false;
    }

    ostringstream out;
    out << "semuaBerhasil=" << (semua ? "ya" : "tidak") << " " << gambarStack(s);
    return out.str();
}

// Menambah di atas isi yang sudah ada: rantai lama harus tetap utuh dan berada
// di bawah nilai baru, dengan urutan yang sama persis.
static string uji04() {
    const int awal[] = {7, 3};
    Stack s;
    buatStack(s, awal, 2);

    ostringstream out;
    bool r1 = push(s, 9);
    out << "push9: ret=" << bo(r1) << " " << gambarStack(s);
    bool r2 = push(s, -4);
    out << " | push-4: ret=" << bo(r2) << " " << gambarStack(s);
    return out.str();
}

// =============================================================================
// SOAL — pop  (4 test = 20 poin)
// =============================================================================

// Mengambil satu elemen: yang keluar harus elemen teratas.
static string uji05() {
    const int awal[] = {10, 20, 30};
    Stack s;
    buatStack(s, awal, 3);

    // Node yang keluar harus benar-benar dilepas, bukan cuma dilewati. Tanpa
    // pemeriksaan ini, `pop` yang bocor tetap lolos seluruh test.
    int nilai = NILAI_AWAL;
    long long sebelum = blokHidup();
    bool r = pop(s, nilai);
    long long sesudah = blokHidup();

    long long b1 = blokHidup();
    int abaikan = NILAI_AWAL;
    bool rGagal = pop(s, abaikan);
    rGagal = rGagal && false;
    long long b2 = blokHidup();
    (void)rGagal;

    ostringstream out;
    out << "ret=" << bo(r) << " nilai=" << nilai << " " << gambarStack(s)
        << " dibuang=" << (sebelum - sesudah) << "/1"
        << " dibuangLagi=" << (b1 - b2) << "/1";
    return out.str();
}

// Beberapa pengambilan berturut-turut: urutan keluarnya harus kebalikan urutan
// masuknya.
static string uji06() {
    const int awal[] = {10, 20, 30, 40};
    Stack s;
    buatStack(s, awal, 4);

    ostringstream out;
    out << "keluar=[";
    bool semua = true;
    for (int i = 0; i < 3; ++i) {
        int nilai = NILAI_AWAL;
        if (!pop(s, nilai)) semua = false;
        if (i > 0) out << " ";
        out << nilai;
    }
    out << "] semuaBerhasil=" << (semua ? "ya" : "tidak") << " " << gambarStack(s);
    return out.str();
}

// Mengambil sampai stack menjadi kosong.
static string uji07() {
    const int awal[] = {10, 20};
    Stack s;
    buatStack(s, awal, 2);

    ostringstream out;
    out << "keluar=[";
    bool semua = true;
    for (int i = 0; i < 2; ++i) {
        int nilai = NILAI_AWAL;
        if (!pop(s, nilai)) semua = false;
        if (i > 0) out << " ";
        out << nilai;
    }
    out << "] semuaBerhasil=" << (semua ? "ya" : "tidak") << " " << gambarStack(s);
    return out.str();
}

// UNDERFLOW: mengambil dari stack kosong harus gagal tanpa menyentuh variabel
// penerima. Pengambilan dari stack berisi dipakai sebagai kontrol, supaya
// jawaban "selalu gagal" tidak bisa lolos.
static string uji08() {
    ostringstream out;

    Stack kosong;
    buatStack(kosong, nullptr, 0);
    int nilaiKosong = NILAI_AWAL;
    bool r1 = pop(kosong, nilaiKosong);
    out << "kosong: ret=" << bo(r1) << " nilai=" << nilaiKosong << " "
        << gambarStack(kosong);

    const int berisi[] = {10, 20, 30, 40, 50};
    Stack s;
    buatStack(s, berisi, 5);
    int nilai = NILAI_AWAL;
    bool r2 = pop(s, nilai);
    out << " | kontrol: ret=" << bo(r2) << " nilai=" << nilai << " "
        << gambarStack(s);
    return out.str();
}

// =============================================================================
// SOAL — peek  (2 test = 10 poin)
// =============================================================================



// =============================================================================
// SOAL — inisialisasi, isEmpty, clear  (3 test = 15 poin)
// =============================================================================



// clear mengosongkan stack yang berisi, boleh dipanggil lagi pada stack yang
// sudah kosong, dan stack tetap dapat dipakai seperti biasa sesudahnya.
static string uji13() {
    const int awal[] = {10, 20, 30};
    Stack s;
    buatStack(s, awal, 3);

    ostringstream out;
    clear(s);
    out << "setelahClear: " << gambarStack(s) << " isEmpty=" << bo(isEmpty(s));

    clear(s);
    out << " | clearLagi: " << gambarStack(s) << " isEmpty=" << bo(isEmpty(s));

    push(s, 77);
    int nilai = NILAI_AWAL;
    bool r = pop(s, nilai);
    out << " | dipakaiLagi: ret=" << bo(r) << " nilai=" << nilai << " "
        << gambarStack(s);
    return out.str();
}

// =============================================================================
// SOAL — display  (2 test = 10 poin)
// =============================================================================



// =============================================================================
// SOAL — urutan LIFO  (3 test = 15 poin)
// =============================================================================
// Berbeda dari task lain, ketiga test ini SENGAJA memakai fungsi mahasiswa
// untuk membangun keadaannya, karena yang diuji memang rangkaian operasinya.

// Rangkaian dasar: push tiga kali, ambil, tambah lagi, lalu kosongkan.
static string uji16() {
    Stack s;
    s.top = reinterpret_cast<Node*>(0xD15EA5E);
    inisialisasi(s);

    int keluar[4];
    bool semua = true;

    push(s, 10);
    push(s, 20);
    push(s, 30);
    if (!pop(s, keluar[0])) semua = false;
    push(s, 40);
    if (!pop(s, keluar[1])) semua = false;
    if (!pop(s, keluar[2])) semua = false;
    if (!pop(s, keluar[3])) semua = false;

    ostringstream out;
    out << "keluar=[";
    for (int i = 0; i < 4; ++i) {
        if (i > 0) out << " ";
        out << keluar[i];
    }
    out << "] semuaBerhasil=" << (semua ? "ya" : "tidak")
        << " isEmpty=" << bo(isEmpty(s)) << " " << gambarStack(s);
    return out.str();
}

// Rangkaian yang menyentuh batas bawah di tengah jalan lalu dilanjutkan lagi,
// serta memakai clear di tengah rangkaian. Stack harus tetap dapat dipakai
// secara normal sesudahnya.
static string uji17() {
    Stack s;
    s.top = reinterpret_cast<Node*>(0xD15EA5E);
    inisialisasi(s);

    ostringstream out;

    for (int i = 1; i <= 5; ++i) push(s, i);
    out << "display=\"" << display(s) << "\"";

    out << " keluar=[";
    for (int i = 0; i < 5; ++i) {
        int nilai = NILAI_AWAL;
        pop(s, nilai);
        if (i > 0) out << " ";
        out << nilai;
    }
    out << "]";

    int lebih = NILAI_AWAL;
    bool gagal = pop(s, lebih);
    out << " popSaatKosong=" << bo(gagal) << " nilai=" << lebih;

    push(s, 8);
    push(s, 9);
    out << " | setelahDipakaiLagi: " << gambarStack(s);

    clear(s);
    out << " | setelahClear: isEmpty=" << bo(isEmpty(s)) << " "
        << gambarStack(s);
    return out.str();
}


// =============================================================================
// SOAL — kurungSeimbang  (2 test = 10 poin)
// =============================================================================

// Ekspresi yang pasangan kurungnya seimbang.
static string uji19() {
    ostringstream out;
    out << "berpasangan=" << bo(kurungSeimbang("( a + b ) * ( c - d )"));
    out << " kosong=" << bo(kurungSeimbang(""));
    out << " tanpaKurung=" << bo(kurungSeimbang("a + b * c"));
    out << " campuran=" << bo(kurungSeimbang("{[()]}"));
    out << " bersarang=" << bo(kurungSeimbang("((()))"));
    out << " gabungan=" << bo(kurungSeimbang("( a + [ b - c ] ) * { d }"));

    // Bersarang jauh lebih dalam daripada kapasitas stack berbasis array pada
    // versi sebelumnya — implementasi linked list tidak boleh punya batas.
    string dalam;
    for (int i = 0; i < 40; ++i) dalam += "(";
    for (int i = 0; i < 40; ++i) dalam += ")";
    out << " bersarangDalam=" << bo(kurungSeimbang(dalam));
    return out.str();
}

// Ekspresi yang pasangan kurungnya tidak seimbang, dalam berbagai bentuk.
// Satu ekspresi yang seimbang disertakan sebagai kontrol, supaya jawaban
// "selalu tidak seimbang" tidak bisa lolos.
static string uji20() {
    ostringstream out;
    out << "kurangTutup=" << bo(kurungSeimbang("( a + b ) * ( c - d"));
    out << " kurangBuka=" << bo(kurungSeimbang("a + b ) * ( c - d )"));
    out << " bersilangan=" << bo(kurungSeimbang("( a + [ b ) ]"));
    out << " terbalik=" << bo(kurungSeimbang(")("));
    out << " jenisSalah=" << bo(kurungSeimbang("{ [ }"));
    out << " satuBuka=" << bo(kurungSeimbang("("));
    out << " kontrol=" << bo(kurungSeimbang("( a ) [ b ] { c }"));
    return out.str();
}

// =============================================================================
// Pendaftaran test
// =============================================================================

// =============================================================================
// Test tambahan
// =============================================================================

// Soal 1: setiap pemanggilan menyediakan TEPAT satu node, dan tumpukan yang
// sangat dalam tetap tertampung — linked list tidak punya kapasitas tetap.
static string uji21() {
    Stack s;
    inisialisasi(s);

    long long sebelum = blokHidup();
    bool semuaLima = true;
    for (int i = 1; i <= 5; ++i) {
        if (!push(s, i * 11)) semuaLima = false;
    }
    long long sesudah = blokHidup();

    ostringstream out;
    out << "lima: semuaTrue=" << bo(semuaLima)
        << " nodeBaru=" << (sesudah - sebelum) << "/5 " << gambarStack(s);

    Stack dalam;
    inisialisasi(dalam);
    bool semuaDalam = true;
    for (int i = 0; i < 150; ++i) {
        if (!push(dalam, i)) semuaDalam = false;
    }
    int puncak = 0;
    peek(dalam, puncak);
    int banyak = 0;
    for (Node* p = dalam.top; p != nullptr && banyak <= 200; p = p->next) banyak++;

    out << " | dalam: semuaTrue=" << bo(semuaDalam)
        << " banyak=" << banyak << " puncak=" << puncak;

    clear(s);
    clear(dalam);
    return out.str();
}

// Soal 3: banyaknya node yang dibebaskan diperiksa TEPAT, bukan sekadar
// "tumpukannya jadi kosong". Pembebasan yang berhenti di tengah jalan
// tertangkap di sini.
static string uji22() {
    ostringstream out;

    const int satu[] = {42};
    Stack s1;
    buatStack(s1, satu, 1);
    long long a1 = blokHidup();
    clear(s1);
    long long a2 = blokHidup();
    out << "satu: dibebaskan=" << (a1 - a2) << "/1 isEmpty=" << bo(isEmpty(s1));

    const int sepuluh[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Stack s2;
    buatStack(s2, sepuluh, 10);
    long long b1 = blokHidup();
    clear(s2);
    long long b2 = blokHidup();
    out << " | sepuluh: dibebaskan=" << (b1 - b2) << "/10 isEmpty="
        << bo(isEmpty(s2));

    return out.str();
}

// Soal 3: clear di tengah pemakaian. Sesudah sebagian elemen dikeluarkan lewat
// pop, yang dibebaskan clear harus tepat sebanyak SISANYA saja.
static string uji23() {
    const int isi[] = {1, 2, 3, 4, 5};
    Stack s;
    buatStack(s, isi, 5);

    int buang = 0;
    pop(s, buang);
    pop(s, buang);

    long long sebelum = blokHidup();
    clear(s);
    long long sesudah = blokHidup();

    ostringstream out;
    out << "dibebaskan=" << (sebelum - sesudah) << "/3"
        << " isEmpty=" << bo(isEmpty(s)) << " " << gambarStack(s);
    return out.str();
}

// Soal 3: clear pada tumpukan yang sudah kosong dan clear dua kali
// berturut-turut sama-sama aman, dan TIDAK membebaskan node apa pun.
static string uji24() {
    ostringstream out;

    Stack kosong;
    inisialisasi(kosong);
    long long a1 = blokHidup();
    clear(kosong);
    clear(kosong);
    long long a2 = blokHidup();
    out << "kosong: dibebaskan=" << (a1 - a2) << "/0 isEmpty="
        << bo(isEmpty(kosong));

    const int isi[] = {7, 8};
    Stack s;
    buatStack(s, isi, 2);
    clear(s);
    long long b1 = blokHidup();
    clear(s);
    long long b2 = blokHidup();
    out << " | clearKedua: dibebaskan=" << (b1 - b2) << "/0 isEmpty="
        << bo(isEmpty(s)) << " " << gambarStack(s);

    return out.str();
}

// Soal 4: tanda tutup yang TIDAK SEJENIS dengan pembukanya, untuk seluruh
// kombinasi silang yang mungkin.
static string uji25() {
    ostringstream out;
    out << "kurungSiku=" << bo(kurungSeimbang("(]"));
    out << " kurungKurawal=" << bo(kurungSeimbang("(}"));
    out << " sikuBulat=" << bo(kurungSeimbang("[)"));
    out << " sikuKurawal=" << bo(kurungSeimbang("[}"));
    out << " kurawalBulat=" << bo(kurungSeimbang("{)"));
    out << " kurawalSiku=" << bo(kurungSeimbang("{]"));
    out << " kontrol=" << bo(kurungSeimbang("()[]{}"));
    return out.str();
}

// Soal 4: karakter selain keenam tanda kurung benar-benar diabaikan, termasuk
// pada potongan kode yang penuh operator dan tanda baca lain.
static string uji26() {
    ostringstream out;
    out << "kode=" << bo(kurungSeimbang("for (int i = 0; i < n; ++i) { a[i] = b[i] * 2; }"));
    out << " tandaBaca=" << bo(kurungSeimbang("a, b; c: d! e? f/g"));
    out << " angka=" << bo(kurungSeimbang("12345"));
    out << " spasi=" << bo(kurungSeimbang("     "));
    out << " kodeRusak=" << bo(kurungSeimbang("if (x > 0 { y = 1; }"));
    return out.str();
}

// Soal 4: bersarang SANGAT dalam. Stack berbasis linked list tidak boleh punya
// batas kedalaman, dan satu tanda buka yang menggantung di kedalaman itu tetap
// harus terdeteksi.
static string uji27() {
    string seimbang;
    for (int i = 0; i < 200; ++i) seimbang += "(";
    for (int i = 0; i < 200; ++i) seimbang += ")";

    string kurang;
    for (int i = 0; i < 200; ++i) kurang += "(";
    for (int i = 0; i < 199; ++i) kurang += ")";

    string campur;
    for (int i = 0; i < 60; ++i) campur += "([{";
    for (int i = 0; i < 60; ++i) campur += "}])";

    ostringstream out;
    out << "dalam200=" << bo(kurungSeimbang(seimbang));
    out << " kurangSatu=" << bo(kurungSeimbang(kurang));
    out << " campurDalam=" << bo(kurungSeimbang(campur));
    return out.str();
}

static void suiteSoal1() {
    cout << COLOR_CYAN << COLOR_BOLD
              << "\n[TEST SUITE] Soal 1 — push()   (perubahan dicatat ke puncak)"
              << COLOR_RESET << endl;

    UJI("push pada tumpukan kosong lalu satu penambahan lagi di atasnya",
        uji01,
        "push10: ret=true isi(bawah->atas)=[10] banyak=1"
        " | push20: ret=true isi(bawah->atas)=[10 20] banyak=2");

    UJI("push berturut-turut dari kosong, termasuk nilai negatif dan nol",
        uji02,
        "semuaBerhasil=ya isi(bawah->atas)=[10 20 30 -5 0] banyak=5");

    UJI("push tidak pernah ditolak — linked list tidak punya kapasitas tetap",
        uji03,
        "semuaBerhasil=ya isi(bawah->atas)=[1 2 3 4 5 6 7 8 9 10 11 12] "
        "banyak=12");

    UJI("push di atas isi yang sudah ada menjaga urutan rantai lama",
        uji04,
        "push9: ret=true isi(bawah->atas)=[7 3 9] banyak=3"
        " | push-4: ret=true isi(bawah->atas)=[7 3 9 -4] banyak=4");

    UJI("push menyediakan tepat satu node per pemanggilan, dan tumpukan yang "
        "sangat dalam tetap tertampung",
        uji21,
        "lima: semuaTrue=true nodeBaru=5/5 isi(bawah->atas)=[11 22 33 44 55] "
        "banyak=5 | dalam: semuaTrue=true banyak=150 puncak=149");
}

static void suiteSoal2() {
    cout << COLOR_CYAN << COLOR_BOLD
              << "\n[TEST SUITE] Soal 2 — pop()   (Ctrl+Z membatalkan terakhir)"
              << COLOR_RESET << endl;

    UJI("pop mengeluarkan elemen teratas dan mengisi variabel penerima",
        uji05,
        "ret=true nilai=30 isi(bawah->atas)=[10] banyak=1 dibuang=1/1 "
        "dibuangLagi=1/1");

    UJI("beberapa pop berturut-turut keluar dalam urutan kebalikan masuknya",
        uji06,
        "keluar=[40 30 20] semuaBerhasil=ya isi(bawah->atas)=[10] banyak=1");

    UJI("pop sampai tumpukan menjadi kosong",
        uji07,
        "keluar=[20 10] semuaBerhasil=ya isi(bawah->atas)=[] banyak=0");

    UJI("pop pada tumpukan kosong gagal tanpa mengubah apa pun (underflow)",
        uji08,
        "kosong: ret=false nilai=-999 isi(bawah->atas)=[] banyak=0"
        " | kontrol: ret=true nilai=50 isi(bawah->atas)=[10 20 30 40] "
        "banyak=4");

    UJI("LIFO: push 10, 20, 30, pop, push 40, pop, pop, pop",
        uji16,
        "keluar=[30 40 20 10] semuaBerhasil=ya isEmpty=true "
        "isi(bawah->atas)=[] banyak=0");

    UJI("LIFO: rangkaian yang menyentuh keadaan kosong di tengah jalan, lalu "
        "dipakai lagi dan dikosongkan",
        uji17,
        "display=\"5 4 3 2 1\" keluar=[5 4 3 2 1] popSaatKosong=false "
        "nilai=-999 | setelahDipakaiLagi: isi(bawah->atas)=[8 9] banyak=2"
        " | setelahClear: isEmpty=true isi(bawah->atas)=[] banyak=0");
}

static void suiteSoal3() {
    cout << COLOR_CYAN << COLOR_BOLD
              << "\n[TEST SUITE] Soal 3 — clear()   (Ctrl+S membuang riwayat)"
              << COLOR_RESET << endl;

    UJI("clear mengosongkan tumpukan, aman dipanggil ulang, dan tumpukan tetap "
        "dapat dipakai",
        uji13,
        "setelahClear: isi(bawah->atas)=[] banyak=0 isEmpty=true"
        " | clearLagi: isi(bawah->atas)=[] banyak=0 isEmpty=true"
        " | dipakaiLagi: ret=true nilai=77 isi(bawah->atas)=[] banyak=0");

    UJI("clear membebaskan node dalam jumlah yang tepat, baik pada satu elemen "
        "maupun pada tumpukan sepuluh elemen",
        uji22,
        "satu: dibebaskan=1/1 isEmpty=true"
        " | sepuluh: dibebaskan=10/10 isEmpty=true");

    UJI("clear sesudah sebagian elemen dikeluarkan membebaskan tepat sisanya",
        uji23,
        "dibebaskan=3/3 isEmpty=true isi(bawah->atas)=[] banyak=0");

    UJI("clear pada tumpukan yang sudah kosong tidak membebaskan node apa pun "
        "dan tetap aman",
        uji24,
        "kosong: dibebaskan=0/0 isEmpty=true"
        " | clearKedua: dibebaskan=0/0 isEmpty=true isi(bawah->atas)=[] "
        "banyak=0");
}





static void suiteSoal4() {
    cout << COLOR_CYAN << COLOR_BOLD
              << "\n[TEST SUITE] Soal 4 — kurungSeimbang()   (pemeriksa kurung)"
              << COLOR_RESET << endl;

    UJI("kurungSeimbang mengenali ekspresi yang pasangannya seimbang, termasuk "
        "bersarang dan tiga jenis kurung sekaligus",
        uji19,
        "berpasangan=true kosong=true tanpaKurung=true campuran=true "
        "bersarang=true gabungan=true bersarangDalam=true");

    UJI("kurungSeimbang mengenali kurang tutup, kurang buka, pasangan "
        "bersilangan, dan urutan terbalik",
        uji20,
        "kurangTutup=false kurangBuka=false bersilangan=false terbalik=false "
        "jenisSalah=false satuBuka=false kontrol=true");

    UJI("kurungSeimbang menolak penutup yang tidak sejenis, untuk seluruh "
        "kombinasi silang",
        uji25,
        "kurungSiku=false kurungKurawal=false sikuBulat=false "
        "sikuKurawal=false kurawalBulat=false kurawalSiku=false kontrol=true");

    UJI("kurungSeimbang mengabaikan karakter selain keenam tanda kurung",
        uji26,
        "kode=true tandaBaca=true angka=true spasi=true kodeRusak=false");

    UJI("kurungSeimbang tidak punya batas kedalaman bersarang",
        uji27,
        "dalam200=true kurangSatu=false campurDalam=true");
}

// =============================================================================
// Main
// =============================================================================

int main() {
    cout << COLOR_BOLD
              << "============================================" << endl;
    cout << " Praktikum Struktur Data C++ — Auto Checker" << endl;
    cout << " Pertemuan 5: Study Case Aplikasi Editor Tulis" << endl;
    cout << "============================================"
              << COLOR_RESET << endl;

    suiteSoal1();     // push             5 test = 25 poin
    suiteSoal2();     // pop              6 test = 30 poin
    suiteSoal3();     // clear            4 test = 20 poin
    suiteSoal4();     // kurungSeimbang   5 test = 25 poin

    // Pengaman untuk instruktur: bobot per test dihitung dari angka rencana,
    // jadi jumlah test yang benar-benar berjalan harus sama dengan rencana.
    if (total_tests != TOTAL_TEST_DIRENCANAKAN) {
        cerr << "PERINGATAN (instruktur): jumlah test berjalan ("
                  << total_tests << ") tidak sama dengan rencana ("
                  << TOTAL_TEST_DIRENCANAKAN << ")." << endl;
    }

    // -----------------------------------------------------------------------
    // Scoring Summary
    // -----------------------------------------------------------------------
    int score = (total_tests > 0) ? (passed_tests * 100 / total_tests) : 0;

    cout << "\n" << COLOR_BOLD
              << "============================================\n"
              << " SCORING SUMMARY\n"
              << "============================================\n"
              << COLOR_RESET;

    cout << " Tests Berhasil : " << COLOR_GREEN << COLOR_BOLD
              << passed_tests << COLOR_RESET << " / " << total_tests << "\n";
    cout << " Tests Gagal    : " << COLOR_RED << COLOR_BOLD
              << failed_tests << COLOR_RESET << " / " << total_tests << "\n";

    // Score line — warna hijau jika sempurna, kuning jika sebagian, merah jika 0
    string score_color = (score == 100) ? COLOR_GREEN
                            : (score > 0)    ? COLOR_YELLOW
                                             : COLOR_RED;
    cout << " Score          : " << score_color << COLOR_BOLD
              << score << " / 100" << COLOR_RESET << "\n";

    cout << COLOR_BOLD
              << "============================================\n"
              << COLOR_RESET;

    // -----------------------------------------------------------------------
    // Hasil yang dapat dibaca mesin.
    // Berkas inilah yang diunggah sebagai artifact dan dibaca aplikasi web.
    // -----------------------------------------------------------------------
    if (!write_result_json("result.json", score)) {
        cerr << "PERINGATAN: gagal menulis result.json" << endl;
    }

    if (failed_tests == 0) {
        cout << COLOR_GREEN << COLOR_BOLD
                  << " STATUS: SEMUA TEST BERHASIL ✓\n"
                  << COLOR_RESET;
        return 0; // exit code 0 = GitHub Actions SUCCESS
    } else {
        cout << COLOR_RED << COLOR_BOLD
                  << " STATUS: " << failed_tests << " TEST GAGAL ✗\n"
                  << COLOR_RESET;
        return 1; // exit code non-zero = GitHub Actions FAIL
    }
}
