#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <iomanip>
using namespace std;

struct DoUong {
    string maMon;
    string tenMon;
    string loai;
    string size;
    double gia;
    string trangThai;
};

bool docMenuTuFile(vector<DoUong> &menu) {
    ifstream file("Menu.txt");

    if (!file.is_open()) {
        cout << "Khong mo duoc file Menu.txt!\n";
        return false;
    }

    menu.clear();

    string dong;

    while (getline(file, dong)) {
        if (dong.empty()) {
            continue;
        }

        stringstream ss(dong);
        DoUong x;
        string giaText;

        getline(ss, x.maMon, '|');
        getline(ss, x.tenMon, '|');
        getline(ss, x.loai, '|');
        getline(ss, x.size, '|');
        getline(ss, giaText, '|');
        getline(ss, x.trangThai, '|');

        if (x.maMon.empty() || x.tenMon.empty() || x.loai.empty()
            || x.size.empty() || giaText.empty() || x.trangThai.empty()) {
            cout << "Dong bi thieu du lieu: " << dong << endl;
            continue;
        }

        x.gia = stod(giaText);

        menu.push_back(x);
    }

    file.close();
    return true;
}

bool ghiMenuVaoFile(const vector<DoUong> &menu) {
    ofstream file("Menu.txt");

    if (!file.is_open()) {
        cout << "Khong mo duoc file Menu.txt de ghi!\n";
        return false;
    }

    for (const DoUong &x : menu) {
        file << x.maMon << "|"
             << x.tenMon << "|"
             << x.loai << "|"
             << x.size << "|"
             << fixed << setprecision(0) << x.gia << "|"
             << x.trangThai << endl;
    }

    file.close();
    return true;
}

void hienThiMenu(const vector<DoUong> &menu) {
    cout << left
         << setw(10) << "Ma"
         << setw(25) << "Ten mon"
         << setw(15) << "Loai"
         << setw(8) << "Size"
         << setw(12) << "Gia"
         << setw(15) << "Trang thai" << endl;

    cout << string(85, '-') << endl;

    for (const DoUong &x : menu) {
        cout << left
             << setw(10) << x.maMon
             << setw(25) << x.tenMon
             << setw(15) << x.loai
             << setw(8) << x.size
             << setw(12) << fixed << setprecision(0) << x.gia
             << setw(15) << x.trangThai << endl;
    }
}

void themDoUong(vector<DoUong> &menu) {
    DoUong x;

    cout << "Nhap ma mon: ";
    getline(cin, x.maMon);

    cout << "Nhap ten mon: ";
    getline(cin, x.tenMon);

    cout << "Nhap loai: ";
    getline(cin, x.loai);

    cout << "Nhap size: ";
    getline(cin, x.size);

    cout << "Nhap gia: ";
    cin >> x.gia;
    cin.ignore();

    x.trangThai = "Con hang";

    menu.push_back(x);

    if (ghiMenuVaoFile(menu)) {
        cout << "Da them mon va luu vao file thanh cong!\n";
    }
}

int main() {
    vector<DoUong> menu;

    docMenuTuFile(menu);

    int chon;

    do {
        cout << "\n===== QUAN LY MENU =====\n";
        cout << "1. Hien thi menu\n";
        cout << "2. Them do uong\n";
        cout << "3. Doc lai file\n";
        cout << "0. Thoat\n";
        cout << "Nhap lua chon: ";
        cin >> chon;
        cin.ignore();

        switch (chon) {
        case 1:
            hienThiMenu(menu);
            break;

        case 2:
            themDoUong(menu);
            break;

        case 3:
            if (docMenuTuFile(menu)) {
                cout << "Da doc lai file Menu.txt!\n";
            }
            break;

        case 0:
            cout << "Thoat chuong trinh!\n";
            break;

        default:
            cout << "Lua chon khong hop le!\n";
        }

    } while (chon != 0);

    return 0;
}