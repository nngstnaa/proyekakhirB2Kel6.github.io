#include <iostream> //untuk input output
#include <string> //untuk fungsi-fungsi dan objek yang berkaitan dalam pemrosesan string
#include <iomanip> //untuk mengendalikan format dan tampilan keluaran 
#include <fstream> //digunakan untuk berkas I/O
#include <sstream> //untuk manipulasi string
#include <unistd.h> //untuk sistem berbasis Unix
#include <queue> //untuk implementasi antrian

using namespace std; //mengimpor seluruh anggota dari namespace std ke dalam ruang nama global

// memakai single linked list
//struct album
struct album {
    int id, jumlah_stok, harga, tahun_rilis;
    string nama;
};

//struct User
struct User {
    string username, password;
    User* next;
};

//struct Admin
struct Admin {
    string username, password;
};

//struct Node
struct Node {
    album data;
    Node* next;
};

//struct UserOrder
struct UserOrder {
    int id;
    string namaPemesan;
    string alamatPengiriman;
    album albumPemesanan;
    int jumlahPemesanan;
    string status;
    UserOrder* next;
};

//struct Pesanan
struct Pesanan {
    int idAlbum;
    int jumlahPesanan;
};

//struct pesananFile
struct PesananFile {
    int id;
    int idAlbum;
    int jumlahPesanan;
};

// Definisikan antrian pesanan
queue<Pesanan> pesananQueue; //deklarasi sebuah objek dari kelas queue dengan tipe elemen pesanan

UserOrder* userOrders = nullptr; //deklarasi variabel userOrders dengan tipe pointer ke UserOrder, nilai nullptr yang berarti pointer ini tidak menunjuk ke objek apapun.
int pesananId = 1; //digunakan untuk memberikan nomor ID yang unik kepada setiap pesanan yang dibuat atau diproses.

Node* head = nullptr; //deklarasi variabel head dengan tipe pointer ke Node, nilai pointer diatur ke nullptr yang berarti pointer ini tidak menunjuk ke node atau objek apapun.
int count = 0; //untuk melacak jumlah elemen dalam struktur data atau sesuatu yang perlu dihitung 

User* users = nullptr; //deklarasi variabel users dengan tipe pointer ke User, nilai pointer diatur ke nullptr yang berarti pointer ini tidak menunjuk ke objek apapun
Admin admin = {"admin", "admin"}; //username dan password untuk login admin

void TAMPIL(Node **head); //menampilkan data album
void dataUser(); //mengelola data pengguna (user)
void dataAlbum(); //mengelola data album
void simpanDataKeFile(); //menyimap data ke dalam file
bool cek_username(string username); //untuk memeriksa apakah sebuah username tertentu sudah ada dalam sistem 
void registerUser(); //pendaftaran pengguna (user) baru
void login(); //proses masuk admin dan user ke dalam program
void userMenu(string username); // untuk menampilkan menu dan fungsi-fungsi yang tersedia kepada user setelah login
void loginUser(); //untuk proses login user
void loginAdmin(); //untuk proses login admin 
void SORTING(); //untuk pengurutan data
void menuAwal(); //untuk menampilkan menu awal atau menu utama dalam program
void adminMenu(); //untuk menampilkan menu dan fungsi-fungsi yang tersedia kepada admin setelah login

//fungsi dataUser untuk membaca data pengguna dari berkas dan memasukkannya ke dalam struktur data yang digunakan dalam program
void dataUser() {
    fstream file; //mendeklarasikan objek file dari tipe fstream yang akan digunakan untuk membuka dan membaca berkas
    string data, username, password; //mendeklarasikan variabel string data, username, dan password yang akan digunakan untuk menyimpan data yang dibaca dari berkas
    file.open("DataRegisUser.txt", ios::in); //untuk membuka file "DataRegisterUser.txt"
    //loop while untuk membaca setiap baris dari berkas menggunakan fungsi getline()
    while (getline(file, data)) { 
        stringstream ss(data); //untuk memproses baris yang dibaca menjadi data pengguna yang terpisah.
        getline(ss, username, ','); //memisahkan data username dan password yang ada dalam baris menggunakan getline()
        getline(ss, password, '\n'); //dengan pemisah yang sesuai (',' dan '\n').

        User* newUser = new User; //membuat objek User baru dengan menggunakan operator new untuk alokasi memori dinamis.
        newUser->username = username; //mengisi atribut username dan 
        newUser->password = password; //password pada objek newUser dengan data yang telah dibaca dari berkas
        newUser->next = users; //digunakan dalam struktur data yang menghubungkan pengguna (user) satu sama lain
        users = newUser; //mengatur users untuk menunjuk ke objek newUser, sehingga objek newUser menjadi bagian dari struktur data yang mengelola pengguna
    }
    file.close(); //menutup file setelah selesai membacanya
}

//fungsi isInteger dapat digunakan untuk memeriksa apakah sebuah string berisi angka integer atau tidak
bool isInteger(const std::string& s) {
    if (s.empty()) return false; //memeriksa apakah string s kosong. Jika string kosong, maka fungsi langsung mengembalikan false, karena string kosong bukan representasi dari angka integer.
    //kemudian melakukan iterasi melalui setiap karakter dalam string s menggunakan loop for-each
    for (char c : s) { 
        if (!std::isdigit(c) && (c != '-' && c != '+')) { //kode ini memeriksa setiap karakter dalam string untuk memastikan bahwa hanya karakter angka, tanda minus, atau tanda plus yang diterima, sehingga memungkinkan untuk menentukan apakah string adalah angka integer atau tidak.
            return false; 
        }
    }
    return true; // //jika tidak ada karakter yang memicu pengembalian false dalam loop, maka string dianggap valid sebagai representasi angka integer, dan fungsi mengembalikan true.
}

//fungsi dataAlbum untuk membaca data album dari berkas teks "DataAlbum.txt" dan memprosesnya.
void dataAlbum() {
    //Membuka file "DataAlbum.txt" untuk dibaca
    fstream file;
    string data;
    file.open("DataAlbum.txt", ios::in);
    //Membaca setiap baris dari file
    while (getline(file, data)) {
        // Menggunakan stringstream untuk memproses baris menjadi potongan-potongan data
        stringstream ss(data);
        string idStr, nama, tahun_rilisStr, jumlah_stokStr, hargaStr;
        
        // Memisahkan data dengan menggunakan koma sebagai pemisah
        getline(ss, idStr, ',');
        getline(ss, nama, ',');
        getline(ss, tahun_rilisStr, ',');
        getline(ss, jumlah_stokStr, ',');
        getline(ss, hargaStr, '\n');

        // Mengecek apakah data valid sebelum mengonversi ke integer
        if (isInteger(idStr) && isInteger(tahun_rilisStr) && isInteger(jumlah_stokStr) && isInteger(hargaStr)) {
            // Membuat objek album baru dan menginisialisasi nilainya dari data yang telah divalidasi
            album newAlbum;
            newAlbum.id = stoi(idStr);
            newAlbum.nama = nama;
            newAlbum.tahun_rilis = stoi(tahun_rilisStr);
            newAlbum.jumlah_stok = stoi(jumlah_stokStr);
            newAlbum.harga = stoi(hargaStr);
            
            // Membuat node baru untuk menyimpan album tersebut
            Node* newNode = new Node;
            newNode->data = newAlbum;
            newNode->next = nullptr;

            // Memasukkan node baru ke dalam linked list
            if (head == nullptr) {
                head = newNode;
            } else {
                Node* current = head;
                while (current->next != nullptr) {
                    current = current->next;
                }
                current->next = newNode;
            }
        }
    }
    file.close(); //menutup file
}

//menyimapn data dari linked list ke dalam file
void simpanDataKeFile() {
    fstream fileOut;
    fileOut.open("DataAlbum.txt", ios::out | ios::trunc); // Membuka file untuk penulisan baru
    Node* current = head; //setiap node dalam linked list (head) diiterasi, dan data album dari setiap node ditulis ke dalam file dengan format yang sesuai

    while (current != nullptr) {
        album a = current->data;
        fileOut << a.id << "," << a.nama << "," << a.tahun_rilis << "," << a.jumlah_stok << "," << a.harga << "\n";
        current = current->next;
    }

    fileOut.close();
}

//memeriksa apakah suatu username sudah ada dalam linked list users
bool cek_username(string username) {
    User* current = users;
    while (current != nullptr) {
        if (current->username == username) {
            return false; //mengembalika false jika username sudah ada
        }
        current = current->next;
    }
    return true; //// Mengembalikan true jika username belum ada
}

//digunakan untuk melakukan registrasi pengguna
void registerUser() {
    system("cls"); // Membersihkan layar konsol
    string username, password;

    while (true) {
        cout << "===========================================" << endl;
        cout << "|           R E G I S T R A S I           |" << endl;
        cout << "===========================================" << endl;
        cout << "|   Silahkan Registrasi akun anda tanpa   |" << endl;
        cout << "|     menggunakan tanda baca spasi!       |" << endl;
        cout << "===========================================" << endl;
        while (true) {
            cout << "Masukkan Username : ";
            cin >> username;
            cout << "Masukkan Password : ";
            cin >> password;

            if (!username.empty() && !password.empty()) {
                if (cek_username(username)) {
                    // Membuat node baru untuk pengguna
                    User* newUser = new User;
                    newUser->username = username;
                    newUser->password = password;
                    newUser->next = users;
                    users = newUser;

                    // Menyimpan data pengguna ke file
                    fstream file;
                    file.open("DataRegisUser.txt", ios::out | ios::app);
                    file << username << "," << password << "\n";
                    file.close();

                    // Memberikan konfirmasi registrasi berhasil
                    cout << "===========================================" << endl;
                    cout << "|       ANDA BERHASIL REGISTER AKUN       |" << endl;
                    cout << "===========================================" << endl;
                    sleep(3); //menunggu selama 3 detik
                    system("cls"); //membersihkan layar konsol
                    menuAwal(); //memanggil fungsi awal
                    break;
                } else {
                    //memberikan pesan jika username sudah ada
                    system("cls");
                    cout << "============================================" << endl;
                    cout << "|  USERNAME YANG ANDA MASUKKAN SUDAH ADA ! |" << endl;
                    cout << "============================================" << endl;
                }
            }
        }
    }
}

//menampilkan menu login dan memanggil fungsi login yang sesuai berdasarkan pilihan pengguna
void login() {
    int pilih;
    while (true) {
        system("cls"); //membersihkan layar konsol
        cout << "==================================" << endl;
        cout << "|           Menu Login           |" << endl;
        cout << "==================================" << endl;
        cout << "| Ingin login Sebagai?           |" << endl;
        cout << "| 1. User                        |" << endl;
        cout << "| 2. Admin                       |" << endl;
        cout << "| 0. Back                        |" << endl;
        cout << "==================================" << endl;
        cout << "Pilihan : ";

        cin >> pilih;

        // Pemeriksaan kesalahan input string
        if (cin.fail()) { // Periksa jika input bukan angka
            cin.clear(); // Mengembalikan keadaan cin ke normal
            cin.ignore(INT_MAX, '\n'); // Membersihkan buffer input dengan batasan sejumlah karakter
            cout << "==================================" << endl;
            cout << "|       MENU TIDAK TERSEDIA      |" << endl;
            cout << "==================================" << endl;
            cin.get(); // Menunggu input karakter tambahan sebelum membersihkan layar
            system("cls"); // Membersihkan layar konsol
        } else {
            // Jika input berhasil, lanjutkan dengan switch
            getchar(); // Mengonsumsi karakter baru (newline) setelah memasukkan pilihan

            switch (pilih) {
                case 1:
                    loginUser(); // Memanggil fungsi loginUser jika pilihan 1 dipilih
                    break;
                case 2:
                    loginAdmin(); // Memanggil fungsi loginAdmin jika pilihan 2 dipilih
                    break;
                case 0:
                    system("cls"); // Membersihkan layar konsol
                    menuAwal(); // Kembali ke menu awal 
                    break;
                default:
                    cout << "==================================" << endl;
                    cout << "|       MENU TIDAK TERSEDIA      |" << endl;
                    cout << "==================================" << endl;
                    cin.get(); // Menunggu input karakter tambahan sebelum membersihkan layar
                    system("cls"); // Membersihkan layar konsol
                    break;
            }
        }
    }
}
//fungsi login untuk user.
void loginUser() {
    system("cls"); //membersihkan layar konsul
    string username, password;
    int coba = 1;
    while (coba <= 3) { // Memberi pengguna 3 kesempatan untuk login
        cout << "===============================================" << endl;
        cout << "|             L O G I N   U S E R             |" << endl;
        cout << "===============================================" << endl;
        cout << "|        Masukkan Username & Password         |" << endl;
        cout << "|             yang telah dibuat!              |" << endl;
        cout << "===============================================" << endl;
        cout << " Username : ";
        cin >> username;
        cout << " Password : ";
        cin >> password;

        User* current = users;
        while (current != nullptr) {
            if (username == current->username && password == current->password) {
                system("cls"); //membersihkan layar konsul
                userMenu(username); // Memanggil fungsi userMenu jika login berhasil
                break;
            }
            current = current->next;
        }
        system("cls"); //membersihkan layar konsul
        cout << "===============================================" << endl;
        cout << "| USERNAME/PASSWORD YANG ANDA MASUKKAN SALAH! |" << endl;
        cout << "===============================================" << endl;
        coba++;
    }
    system("cls"); //membersihkan layar konsul 
    cout << "====================================================" << endl;
    cout << "|     KESEMPATAN ANDA UNTUK LOGIN TELAH HABIS,     |" << endl;
    cout << "| SILAHKAN TEKAN ENTER UNTUK KEMBALI KE MENU LOGIN |" << endl;
    cout << "====================================================" << endl;
    cin.ignore(); // Mengabaikan karakter newline dari input sebelumnya
    cin.get(); // Menunggu input Enter sebelum kembali ke menu login
    system("cls"); // Membersihkan layar konsol
    login(); // Kembali ke menu login
}

//fungsi login untuk admin
void loginAdmin() {
    system("cls"); //mebersihkan layar konsol
    string username, password;
    int coba = 1;
    while (coba <= 3) { //memberikan 3 kali kesempatan untuk login
        cout << "===============================================" << endl;
        cout << "|            L O G I N   A D M I N            |" << endl;
        cout << "===============================================" << endl;
        cout << "|     Masukkan Username & Password Admin!     |" << endl;
        cout << "===============================================" << endl;
        cout << " Username : ";
        cin >> username;
        cout << " Password : ";
        cin >> password;

        if (username == admin.username && password == admin.password) {
            system("cls"); // Membersihkan layar konsol
            adminMenu(); // Memanggil fungsi adminMenu jika login berhasil
            break;
        }
        system("cls"); //membersihkan layar konsol
        cout << "===============================================" << endl;
        cout << "| USERNAME/PASSWORD YANG ANDA MASUKKAN SALAH! |" << endl;
        cout << "===============================================" << endl;
        coba++;
    }
    system("cls"); //membersihkan layar konsol
    cout << "====================================================" << endl;
    cout << "|     KESEMPATAN ANDA UNTUK LOGIN TELAH HABIS,     |" << endl;
    cout << "| SILAHKAN TEKAN ENTER UNTUK KEMBALI KE MENU LOGIN |" << endl;
    cout << "====================================================" << endl;
    cin.ignore(); // Mengabaikan karakter newline dari input sebelumnya
    cin.get(); // Menunggu input Enter sebelum kembali ke menu login
    system("cls"); // Membersihkan layar konsol
    login(); // Kembali ke menu login
}

//fungsi untuk menampilkan menu awal program
void menuAwal() {
    int pilih;
    while (true) {
        system("cls"); //membersihkan layar konsol
        cout << "===============================================" << endl;
        cout << "|           PROGRAM PEMESANAN ALBUM           |" << endl;
        cout << "|                 ARIANA GRANDE               |" << endl;
        cout << "===============================================" << endl;
        cout << "|                1. Register                  |" << endl;
        cout << "|                2. Login                     |" << endl;
        cout << "|                0. Exit                      |" << endl;
        cout << "===============================================" << endl;
        cout << "Pilihan : ";
        cin >> pilih;

        // Pemeriksaan kesalahan input string
        if (cin.fail()) {  // Periksa jika input bukan angka
            cin.clear(); // Mengembalikan keadaan cin ke normal
            cin.ignore(INT_MAX, '\n'); // Membersihkan buffer input dengan batasan sejumlah karakter
            cout << "===============================================" << endl;
            cout << "|             MENU TIDAK TERSEDIA!            |" << endl;
            cout << "===============================================" << endl;
            cin.get(); // Menunggu input karakter tambahan sebelum membersihkan layar
            system("cls"); // Membersihkan layar konsol
        } else{
            // Jika input berhasil, lanjutkan dengan switch
            getchar(); // Mengonsumsi karakter baru (newline) setelah memasukkan pilihan

            switch (pilih) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    login();
                    break;
                case 0:
                    system("cls");
                    cout << "===============================================" << endl;
                    cout << "| TERIMA KASIH TELAH MENGGUNAKAN PROGRAM KAMI |" << endl;
                    cout << "|             KELOMPOK 6 KELAS B2             |" << endl;
                    cout << "|    PROJECT AKHIR PRAKTIKUM STRUKTUR DATA    |" << endl;
                    cout << "===============================================" << endl;
                    exit(0);
                default:
                    cout << "===============================================" << endl;
                    cout << "|             MENU TIDAK TERSEDIA!            |" << endl;
                    cout << "===============================================" << endl;
                    cin.ignore(10000, '\n'); // Abaikan karakter newline dari input sebelumnya
                    cin.get(); // Tunggu input Enter sebelum melanjutkan
                    system("cls");
                    break;
            }
        }
    }
}

//untuk menambahkan data album ke dalam linked list dan juga menyimpan data tersebut ke dalam file "DataAlbum.txt"
// Fungsi untuk menambah data album
void TAMBAH(Node** head) {
    system("cls"); // Membersihkan layar konsol
    string ulang;

    while (ulang != "n") {
        Node* new_node = new Node;
        new_node->next = nullptr;

        // Memasukkan data album dari pengguna
        cout << "===========================================" << endl;
        cout << "|            MENAMBAHKAN DATA             |" << endl;
        cout << "===========================================" << endl;
        cout << "Nama Album        : ";
        cin.ignore(); // Membersihkan buffer input
        getline(cin, new_node->data.nama);

        // Memasukkan tahun rilis dengan penanganan kesalahan
        while (true) {
            cout << "Tahun Rilis       : ";
            string tahun_rilis_str;
            cin >> tahun_rilis_str;

            std::stringstream converter(tahun_rilis_str);
            if (converter >> new_node->data.tahun_rilis && converter.eof()) {
                break; // Keluar dari loop jika konversi berhasil dan tidak ada karakter tambahan
            } else {
                cout << "===================================================" << endl;
                cout << "Masukkan tahun rilis dalam bentuk angka. Coba lagi." << endl;
                cout << "===================================================" << endl;
                cin.clear(); // Membersihkan status error pada cin
                cin.ignore(32767, '\n'); // Membersihkan buffer input (32767 adalah jumlah karakter yang besar)
            }
        }

        // Memasukkan jumlah stok dengan penanganan kesalahan
        while (true) {
            cout << "Jumlah Stok Album : ";
            if (cin >> new_node->data.jumlah_stok) {
                break; // Keluar dari loop jika input sesuai
            } else {
                cout << "===================================================" << endl;
                cout << "Masukkan jumlah stok dalam bentuk angka. Coba lagi." << endl;
                cout << "===================================================" << endl;
                cin.clear(); // Membersihkan status error pada cin
                cin.ignore(32767, '\n'); // Membersihkan buffer input
            }
        }

        // Memasukkan harga album dengan penanganan kesalahan
        while (true) {
            cout << "Harga Album       : ";
            if (cin >> new_node->data.harga) {
                break; // Keluar dari loop jika input sesuai
            } else {
                cout << "===================================================" << endl;
                cout << "Masukkan harga album dalam bentuk angka. Coba lagi." << endl;
                cout << "===================================================" << endl;
                cin.clear(); // Membersihkan status error pada cin
                cin.ignore(32767, '\n'); // Membersihkan buffer input
            }
        }

        cout << "===========================================" << endl;

        // Menentukan ID baru
        int new_id = 1;  // ID awal
        if (*head != nullptr) {
            Node* current = *head;
            while (current->next != nullptr) {
                current = current->next;
            }
            new_id = current->data.id + 1;  // ID baru adalah ID terakhir + 1
        }

        // Memeriksa apakah ID baru sudah ada dalam daftar
        bool id_exists = false;
        Node* current = *head;
        while (current != nullptr) {
            if (current->data.id == new_id) {
                id_exists = true;
                break;
            }
            current = current->next;
        }

        // Jika ID sudah ada, cari ID yang belum digunakan
        if (id_exists) {
            new_id++;
            while (current != nullptr) {
                if (current->data.id == new_id) {
                    new_id++;
                    current = *head; // Ulangi pencarian dari awal
                } else {
                    current = current->next;
                }
            }
        }

        new_node->data.id = new_id;

        int posisi;
            cout << "===========================================" << endl;
            cout << "1. Tambah di awal" << endl;
            cout << "2. Tambah di tengah" << endl;
            cout << "3. Tambah di akhir" << endl;
            cout << "===========================================" << endl;

            do {
                cout << "Pilih posisi penambahan [1/2/3]: ";
                if (cin >> posisi) {
                    cout << "===========================================" << endl;

                    if (posisi < 1 || posisi > 3) {
                        cout << "Pilihan tidak valid. Harap masukkan [1/2/3]: " << endl;
                        cout << "=============================================" << endl;
                    }
                } else {
                    cout << "Input tidak valid. Harap masukkan angka [1/2/3]. " << endl;
                    cout << "===============================================" << endl;
                    cin.clear(); // Membersihkan status error pada cin
                    cin.ignore(32767, '\n'); // Membersihkan buffer input
                    posisi = 0; // Memberi nilai posisi agar loop terus berlanjut
                }
            } while (posisi < 1 || posisi > 3);

            if (posisi == 1) {
                // Tambah di awal
                new_node->next = *head;
                *head = new_node;
            } else if (posisi == 2) {
                // Tambah di tengah
                int posisi_tengah;
                cout << "===========================================" << endl;
                cout << "Masukkan posisi tengah (1-" << new_id << "): ";
                cin >> posisi_tengah;
                cout << "===========================================" << endl;

                if (posisi_tengah == 1) {
                    // Tambah di awal
                    new_node->next = *head;
                    *head = new_node;
                } else {
                    Node* current = *head;
                    Node* prev = nullptr;
                    int current_pos = 1;

                    while (current != nullptr && current_pos < posisi_tengah) {
                        prev = current;
                        current = current->next;
                        current_pos++;
                    }

                    // Tambah di tengah
                    prev->next = new_node;
                    new_node->next = current;
                }
            } else if (posisi == 3) {
                // Tambah di akhir
                if (*head == nullptr) {
                    *head = new_node;
                } else {
                    Node* current = *head;
                    while (current->next != nullptr) {
                        current = current->next;
                    }
                    current->next = new_node;
                }
            }

        // Menyimpan data ke file "DataAlbum.txt"
        fstream fileOut;
        fileOut.open("DataAlbum.txt", ios::out | ios::app);
        fileOut << new_node->data.id << "," << new_node->data.nama << "," << new_node->data.tahun_rilis << "," << new_node->data.jumlah_stok << "," << new_node->data.harga << "\n";
        fileOut.close();

        cout << endl;
        cout << "===========================================" << endl;
        cout << "|      SUKSES MENAMBAHKAN DATA ALBUM      |" << endl;
        cout << "===========================================" << endl;
        cout << endl;
        cout << "===========================================" << endl;
        cout << "Ingin menambah data lagi? [y/n] : ";
        cin >> ulang;
        cout << "===========================================" << endl;
        cin.ignore(); // Membersihkan buffer input
        cin.get();    // Menunggu input Enter sebelum kembali
        system("cls");
    }
}

// untuk menampilkan data album dari linked list ke layar konsol.
void TAMPIL(Node **head) {
    if (*head == nullptr) {
        // Menampilkan pesan jika data kosong
        cout << "===========================================" << endl;
        cout << "|               DATA KOSONG               |" << endl;
        cout << "===========================================" << endl;
    } else {
        Node* current = *head; // Menginisialisasi pointer 'current' ke head linked list
        // Menampilkan header tabel
        cout << "===================================================================================" << endl;
        cout << "|                      A L B U M   A R I A N A   G R A N D E                      |" << endl;
        cout << "===================================================================================" << endl;
        cout << "| " << setw(2) << "ID"
             << " | " << setw(20) << "NAMA ALBUM"
             << " | " << setw(15) << "TAHUN RILIS"
             << " | " << setw(15) << "JUMLAH STOK"
             << " | " << setw(15) << "HARGA"
             << " |" << endl;
        cout << "===================================================================================" << endl;

        // Iterasi melalui linked list dan menampilkan setiap data album
        int i = 0;
        while (current != nullptr) {
            album a = current->data; // Menggunakan variabel 'a' untuk mengakses data album
            // Menampilkan data album dalam format tabel
            cout << "| " << setw(2) << a.id << " | " << setw(20) << a.nama << " | " << setw(15) << a.tahun_rilis << " | " << setw(15) << a.jumlah_stok << " | " << setw(15) << a.harga << " |" << endl;
            current = current->next; // Pindah ke node berikutnya dalam linked list
            i++;
        }
        cout << "===================================================================================" << endl;
    }
    cin.ignore(); // Membersihkan buffer input
    cin.get();    // Menunggu input Enter sebelum kembali
}

//untuk mengubah data album dalam linked list berdasarkan ID yang dimasukkan pengguna. 
void UBAH(Node** head) {
    if (*head == nullptr) {
        // Menampilkan pesan jika data kosong
        cout << "===========================================" << endl;
        cout << "|               DATA KOSONG               |" << endl;
        cout << "===========================================" << endl;
    } else {
        int idToUbah;
        TAMPIL(head); // Menampilkan data album untuk referensi pengguna
        cout << "===========================================" << endl;
        cout << "|              MENGUBAH DATA              |" << endl;
        cout << "===========================================" << endl;
        
        // Memasukkan ID album dengan penanganan kesalahan
        while (true) {
            cout << "ID Album           : ";
            std::string id_str;
            cin >> id_str;

            std::stringstream id_converter(id_str);
            if (id_converter >> idToUbah && id_converter.eof()) {
                break;
            } else {
                cout << "===================================================" << endl;
                cout << "Masukkan ID album dalam bentuk angka. Coba lagi." << endl;
                cout << "===================================================" << endl;
                cin.clear();
                cin.ignore(32767, '\n');
            }
        }

        Node* current = *head;
        bool found = false;

        while (current != nullptr) {
            album& a = current->data; // Menggunakan referensi untuk mengubah data langsung pada node
            if (a.id == idToUbah) {
                cout << "Ubah Data Album Nomor " << a.id << endl;
                cout << "===========================================" << endl;
                cout << "Nama Album Terbaru        : ";
                cin.ignore();
                getline(cin, a.nama);

                // Memasukkan tahun rilis dengan penanganan kesalahan
                while (true) {
                    cout << "Tahun Rilis       : ";
                    std::string tahun_rilis_str;
                    cin >> tahun_rilis_str;

                    std::stringstream converter(tahun_rilis_str);
                    if (converter >> a.tahun_rilis && converter.eof()) {
                        break;
                    } else {
                        cout << "===================================================" << endl;
                        cout << "Masukkan tahun rilis dalam bentuk angka. Coba lagi." << endl;
                        cout << "===================================================" << endl;
                        cin.clear();
                        cin.ignore(32767, '\n');
                    }
                }

                // Memasukkan jumlah stok dengan penanganan kesalahan
                while (true) {
                    cout << "Jumlah Stok Album : ";
                    if (cin >> a.jumlah_stok) {
                        break;
                    } else {
                        cout << "===================================================" << endl;
                        cout << "Masukkan jumlah stok dalam bentuk angka. Coba lagi." << endl;
                        cout << "===================================================" << endl;
                        cin.clear();
                        cin.ignore(32767, '\n');
                    }
                }

                // Memasukkan harga album dengan penanganan kesalahan
                while (true) {
                    cout << "Harga Album       : ";
                    if (cin >> a.harga) {
                        break;
                    } else {
                        cout << "===================================================" << endl;
                        cout << "Masukkan harga album dalam bentuk angka. Coba lagi." << endl;
                        cout << "===================================================" << endl;
                        cin.clear();
                        cin.ignore(32767, '\n');
                    }
                }

                cout << "===========================================" << endl;
                found = true;
                break;
            }
            current = current->next;
        }

        if (found) {
            simpanDataKeFile(); // Menyimpan perubahan ke file
            cout << "===========================================" << endl;
            cout << "|       SUKSES MENGUBAH DATA ALBUM        |" << endl;
            cout << "===========================================" << endl;
        } else {
            cout << "===========================================" << endl;
            cout << "| Album dengan ID " << idToUbah << " tidak ditemukan! |" << endl;
            cout << "===========================================" << endl;
        }
    }

    cin.ignore(); // Membersihkan buffer input
    cin.get();    // Menunggu input Enter sebelum kembali
}

// hapus data
int inputID() {
    int id;
    string input;

    while (true) {
        cout << "Hapus Data Album Nomor : ";
        cin >> input;

        stringstream converter(input);

        if (converter >> id && converter.eof()) {
            // Input valid, keluar dari loop
            break;
        } else {
            cout << "Masukkan ID dalam bentuk angka. Coba lagi." << endl;
            cout << "===========================================" << endl;

            // Membersihkan status error pada cin
            cin.clear();

            // Membersihkan buffer input
            cin.ignore(256, '\n'); // Ganti nilai 256 dengan nilai yang diinginkan
        }
    }

    return id;
}

//fungsi untuk menghapus
void HAPUS(Node** head) {
    if (*head == nullptr) {
        // Menampilkan pesan jika data kosong
        cout << "===========================================" << endl;
        cout << "|               DATA KOSONG               |" << endl;
        cout << "===========================================" << endl;
    } else {
        int id;
        TAMPIL(head); // Menampilkan data album untuk referensi pengguna
        cout << "===========================================" << endl;
        cout << "|             MENGHAPUS DATA              |" << endl;
        cout << "===========================================" << endl;

        id = inputID();

        Node* current = *head;
        Node* prev = nullptr;

        while (current != nullptr) {
            album a = current->data;
            if (a.id == id) {
                if (prev == nullptr) {
                    *head = current->next;
                } else {
                    prev->next = current->next;
                }

                simpanDataKeFile();

                delete current;
                cout << "===========================================" << endl;
                cout << "|      SUKSES MENGHAPUS DATA ALBUM        |" << endl;
                cout << "===========================================" << endl;
                cin.ignore();
                cin.get();
                return;
            }
            prev = current;
            current = current->next;
        }

        cout << "===========================================" << endl;
        cout << "|           DATA TIDAK DITEMUKAN          |" << endl;
        cout << "===========================================" << endl;
        sleep(1.7);
        return;
    }
    cin.ignore();
    cin.get();
}

//fungsi partision digunakan untuk membagi array menjadi dua bagian, dengan elemen-elemen yang lebih 
//kecil dari pivot dipindahkan ke kiri dan elemen-elemen yang lebih besar dipindahkan ke kanan.
int partition(album* arr, int low, int high, bool ascending) {
    // Pilih pivot (dalam kasus ini, pivot diambil dari elemen terakhir)
    int pivot = arr[high].id;
    int i = low - 1;

    // Iterasi melalui elemen array
    for (int j = low; j <= high - 1; j++) {
        // Jika elemen saat ini lebih kecil (atau lebih besar, tergantung pada ascending) dari pivot
        if ((ascending && arr[j].id < pivot) || (!ascending && arr[j].id > pivot)) {
            // Tukar elemen ke posisi yang benar
            i++;
            swap(arr[i], arr[j]);
        }
    }
    // Tukar pivot ke posisi yang sesuai
    swap(arr[i + 1], arr[high]);

    // Kembalikan posisi pivot
    return (i + 1);
}

//Fungsi quickSort adalah implementasi rekursif dari algoritma quicksort yang digunakan untuk mengurutkan array album berdasarkan ID-nya. 
void quickSort(album* arr, int low, int high, bool ascending) {
    // Jika masih ada elemen yang harus diurutkan
    if (low < high) {
        // Dapatkan posisi pivot setelah membagi array
        int pi = partition(arr, low, high, ascending);

        // Panggil quickSort secara rekursif pada dua bagian array sebelum dan sesudah pivot
        quickSort(arr, low, pi - 1, ascending);
        quickSort(arr, pi + 1, high, ascending);
    }
}

//digunakan untuk mengurutkan data album berdasarkan ID menggunakan algoritma quicksort.
void SORTING() {
    //Memeriksa apakah data kosong
    if (head == nullptr) {
        cout << "===========================================" << endl;
        cout << "|               DATA KOSONG               |" << endl;
        cout << "===========================================" << endl;
        cin.ignore(); // Membersihkan buffer input
        cin.get();// Menunggu input Enter sebelum kembali
        return;
    }

    //Menghitung jumlah elemen dalam list
    int n = 0;
    Node* current = head;
    while (current != nullptr) {
        n++;
        current = current->next;
    }

    // Membuat array untuk menyimpan data album
    album* arr = new album[n];
    current = head;
    int i = 0;

    // Mengisi array dengan data album dari linked list
    while (current != nullptr) {
        arr[i] = current->data;
        current = current->next;
        i++;
    }

    // Menggunakan algoritma quicksort untuk mengurutkan array
    quickSort(arr, 0, n - 1, true);

    // Menyimpan data yang telah diurutkan ke dalam linked list
    current = head;
    i = 0;

    while (current != nullptr) {
        current->data = arr[i];
        current = current->next;
        i++;
    }
    // Menghapus array yang digunakan untuk penyimpanan sementara
    delete[] arr;
    // Menampilkan data yang telah diurutkan
    TAMPIL(&head);
}

// implementasi algoritma Boyer-Moore untuk pencarian pola dalam teks.
int boyerMooreSearch(const string& text, const string& pattern) {
    int textLength = text.length();
    int patternLength = pattern.length();

    // Membuat tabel last occurrence untuk karakter-karakter dalam pola
    int lastOccurrence[256];
    for (int i = 0; i < 256; i++) {
        lastOccurrence[i] = -1;
    }

    for (int i = 0; i < patternLength; i++) {
        lastOccurrence[(int)pattern[i]] = i;
    }

    // Memulai pencarian dari akhir pola
    int i = patternLength - 1;
    int j = patternLength - 1;

    // Melakukan pencarian di seluruh teks
    while (i < textLength) {
        if (pattern[j] == text[i]) {
            // Cocokkan karakter dari belakang
            if (j == 0) {
                return i; // Pola ditemukan, mengembalikan indeks awal pola di teks
            }
            i--;
            j--;
        } else {
            // Geser indeks pencarian berdasarkan aturan Boyer-Moore
            i = i + patternLength - min(j, 1 + lastOccurrence[(int)text[i]]);
            j = patternLength - 1;
        }
    }

    return -1; // Pola tidak ditemukan dalam teks
}

//fungsi searching untuk mencari album berdasarkan judul menggunakan algoritma Boyer-Moore yang telah diimplementasikan sebelumnya.
void SEARCHING() {
    // Meminta pengguna untuk memasukkan judul album yang ingin dicari
    string pattern;
    cout << "===================================================================================" << endl;
    cout << "|                                  MENCARI DATA                                   |" << endl;
    cout << "===================================================================================" << endl;
    cout << "Masukkan Judul Album Yang Ingin Dicari : ";
    cin >> pattern;
    cout << "===================================================================================" << endl;

    // Menginisialisasi pointer ke node saat ini dan variabel untuk menghitung hasil pencarian
    Node* current = head;
    int resultCount = 0;

    // Iterasi melalui setiap node dalam daftar album
    while (current != nullptr) {
        // Menggunakan algoritma Boyer-Moore untuk mencari judul album dalam node saat ini
        if (boyerMooreSearch(current->data.nama, pattern) != -1) {
            // Jika judul album cocok, menampilkan informasi album tersebut
            album a = current->data;
            cout << endl;
            cout << "===================================================================================" << endl;
            cout << "| " << setw(2) << "ID"
                << " | " << setw(20) << "NAMA ALBUM"
                << " | " << setw(15) << "TAHUN RILIS"
                << " | " << setw(15) << "JUMLAH STOK"
                << " | " << setw(15) << "HARGA"
                << " |" << endl;
            cout << "===================================================================================" << endl;
            cout << "| " << setw(2) << a.id << " | " << setw(20) << a.nama << " | " << setw(15) << a.tahun_rilis << " | " << setw(15) << a.jumlah_stok << " | " << setw(15) << a.harga << " |" << endl;
            cout << "===================================================================================" << endl;
            resultCount++;
        }
        // Melanjutkan pencarian ke node berikutnya
        current = current->next;
    }

    // Menampilkan pesan jika tidak ada judul album yang cocok
    if (resultCount == 0) {
        cout << endl;
        cout << "====================================================" << endl;
        cout << "| JUDUL ALBUM TIDAK ADA DI DALAM DAFTAR DATA ALBUM |" << endl;
        cout << "====================================================" << endl;
    }
    cin.ignore(); // Membersihkan buffer input
    cin.get(); // Menunggu input Enter sebelum kembali
}

//untuk menambahkan pesanan pengguna ke dalam sistem.
void tambahPesananUser(UserOrder* pesanan) {
    // Set status menjadi "menunggu proses" jika pesanan belum diproses
    // Jika status pesanan kosong, fungsi ini mengatur status pesanan sebagai "menunggu proses". 
    // Ini memastikan bahwa pesanan memiliki status yang valid.
    if (pesanan->status.empty()) {
        pesanan->status = "menunggu proses";
    }

    // Simpan pesanan ke dalam file
    // Fungsi ini membuka file "DataPesananUser.txt" dan menyimpan informasi pesanan ke dalamnya.
    fstream fileOut;
    fileOut.open("DataPesananUser.txt", ios::out | ios::app);
    fileOut << pesanan->id << "," << pesanan->namaPemesan << "," << pesanan->alamatPengiriman << ","
            << pesanan->albumPemesanan.id << "," << pesanan->jumlahPemesanan << "," << pesanan->status << "\n";
    fileOut.close();

    // Tambahkan pesanan ke awal daftar pesanan pengguna
    // Fungsi ini menambahkan pesanan ke awal daftar pesanan pengguna. Pesanan ditambahkan ke depan agar mudah diakses dan dikelola.
    pesanan->next = userOrders;
    userOrders = pesanan;
}

//fungsi untuk membaca data pesanan user dari file "DataPesananUser.txt"
void bacaDataPesananUser() {
    fstream file;
    string data;
    file.open("DataPesananUser.txt", ios::in); //membuka file

    // membaca data dari file "DataPesananUser.txt" baris per baris menggunakan getline.
    while (getline(file, data)) {
        stringstream ss(data);
        string idStr, namaPemesan, alamatPengiriman, idAlbumStr, jumlahPemesananStr;

        // Memisahkan data dari baris menjadi bagian-bagian yang sesuai
        getline(ss, idStr, ',');
        getline(ss, namaPemesan, ',');
        getline(ss, alamatPengiriman, ',');
        getline(ss, idAlbumStr, ',');
        getline(ss, jumlahPemesananStr, '\n');

        // Validasi masukan sebelum mengonversi
        if (isInteger(idStr) && isInteger(idAlbumStr) && isInteger(jumlahPemesananStr)) {
            // Membuat objek pesanan baru
            UserOrder* pesanan = new UserOrder;
            pesanan->id = std::stoi(idStr);
            pesanan->namaPemesan = namaPemesan;
            pesanan->alamatPengiriman = alamatPengiriman;
            pesanan->jumlahPemesanan = std::stoi(jumlahPemesananStr);

            // Mengonversi ID album dan mencari album sesuai ID
            int idAlbum = std::stoi(idAlbumStr);
            Node* current = head;
            bool albumDitemukan = false;

            // Mencari album dengan ID yang sesuai
            while (current != nullptr) {
                if (current->data.id == idAlbum) {
                    pesanan->albumPemesanan = current->data;
                    albumDitemukan = true;
                    break;
                }
                current = current->next;
            }

            // Jika album ditemukan, tambahkan pesanan ke daftar pesanan pengguna
            if (albumDitemukan) {
                pesanan->next = userOrders;
                userOrders = pesanan;
            }
        }
    }
    file.close(); //menutup file
}

//fungsi untuk pemesanan user
void PEMESANAN(string username) {
    system("cls");
    if (head == nullptr) {
        // Memeriksa apakah daftar album kosong
        cout << "===========================================" << endl;
        cout << "|               DATA KOSONG               |" << endl;
        cout << "===========================================" << endl;
        return;
    }

    TAMPIL(&head); // Tampilkan daftar album untuk pemilihan pesanan

    int idAlbum;
    cout << "===================================================================================" << endl;
    cout << "Masukkan ID Album yang ingin dipesan : ";
    cin >> idAlbum;
    cout << "===================================================================================" << endl;

    Node* current = head;
    bool albumDitemukan = false;
    album albumPilihan;

    // Mencari album yang dipilih berdasarkan ID
    while (current != nullptr) {
        if (current->data.id == idAlbum) {
            albumDitemukan = true;
            albumPilihan = current->data;
            break;
        }
        current = current->next;
    }

    // Memeriksa apakah album ditemukan
    if (!albumDitemukan) {
        cout << "===================================================================================" << endl;
        cout << "| Album dengan ID " << idAlbum << " tidak ditemukan |" << endl;
        cout << "===================================================================================" << endl;
        return;
    }

    // Meminta informasi pengguna untuk pesanan
    UserOrder* pesananUser = new UserOrder;
    pesananUser->albumPemesanan = albumPilihan;

    cout << "===================================================================================" << endl;
    cout << "|                                   DATA PEMESAN                                  |" << endl;
    cout << "===================================================================================" << endl;
    pesananUser->namaPemesan = username;
    cout << "Alamat Pengiriman : " ;
    cin.ignore();
    getline(cin, pesananUser->alamatPengiriman);
    cout << "Jumlah Pesanan    : ";
    cin >> pesananUser->jumlahPemesanan;
    cout << "===================================================================================" << endl;

    // Memeriksa apakah jumlah yang dipesan tidak melebihi stok
    if (pesananUser->jumlahPemesanan <= albumPilihan.jumlah_stok) {
        // Mengurangi stok album
        albumPilihan.jumlah_stok -= pesananUser->jumlahPemesanan;
        pesananUser->id = pesananId++;
        tambahPesananUser(pesananUser);

        //Jika jumlah pesanan valid, mengurangi stok album, menambahkan pesanan 
        //ke daftar pesanan pengguna, dan menambahkannya ke dalam antrian.
        Pesanan pesanan;
        pesanan.idAlbum = idAlbum;
        pesanan.jumlahPesanan = pesananUser->jumlahPemesanan;
        pesananQueue.push(pesanan);

        cout << "===========================================" << endl;
        cout << "|    ANDA BERHASIL MELAKUKAN PEMESANAN    |" << endl;
        cout << "===========================================" << endl;
    } else {
        // Jika jumlah melebihi stok, beri pesan dan hapus pesanan
        cout << "===========================================" << endl;
        cout << "|   MAAF, JUMLAH PEMESANAN MELEBIHI STOK   |" << endl;
        cout << "===========================================" << endl;
        delete pesananUser; 
    }
    cin.ignore();
    cin.get();
}

//untuk menampilkan rincian pesanan pengguna berdasarkan nama user
void RINCIANPEMESANAN(string username) {
    // Memeriksa apakah belum ada pesanan untuk pengguna tertentu
    if (userOrders == nullptr) {
        cout << "==================================================" << endl;
        cout << "|  SILAHKAN MELAKUKAN PEMESANAN TERLEBIH DAHULU  |" << endl;
        cout << "==================================================" << endl;
        
        cin.ignore();
        cin.get();
        system("cls"); // Mengosongkan layar (tidak akan pernah dieksekusi karena sudah ada return sebelumnya)
        return;
    }

    // Melakukan iterasi melalui daftar pesanan pengguna yang sesuai dengan username
    UserOrder* current = userOrders;
    cout << "===========================================" << endl;
    cout << "|             RINCIAN PEMESANAN           |" << endl;
    cout << "===========================================" << endl;
    //digunakan untuk mengiterasi melalui daftar pesanan pengguna yang sesuai dengan nama pengguna yang diberikan 
    while (current != nullptr && current->namaPemesan == username) {
        album a = current->albumPemesanan;
        cout << "ID Album          : " << a.id << endl;
        cout << "Nama Album        : " << a.nama << endl;
        cout << "Harga per Unit    : " << a.harga << endl;
        cout << "Nama Pemesan      : " << current->namaPemesan << endl;
        cout << "Alamat Pengiriman : " << current->alamatPengiriman << endl;
        cout << "Jumlah Pesanan    : " << current->jumlahPemesanan << endl;
        int totalHarga = current->jumlahPemesanan * a.harga;
        cout << "Total Harga       : " << totalHarga << endl;
        cout << "===========================================" << endl;
        current = current->next;
    }
    cin.ignore();
    cin.get();
    system("cls"); // Mengosongkan layar setelah menampilkan rincian pesanan
}

//Fungsi prosesPemesanan ini bertanggung jawab untuk memproses pesanan dari antrian.
void prosesPemesanan() {
    //Menampilkan judul dan memeriksa apakah antrian pesanan (pesananQueue) kosong. Jika kosong, cetak pesan dan kembalikan dari fungsi.
    string konfirmasi;
    cout << "===========================================" << endl;
    cout << "|              PROSES PESANAN             |" << endl;
    cout << "===========================================" << endl;

    if (pesananQueue.empty()) {
        cout << "===========================================" << endl;
        cout << "|       TIDAK ADA PESANAN SAAT INI!       |" << endl;
        cout << "===========================================" << endl;
        cin.ignore();
        cin.get();
        return;
    }

    // Panggil fungsi untuk membaca data pesanan dari file
    bacaDataPesananUser();

    queue<Pesanan> pesananCopy = pesananQueue; // Salin antrian pesanan ke antrian sementara untuk ditampilkan
    int pesananBerhasil = 0;
    int pesananDibatalkan = 0;

    // Buka file "DataAlbum.txt" dalam mode ios::out agar data sebelumnya diganti
    ofstream fileOutAlbum("DataAlbum.txt");

    while (!pesananCopy.empty()) {
        Pesanan pesanan = pesananCopy.front();

        //Iterasi melalui daftar album (head) untuk menemukan album yang sesuai dengan ID pesanan. Jika ditemukan, tampilkan rincian pesanan.
        Node* current = head;
        while (current != nullptr) {
            if (current->data.id == pesanan.idAlbum) {
                cout << "===========================================" << endl;
                cout << "ID Album          : " << pesanan.idAlbum << endl;
                cout << "Nama Album        : " << current->data.nama << endl;
                cout << "Harga per Unit    : " << current->data.harga << endl;
                cout << "Jumlah Pesanan    : " << pesanan.jumlahPesanan << endl;
                cout << "===========================================" << endl;

                // Memeriksa stok tersedia
                if (current->data.jumlah_stok >= pesanan.jumlahPesanan) {
                    cout << "Proses Pesanan Ini? (ya/tidak) : ";
                    cin >> konfirmasi;

                    if (konfirmasi == "ya") {
                        // Pesanan berhasil diproses
                        current->data.jumlah_stok -= pesanan.jumlahPesanan;
                        
                        cout << "===========================================" << endl;
                        cout << "|         PESANAN BERHASIL DIPROSES       |" << endl;
                        cout << "===========================================" << endl;

                        // Hapus pesanan yang telah diproses dari antrian
                        pesananQueue.pop();

                        // Perbarui status pesanan menjadi "Sudah diproses"
                        UserOrder* pesananUser = userOrders;
                        while (pesananUser != nullptr) {
                            if (pesananUser->albumPemesanan.id == pesanan.idAlbum) {
                                pesananUser->status = "Sudah diproses";
                                break;
                            }
                            pesananUser = pesananUser->next;
                        }
                    } else {
                        // Jika konfirmasi "tidak", tandai pesanan sebagai "Pesanan Dibatalkan".
                        cout << "===========================================" << endl;
                        cout << "|           PESANAN DIBATALKAN            |" << endl;
                        cout << "===========================================" << endl;
                    }
                } else {
                    // Jika stok tidak mencukupi, tandai pesanan sebagai "Dibatalkan".
                    cout << "===========================================" << endl;
                    cout << "|          STOK TIDAK MENCUKUPI           |" << endl;
                    cout << "|           PESANAN DIBATALKAN!           |" << endl;
                    cout << "===========================================" << endl;
                }
                break;
            }
            current = current->next;
        }
        pesananCopy.pop();
    }

    // Simpan perubahan status pesanan ke dalam file "DataPesananUser.txt"
    ofstream fileOutPesanan("DataPesananUser.txt");
    UserOrder* pesananUser = userOrders;

    while (pesananUser != nullptr) {
        fileOutPesanan << pesananUser->id << "," << pesananUser->namaPemesan << "," << pesananUser->alamatPengiriman << ","
                << pesananUser->albumPemesanan.id << "," << pesananUser->jumlahPemesanan << "," << pesananUser->status << "\n";
        pesananUser = pesananUser->next;
    }
    fileOutPesanan.close(); //menutup file

    // Simpan perubahan stok ke dalam file "DataAlbum.txt"
    Node* currentNode = head;
    while (currentNode != nullptr) {
        fileOutAlbum << currentNode->data.id << ","
                << currentNode->data.nama << ","
                << currentNode->data.tahun_rilis << ","
                << currentNode->data.jumlah_stok << ","
                << currentNode->data.harga << "\n";
        currentNode = currentNode->next;
    }
    fileOutAlbum.close(); //menutup file

    cin.ignore();
    cin.get();
}

//fungsi untuk menu user
void userMenu(string username) {
    // Menampilkan tampilan awal selamat datang dan menu utama untuk user
    int pilihan;
    cout << "=================================================" << endl;
    cout << "|            SELAMAT DATANG DI PROGRAM          |" << endl;
    cout << "|          PEMESANAN ALBUM ARIANA GRANDE        |" << endl;
    cout << "=================================================" << endl;
    cout << "|            1. MENAMPILKAN DATA ALBUM          |" << endl;
    cout << "|            2. SEARCHING ALBUM                 |" << endl;
    cout << "|            3. PEMESANAN                       |" << endl;
    cout << "|            4. RINCIAN PEMESANAN               |" << endl;
    cout << "|            5. BACK                            |" << endl;
    cout << "=================================================" << endl;
    cout << "Pilih [1-5] : ";
    cin >> pilihan;
    system("cls"); // Membersihkan layar konsol

    // Mengecek apakah input merupakan angka atau bukan
    if (cin.fail()) {
        cin.clear(); // Mengembalikan keadaan cin ke normal
        cin.ignore(INT_MAX, '\n'); // Membersihkan buffer input dengan batasan sejumlah karakter
        cout << "=================================================" << endl;
        cout << "|     PILIHAN TIDAK VALID! MASUKKAN ANGKA (1-5) |" << endl;
        cout << "=================================================" << endl;
        cin.get(); // Menunggu input karakter tambahan sebelum membersihkan layar
        system("cls"); // Membersihkan layar konsol
        userMenu(username); // Memanggil kembali fungsi userMenu
        return;
    }

    // Menggunakan switch untuk memproses opsi menu yang dipilih
    switch (pilihan) {
        case 1:
            TAMPIL(&head); //menampilkan data album
            break;
        case 2:
            SEARCHING(); //menampilkan pencarian album
            break;
        case 3:
            PEMESANAN(username); //memulai proses pemesanan
            break;
        case 4:
            RINCIANPEMESANAN(username); //menampilkan rincian pesanan
            break;
        case 5:
            system("cls"); // Membersihkan layar konsol
            menuAwal(); // Menampilkan pesan logout dan kembali ke menu awal
            return;
        default:
            cout << "=================================================" << endl;
            cout << "|   PILIHAN YANG ANDA MASUKKAN TIDAK TERSEDIA   |" << endl;
            cout << "|           SILAHKAN PILIH KEMBALI              |" << endl;
            cout << "=================================================" << endl;
            cin.get(); // Menunggu input karakter tambahan sebelum membersihkan layar
            sleep(1.7); //menunggu selama 1.7 detik
            system("cls");
            userMenu(username);
            return;
    }

    // Rekursif untuk memastikan menu terus muncul setelah pemrosesan opsi
    system("cls");
    userMenu(username);
}

//fungsi untuk menu admin
void adminMenu() {
    // Menampilkan tampilan awal selamat datang dan menu utama untuk admin
    int pilihan;
    cout << "=================================================" << endl;
    cout << "|              SELAMAT DATANG ADMIN             |" << endl;
    cout << "=================================================" << endl;
    cout << "|            1. MENAMBAH DATA ALBUM             |" << endl;
    cout << "|            2. MENAMPILKAN DATA ALBUM          |" << endl;
    cout << "|            3. MENGUBAH DATA ALBUM             |" << endl;
    cout << "|            4. MENGHAPUS DATA ALBUM            |" << endl;
    cout << "|            5. SORTING                         |" << endl;
    cout << "|            6. SEARCHING                       |" << endl;
    cout << "|            7. PROSES PESANAN                  |" << endl;
    cout << "|            8. BACK                            |" << endl;
    cout << "=================================================" << endl;
    cout << "Pilih [1-8] : ";
    cin >> pilihan;
    system("cls"); //membersihkan layar konsol

    // Mengecek apakah input merupakan angka atau bukan
    if (cin.fail()) {
        cin.clear(); // Mengembalikan keadaan cin ke normal
        cin.ignore(INT_MAX, '\n'); // Membersihkan buffer input dengan batasan sejumlah karakter
        cout << "=================================================" << endl;
        cout << "|   PILIHAN TIDAK VALID! MASUKKAN ANGKA (1-8)   |" << endl;
        cout << "=================================================" << endl;
        cin.get(); // Menunggu input karakter tambahan sebelum membersihkan layar
        system("cls"); // Membersihkan layar konsol
        adminMenu(); // Memanggil kembali fungsi adminMenu
        return;
    }

    // Menggunakan switch untuk memproses opsi menu yang dipilih
    switch (pilihan) {
        case 1:
            TAMBAH(&head); //menambah data album
            break;
        case 2:
            TAMPIL(&head); //menampilkan data album
            break;
        case 3:
            UBAH(&head); //mengubah data album
            break;
        case 4:
            HAPUS(&head); //menghapus data admin
            break;
        case 5:
            SORTING(); //mengurutkan data album berdasarkan ID
            break;
        case 6:
            SEARCHING(); //mencari data album berdasarkan nama album
            break;
        case 7:
            prosesPemesanan(); //memproses pesanan
            break;
        case 8:
            system("cls"); // Menampilkan pesan logout dan kembali ke menu awal
            menuAwal();
            return;
        default:
            cout << "=================================================" << endl;
            cout << "|   PILIHAN YANG ANDA MASUKKAN TIDAK TERSEDIA   |" << endl;
            cout << "|           SILAHKAN PILIH KEMBALI              |" << endl;
            cout << "=================================================" << endl;
            cin.get(); // Menunggu input karakter tambahan sebelum membersihkan layar
            sleep(1.7); //menunggu selama 1.7 detik
            system("cls");
            adminMenu();
            return;
    }

    // Rekursif untuk memastikan menu terus muncul setelah pemrosesan opsi
    system("cls");
    adminMenu();
}

int main() {
    dataUser();  // Menginisialisasi data pengguna
    dataAlbum();  // Menginisialisasi data album
    bacaDataPesananUser();  // Membaca data pesanan pengguna dari file
    menuAwal();  // Menampilkan menu awal aplikasi
    return 0;
}