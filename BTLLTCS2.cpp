#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <stdexcept>
#include <sstream>
#include <limits>
using namespace std;
struct DoUong {
    string maMon;
    string tenMon;
    string loai;
    string size;
    double gia;
    string trangThai;
};
struct Ban {
    int soBan;
    string viTri;
    int soGhe;
    string trangThai; // Trong / Co khach
};
struct MonDaGoi {
    DoUong mon;
    int soLuong;
};
struct HoaDon {
    int maHD;
    int soBan;
    vector<MonDaGoi> dsMon;
    double tongTien;
    double vat;
    double giamGia;
    double thanhTien;
    string thoiGian;
    string ca;
};
struct BanGhiHoaDon {
    int maHD;
    int ngay;
    int thang;
    int nam;
    string ca;
    double thanhTien;
};
struct BanGhiChiTiet {
    int maHD;
    int ngay;
    int thang;
    int nam;
    string ca;
    string maMon;
    string tenMon;
    int soLuong;
    double doanhThuMon;
};
const string FILE_DOANH_THU = "DataCafe.txt";
// HAM LAY THOI GIAN 
int layNgayHienTai() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return ltm->tm_mday;
}
int layThangHienTai() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return 1 + ltm->tm_mon;
}
int layNamHienTai() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return 1900 + ltm->tm_year;
}
int layGioHienTai() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return ltm->tm_hour;
}
string layThoiGian() {
    return to_string(layNgayHienTai()) + "/" +
           to_string(layThangHienTai()) + "/" +
           to_string(layNamHienTai());
}
string layCaHienTai() {
    int gio = layGioHienTai();
    if (gio >= 6 && gio < 12) {
        return "Sang";
    }
    else if (gio >= 12 && gio < 18) {
        return "Chieu";
    }
    else {
        return "Toi";
    }
}
int laySoTuanTrongNam(int ngay, int thang, int nam) {
    tm timeInfo = {};
    timeInfo.tm_mday = ngay;
    timeInfo.tm_mon = thang - 1;
    timeInfo.tm_year = nam - 1900;
    mktime(&timeInfo);
    return timeInfo.tm_yday / 7 + 1;
}
//  KHOI TAO DU LIEU 
bool docMenuTuFile(vector<DoUong> &menu) {
    ifstream file("Menu.txt");
    try {
        if (!file.is_open()) {
            throw runtime_error("Khong mo duoc file Menu.txt");
        }
        menu.clear();
        string dong;
        int soDong = 0;
        while (getline(file, dong)) {
            soDong++;
            if (dong.empty()) {
                continue;
            }
            try {
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
                    throw invalid_argument("Thieu du lieu");
                }
                x.gia = stod(giaText);
                if (x.gia <= 0) {
                    throw invalid_argument("Gia phai lon hon 0");
                }
                if (x.trangThai != "Con hang" && x.trangThai != "Het hang") {
                    throw invalid_argument("Trang thai phai la Con hang hoac Het hang");
                }
                menu.push_back(x);
            }
            catch (const exception &e) {
                cout << "Loi dong " << soDong << " trong Menu.txt: "
                     << e.what() << endl;
            }
        }
        file.close();
        if (menu.empty()) {
            throw runtime_error("Menu.txt khong co du lieu hop le");
        }
        return true;
    }
    catch (const exception &e) {
        cout << "Loi doc menu: " << e.what() << endl;
        return false;
    }
}
void ghiMenuVaoFile(const vector<DoUong> &menu) {
    ofstream file("Menu.txt");
    if (!file.is_open()) {
        cout << "Khong mo duoc file Menu.txt de ghi!\n";
        return;
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
}
bool docBanTuFile(vector<Ban> &dsBan) {
    ifstream file("Ban.txt");
    try {
        if (!file.is_open()) {
            throw runtime_error("Khong mo duoc file Ban.txt");
        }
        dsBan.clear();
        string dong;
        int soDong = 0;
        while (getline(file, dong)) {
            soDong++;
            if (dong.empty()) {
                continue;
            }
            try {
                stringstream ss(dong);
                Ban b;
                string soBanText;
                string soGheText;
                getline(ss, soBanText, '|');
                getline(ss, b.viTri, '|');
                getline(ss, soGheText, '|');
                getline(ss, b.trangThai, '|');
                if (soBanText.empty() || b.viTri.empty()
                    || soGheText.empty() || b.trangThai.empty()) {
                    throw invalid_argument("Thieu du lieu");
                }
                b.soBan = stoi(soBanText);
                b.soGhe = stoi(soGheText);
                if (b.soBan <= 0) {
                    throw invalid_argument("So ban phai lon hon 0");
                }
                if (b.soGhe <= 0) {
                    throw invalid_argument("So ghe phai lon hon 0");
                }
                if (b.trangThai != "Trong" && b.trangThai != "Co khach") {
                    throw invalid_argument("Trang thai ban phai la Trong hoac Co khach");
                }
                dsBan.push_back(b);
            }
            catch (const exception &e) {
                cout << "Loi dong " << soDong << " trong Ban.txt: "
                     << e.what() << endl;
            }
        }
        file.close();
        if (dsBan.empty()) {
            throw runtime_error("Ban.txt khong co du lieu hop le");
        }
        return true;
    }
    catch (const exception &e) {
        cout << "Loi doc ban: " << e.what() << endl;
        return false;
    }
}
void ghiBanVaoFile(const vector<Ban> &dsBan) {
    ofstream file("Ban.txt");
    if (!file.is_open()) {
        cout << "Khong mo duoc file Ban.txt de ghi!\n";
        return;
    }
    for (const Ban &b : dsBan) {
        file << b.soBan << "|"
             << b.viTri << "|"
             << b.soGhe << "|"
             << b.trangThai << endl;
    }
    file.close();
}
//  HIEN THI MENU 
void hienThiMenu(const vector<DoUong> &menu) {
    cout << "\n========== MENU DO UONG ==========\n";
    cout << left << setw(10) << "Ma"
         << setw(25) << "Ten mon"
         << setw(15) << "Loai"
         << setw(10) << "Size"
         << setw(10) << "Gia"
         << "Trang thai" << endl;
    for (const DoUong &x : menu) {
        cout << left << setw(10) << x.maMon
             << setw(25) << x.tenMon
             << setw(15) << x.loai
             << setw(10) << x.size
             << setw(10) << fixed << setprecision(0) <<x.gia <<x.trangThai << endl;
    }
}
//  HIEN THI BAN 
void hienThiBan(const vector<Ban> &dsBan) {
    cout << "\n========== DANH SACH BAN ==========\n";
    cout << left << setw(10) << "So ban"
         << setw(15) << "Vi tri"
         << setw(10) << "So ghe"
         << "Trang thai" << endl;
    for (const Ban &b : dsBan) {
        cout << left << setw(10) << b.soBan
             << setw(15) << b.viTri
             << setw(10) << b.soGhe
             << b.trangThai << endl;
    }
}
//  TIM MON THEO MA 
int timMon(const vector<DoUong> &menu, const string &ma) {
    for (size_t i = 0; i < menu.size(); i++) {
        if (menu[i].maMon == ma) {
            return static_cast<int>(i);
        }
    }
    return -1;
}
void themDoUong(vector<DoUong> &menu) {
    DoUong x;

    cout << "Nhap ma mon: ";
    cin>>x.maMon;
    cin.ignore(1000,'\n');

    if (timMon(menu, x.maMon) != -1) {
        cout << "Ma mon da ton tai!\n";
        return;
    }

    cout << "Nhap ten mon: ";
    getline(cin, x.tenMon);

    cout << "Nhap loai: ";
    getline(cin, x.loai);

    cout << "Nhap size: ";
    getline(cin, x.size);

    cout << "Nhap gia: ";
    cin >> x.gia;
    cin.ignore(1000, '\n');

    x.trangThai = "Con hang";

    try {
        if (x.maMon.empty() || x.tenMon.empty() || x.loai.empty() || x.size.empty()) {
            throw invalid_argument("Thong tin do uong khong duoc bo trong");
        }

        if (x.gia <= 0) {
            throw invalid_argument("Gia do uong phai lon hon 0");
        }
    }
    catch (const exception &e) {
        cout << "Loi them do uong: " << e.what() << endl;
        return;
    }

    menu.push_back(x);
    ghiMenuVaoFile(menu);

    cout << "Da them do uong va luu vao file!\n";
}
void suaDoUong(vector<DoUong> &menu) {
    string ma;
    cout << "Nhap ma mon can sua: ";
    cin>>ma;
    cin.ignore(1000,'\n');
    int vt = timMon(menu, ma);
    if (vt == -1) {
        cout << "Khong tim thay mon!\n";
        return;
    }
    cout << "Nhap ten mon moi: ";
    getline(cin, menu[vt].tenMon);
    cout << "Nhap loai moi: ";
    getline(cin, menu[vt].loai);
    cout << "Nhap size moi: ";
    getline(cin, menu[vt].size);
    cout<<"Nhap gia moi: ";
    cin>>menu[vt].gia;
    cin.ignore(1000,'\n');
    ghiMenuVaoFile(menu);
    cout << "Da sua do uong va luu vao file!\n";
}
void xoaDoUong(vector<DoUong> &menu) {
    string ma;
    cout << "Nhap ma mon can xoa: ";
    cin>>ma;
    cin.ignore(1000,'\n');
    int vt = timMon(menu, ma);
    if (vt == -1) {
        cout << "Khong tim thay mon!\n";
        return;
    }
    menu.erase(menu.begin() + vt);
    ghiMenuVaoFile(menu);
    cout << "Da xoa do uong va luu vao file!\n";
}
void capNhatTrangThaiDoUong(vector<DoUong> &menu) {
    string ma;
    cout << "Nhap ma mon can cap nhat: ";
    cin>>ma;
    cin.ignore(1000,'\n');
    int vt = timMon(menu, ma);
    if (vt == -1) {
        cout << "Khong tim thay mon!\n";
        return;
    }
    cout << "1. Con hang\n";
    cout << "2. Het hang\n";
    int chon;
    cout<<"Nhap trang thai: ";
    cin>>chon;
    if (chon == 1) {
        menu[vt].trangThai = "Con hang";
    }
    else if (chon == 2) {
        menu[vt].trangThai = "Het hang";
    }
    else {
        cout << "Lua chon khong hop le!\n";
        return;
    }
    ghiMenuVaoFile(menu);
    cout << "Da cap nhat trang thai va luu vao file!\n";
}
void quanLyMenuDoUong(vector<DoUong> &menu) {
    int luaChon;
    do {
        docMenuTuFile(menu);
        cout << "\n========== QUAN LY MENU DO UONG ==========\n";
        cout << "1. Xem menu do uong\n";
        cout << "2. Them do uong\n";
        cout << "3. Sua thong tin do uong\n";
        cout << "4. Xoa do uong\n";
        cout << "5. Cap nhat con hang / het hang\n";
        cout << "0. Quay lai\n";
        cout<<"Nhap lua chon: ";
        cin>>luaChon;
        switch (luaChon) {
        case 1:
            hienThiMenu(menu);
            break;
        case 2:
            themDoUong(menu);
            break;
        case 3:
            suaDoUong(menu);
            break;
        case 4:
            xoaDoUong(menu);
            break;
        case 5:
            capNhatTrangThaiDoUong(menu);
            break;
        case 0:
            cout << "Quay lai menu chinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);
}
//  GOI MON 
void goiMon(const vector<DoUong> &menu, vector<MonDaGoi> &dsMon) {
    string ma;
    int soLuong;
    char tiepTuc;
    do {
        hienThiMenu(menu);
        cout << "\nNhap ma mon muon goi: ";
        getline(cin, ma);
        int vt = timMon(menu, ma);
        if (vt == -1) {
            cout << "Khong tim thay mon!\n";
        }
        else if (menu[vt].trangThai == "Het hang") {
            cout << "Mon nay da het hang!\n";
        }
        else {
            cout<<"Nhap so luong: ";
            cin>>soLuong;
            cin.ignore(1000, '\n');
            if (soLuong <= 0) {
                cout << "So luong phai lon hon 0!\n";
            }
            else {
                bool daTonTai = false;
                for (MonDaGoi &x : dsMon) {
                    if (x.mon.maMon == menu[vt].maMon) {
                        x.soLuong += soLuong;
                        daTonTai = true;
                        break;
                    }
                }
                if (!daTonTai) {
                    dsMon.push_back({menu[vt], soLuong});
                }
                cout << "Da them mon vao hoa don!\n";
            }
        }
        cout << "Ban co muon goi tiep khong? (y/n): ";
        cin >> tiepTuc;
        cin.ignore(1000, '\n');
    } while (tiepTuc == 'y' || tiepTuc == 'Y');
}
//  TINH TIEN 
void tinhTien(HoaDon &hd, bool vip) {
    hd.tongTien = 0;
    for (const MonDaGoi &x : hd.dsMon) {
        hd.tongTien += x.mon.gia * x.soLuong;
    }
    hd.vat = hd.tongTien * 0.08;
    if (vip) {
        hd.giamGia = hd.tongTien * 0.10;
    }
    else {
        hd.giamGia = 0;
    }
    hd.thanhTien = hd.tongTien + hd.vat - hd.giamGia;
}
//  IN HOA DON 
void inHoaDon(const HoaDon &hd, bool vip) {
    cout << "\n================ HOA DON ================\n";
    cout << "Ma hoa don: " << hd.maHD << endl;
    if (hd.soBan == 0) {
        cout << "Hinh thuc: Mang di\n";
    }
    else {
        cout << "So ban: " << hd.soBan << endl;
    }
    cout << "Ngay: " << hd.thoiGian << endl;
    cout << "Ca: " << hd.ca << endl;
    cout << "Khach VIP: " << (vip ? "Co" : "Khong") << endl;
    cout << "\nDanh sach mon:\n";
    cout << left << setw(25) << "Ten mon"
         << setw(10) << "SL"
         << setw(12) << "Don gia"
         << "Thanh tien" << endl;
    for (const MonDaGoi &x : hd.dsMon) {
        double thanhTienMon = x.mon.gia * x.soLuong;
        cout << left << setw(25) << x.mon.tenMon
             << setw(10) << x.soLuong
             << setw(12) << fixed << setprecision(0) << x.mon.gia
             << fixed << setprecision(0) << thanhTienMon << endl;
    }
    cout << "\nTong tien mon: " << fixed << setprecision(0) << hd.tongTien << endl;
    cout << "VAT 8%: " << fixed << setprecision(0) << hd.vat << endl;
    cout << "Giam gia VIP: " << fixed << setprecision(0) << hd.giamGia << endl;
    cout << "Thanh tien: " << fixed << setprecision(0) << hd.thanhTien << endl;
    cout << "=======\n";
}
//  LUU DOANH THU VAO FILE 
// File co 2 loai dong:
// HD|maHD|ngay|thang|nam|ca|thanhTienHoaDon
// CT|maHD|ngay|thang|nam|ca|maMon|tenMon|soLuong|doanhThuMon
void luuDoanhThu(const HoaDon &hd) {
    ofstream file(FILE_DOANH_THU, ios::app);
    if (!file.is_open()) {
        cout << "Khong mo duoc file doanhthu.txt!\n";
        return;
    }
    file << "HD|"
         << hd.maHD << "|"
         << layNgayHienTai() << "|"
         << layThangHienTai() << "|"
         << layNamHienTai() << "|"
         << hd.ca << "|"
         << fixed << setprecision(0) << hd.thanhTien
         << endl;
    for (const MonDaGoi &x : hd.dsMon) {
        double doanhThuMon = x.mon.gia * x.soLuong;
        file << "CT|"
             << hd.maHD << "|"
             << layNgayHienTai() << "|"
             << layThangHienTai() << "|"
             << layNamHienTai() << "|"
             << hd.ca << "|"
             << x.mon.maMon << "|"
             << x.mon.tenMon << "|"
             << x.soLuong << "|"
             << fixed << setprecision(0) << doanhThuMon
             << endl;
    }
    file.close();
}
//  DOC FILE DOANH THU 
vector<BanGhiHoaDon> docHoaDonTuFile() {
    vector<BanGhiHoaDon> ds;
    ifstream file(FILE_DOANH_THU);
    if (!file.is_open()) {
        return ds;
    }
    string dong;
    int soDong = 0;
    while (getline(file, dong)) {
        soDong++;
        if (dong.empty()) {
            continue;
        }
        try {
            stringstream ss(dong);
            string loaiDong;
            getline(ss, loaiDong, '|');
            if (loaiDong != "HD") {
                continue;
            }
            string maHDText, ngayText, thangText, namText, tienText;
            BanGhiHoaDon bg;
            getline(ss, maHDText, '|');
            getline(ss, ngayText, '|');
            getline(ss, thangText, '|');
            getline(ss, namText, '|');
            getline(ss, bg.ca, '|');
            getline(ss, tienText, '|');
            if (maHDText.empty() || ngayText.empty() || thangText.empty()
                || namText.empty() || bg.ca.empty() || tienText.empty()) {
                throw invalid_argument("Thieu du lieu hoa don");
            }
            bg.maHD = stoi(maHDText);
            bg.ngay = stoi(ngayText);
            bg.thang = stoi(thangText);
            bg.nam = stoi(namText);
            bg.thanhTien = stod(tienText);
            ds.push_back(bg);
        }
        catch (const exception &e) {
            cout << "Loi dong " << soDong << " trong DataCafe.txt: "
                 << e.what() << endl;
        }
    }
    file.close();
    return ds;
}
vector<BanGhiChiTiet> docChiTietTuFile() {
    vector<BanGhiChiTiet> ds;
    ifstream file(FILE_DOANH_THU);
    if (!file.is_open()) {
        return ds;
    }
    string dong;
    int soDong = 0;
    while (getline(file, dong)) {
        soDong++;
        if (dong.empty()) {
            continue;
        }
        try {
            stringstream ss(dong);
            string loaiDong;
            getline(ss, loaiDong, '|');
            if (loaiDong != "CT") {
                continue;
            }
            string maHDText, ngayText, thangText, namText;
            string soLuongText, tienText;
            BanGhiChiTiet bg;
            getline(ss, maHDText, '|');
            getline(ss, ngayText, '|');
            getline(ss, thangText, '|');
            getline(ss, namText, '|');
            getline(ss, bg.ca, '|');
            getline(ss, bg.maMon, '|');
            getline(ss, bg.tenMon, '|');
            getline(ss, soLuongText, '|');
            getline(ss, tienText, '|');
            if (maHDText.empty() || ngayText.empty() || thangText.empty()
                || namText.empty() || bg.ca.empty() || bg.maMon.empty()
                || bg.tenMon.empty() || soLuongText.empty() || tienText.empty()) {
                throw invalid_argument("Thieu du lieu chi tiet");
            }
            bg.maHD = stoi(maHDText);
            bg.ngay = stoi(ngayText);
            bg.thang = stoi(thangText);
            bg.nam = stoi(namText);
            bg.soLuong = stoi(soLuongText);
            bg.doanhThuMon = stod(tienText);
            if (bg.soLuong <= 0) {
                throw invalid_argument("So luong phai lon hon 0");
            }
            ds.push_back(bg);
        }
        catch (const exception &e) {
            cout << "Loi dong " << soDong << " trong DataCafe.txt: "
                 << e.what() << endl;
        }
    }
    file.close();
    return ds;
}
//  CAC HAM BAO CAO 
double tinhDoanhThuTheoNgay(int ngay, int thang, int nam) {
    vector<BanGhiHoaDon> ds = docHoaDonTuFile();
    double tong = 0;
    for (const BanGhiHoaDon &bg : ds) {
        if (bg.ngay == ngay && bg.thang == thang && bg.nam == nam) {
            tong += bg.thanhTien;
        }
    }
    return tong;
}
double tinhDoanhThuTheoThang(int thang, int nam) {
    vector<BanGhiHoaDon> ds = docHoaDonTuFile();
    double tong = 0;
    for (const BanGhiHoaDon &bg : ds) {
        if (bg.thang == thang && bg.nam == nam) {
            tong += bg.thanhTien;
        }
    }
    return tong;
}
double tinhDoanhThuTheoNam(int nam) {
    vector<BanGhiHoaDon> ds = docHoaDonTuFile();
    double tong = 0;
    for (const BanGhiHoaDon &bg : ds) {
        if (bg.nam == nam) {
            tong += bg.thanhTien;
        }
    }
    return tong;
}
void baoCaoTheoNgay() {
    int ngay;
    cout<<"Nhap ngay: ";
    cin>>ngay;
    int thang;
    cout<<"Nhap thang: ";
    cin>>thang;
    int nam;
    cout<<"Nhap nam: ";
    cin>>nam;
    double tong = tinhDoanhThuTheoNgay(ngay, thang, nam);
    cout << "\nDoanh thu ngay " << ngay << "/" << thang << "/" << nam
         << ": " << fixed << setprecision(0) << tong << endl;
}
void baoCaoTheoThang() {
    int thang;
    cout<<"Nhap thang: ";
    cin>>thang;
    int nam;
    cout<<"Nhap nam: ";
    cin>>nam;
    double tong = tinhDoanhThuTheoThang(thang, nam);
    cout << "\nDoanh thu thang " << thang << "/" << nam
         << ": " << fixed << setprecision(0) << tong << endl;
}
void baoCaoTheoNam() {
    int nam;
    cout<<"Nhap nam: ";
    cin>>nam;
    double tong = tinhDoanhThuTheoNam(nam);
    cout << "\nDoanh thu nam " << nam
         << ": " << fixed << setprecision(0) << tong << endl;
}
void thongKeDoanhThuTheoCa() {
    vector<BanGhiHoaDon> ds = docHoaDonTuFile();
    int ngay;
    cout<<"Nhap ngay: ";
    cin>>ngay;
    int thang;
    cout<<"Nhap thang: ";
    cin>>thang;
    int nam;
    cout<<"Nhap nam: ";
    cin>>nam;
    double sang = 0;
    double chieu = 0;
    double toi = 0;
    for (const BanGhiHoaDon &bg : ds) {
        if (bg.ngay == ngay && bg.thang == thang && bg.nam == nam) {
            if (bg.ca == "Sang") {
                sang += bg.thanhTien;
            }
            else if (bg.ca == "Chieu") {
                chieu += bg.thanhTien;
            }
            else if (bg.ca == "Toi") {
                toi += bg.thanhTien;
            }
        }
    }
    cout << "\n========== DOANH THU THEO CA ==========\n";
    cout << "Ngay: " << ngay << "/" << thang << "/" << nam << endl;
    cout << "Ca sang: " << fixed << setprecision(0) << sang << endl;
    cout << "Ca chieu: " << fixed << setprecision(0) << chieu << endl;
    cout << "Ca toi: " << fixed << setprecision(0) << toi << endl;
    cout << "Tong: " << fixed << setprecision(0) << sang + chieu + toi << endl;
}
void soSanhCungKyThangTruoc() {
    int ngay = layNgayHienTai();
    int thang = layThangHienTai();
    int nam = layNamHienTai();
    int thangTruoc = thang - 1;
    int namThangTruoc = nam;
    if (thangTruoc == 0) {
        thangTruoc = 12;
        namThangTruoc--;
    }
    double hienTai = tinhDoanhThuTheoNgay(ngay, thang, nam);
    double thangTruocDT = tinhDoanhThuTheoNgay(ngay, thangTruoc, namThangTruoc);
    cout << "\n========== SO SANH CUNG KY THANG TRUOC ==========\n";
    cout << "Hom nay: " << ngay << "/" << thang << "/" << nam << endl;
    cout << "Cung ky thang truoc: " << ngay << "/" << thangTruoc << "/" << namThangTruoc << endl;
    cout << "Doanh thu hom nay: " << fixed << setprecision(0) << hienTai << endl;
    cout << "Doanh thu cung ky thang truoc: " << fixed << setprecision(0) << thangTruocDT << endl;
    if (thangTruocDT == 0) {
        cout << "Khong co du lieu thang truoc de so sanh!\n";
        return;
    }
    double chenhlech = hienTai - thangTruocDT;
    double phanTram = chenhlech / thangTruocDT * 100;
    if (chenhlech > 0) {
        cout << "Doanh thu tang: " << fixed << setprecision(2) << phanTram << "%\n";
    }
    else if (chenhlech < 0) {
        cout << "Doanh thu giam: " << fixed << setprecision(2) << -phanTram << "%\n";
    }
    else {
        cout << "Doanh thu khong thay doi.\n";
    }
}
struct XepHangMon {
    string maMon;
    string tenMon;
    int soLuong;
};
int timMonTrongXepHang(const vector<XepHangMon> &dsXepHang, const string &maMon) {
    for (int i = 0; i < static_cast<int>(dsXepHang.size()); i++) {
        if (dsXepHang[i].maMon == maMon) {
            return i;
        }
    }
    return -1;
}
void sapXepMonBanChay(vector<XepHangMon> &dsXepHang) {
    for (int i = 0; i < static_cast<int>(dsXepHang.size()) - 1; i++) {
        for (int j = i + 1; j < static_cast<int>(dsXepHang.size()); j++) {
            if (dsXepHang[j].soLuong > dsXepHang[i].soLuong) {
                XepHangMon tam = dsXepHang[i];
                dsXepHang[i] = dsXepHang[j];
                dsXepHang[j] = tam;
            }
        }
    }
}
void xepHangMonBanChay(int kieuThongKe) {
    vector<BanGhiChiTiet> ds = docChiTietTuFile();
    if (ds.empty()) {
        cout << "Chua co du lieu chi tiet doanh thu!\n";
        return;
    }
    int ngay = layNgayHienTai();
    int thang = layThangHienTai();
    int nam = layNamHienTai();
    int tuanHienTai = laySoTuanTrongNam(ngay, thang, nam);
    vector<XepHangMon> dsXepHang;
    for (const BanGhiChiTiet &bg : ds) {
        bool hopLe = false;
        if (kieuThongKe == 1) {
            if (bg.ngay == ngay && bg.thang == thang && bg.nam == nam) {
                hopLe = true;
            }
        }
        else if (kieuThongKe == 2) {
            int tuanCuaDong = laySoTuanTrongNam(bg.ngay, bg.thang, bg.nam);
            if (tuanCuaDong == tuanHienTai && bg.nam == nam) {
                hopLe = true;
            }
        }
        else if (kieuThongKe == 3) {
            if (bg.thang == thang && bg.nam == nam) {
                hopLe = true;
            }
        }
        if (hopLe) {
            int viTri = timMonTrongXepHang(dsXepHang, bg.maMon);
            if (viTri == -1) {
                XepHangMon mon;
                mon.maMon = bg.maMon;
                mon.tenMon = bg.tenMon;
                mon.soLuong = bg.soLuong;
                dsXepHang.push_back(mon);
            }
            else {
                dsXepHang[viTri].soLuong += bg.soLuong;
            }
        }
    }
    if (dsXepHang.empty()) {
        cout << "Khong co du lieu trong khoang thoi gian nay!\n";
        return;
    }
    sapXepMonBanChay(dsXepHang);
    cout << "\n========== XEP HANG MON BAN CHAY ==========\n";
    if (kieuThongKe == 1) {
        cout << "Thong ke: Hom nay\n";
    }
    else if (kieuThongKe == 2) {
        cout << "Thong ke: Tuan nay\n";
    }
    else {
        cout << "Thong ke: Thang nay\n";
    }
    cout << left << setw(10) << "Hang"
         << setw(10) << "Ma mon"
         << setw(25) << "Ten mon"
         << "So luong ban" << endl;
    for (int i = 0; i < static_cast<int>(dsXepHang.size()); i++) {
        cout << left << setw(10) << i + 1
             << setw(10) << dsXepHang[i].maMon
             << setw(25) << dsXepHang[i].tenMon
             << dsXepHang[i].soLuong << endl;
    }
}
//  BAO CAO DOANH THU 
void baoCaoDoanhThu() {
    int luaChon;
    do {
        cout << "\n========== BAO CAO DOANH THU ==========\n";
        cout << "1. Doanh thu theo ngay\n";
        cout << "2. Doanh thu theo thang\n";
        cout << "3. Doanh thu theo nam\n";
        cout << "4. Doanh thu theo ca\n";
        cout << "5. So sanh hom nay voi cung ky thang truoc\n";
        cout << "6. Xep hang mon ban chay hom nay\n";
        cout << "7. Xep hang mon ban chay tuan nay\n";
        cout << "8. Xep hang mon ban chay thang nay\n";
        cout << "0. Quay lai\n";
        cout<<"Nhap lua chon: ";
        cin>>luaChon;
        switch (luaChon) {
        case 1:
            baoCaoTheoNgay();
            break;
        case 2:
            baoCaoTheoThang();
            break;
        case 3:
            baoCaoTheoNam();
            break;
        case 4:
            thongKeDoanhThuTheoCa();
            break;
        case 5:
            soSanhCungKyThangTruoc();
            break;
        case 6:
            xepHangMonBanChay(1);
            break;
        case 7:
            xepHangMonBanChay(2);
            break;
        case 8:
            xepHangMonBanChay(3);
            break;
        case 0:
            cout << "Quay lai menu chinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);
}
int timBan(const vector<Ban> &dsBan, int soBan) {
    for (int i = 0; i < static_cast<int>(dsBan.size()); i++) {
        if (dsBan[i].soBan == soBan) {
            return i;
        }
    }
    return -1;
}
void themBan(vector<Ban> &dsBan) {
    Ban b;
    cout<<"Nhap so ban: ";
    cin>>b.soBan;
    cin.ignore(1000, '\n');
    if (timBan(dsBan, b.soBan) != -1) {
        cout << "So ban da ton tai!\n";
        return;
    }
    cout << "Nhap vi tri: ";
    getline(cin, b.viTri);
    cout<<"Nhap so ghe: ";
    cin>>b.soGhe;
    b.trangThai = "Trong";
    try {
    if (b.soBan <= 0) {
        throw invalid_argument("So ban phai lon hon 0");
    }
    if (b.viTri.empty()) {
        throw invalid_argument("Vi tri khong duoc bo trong");
    }
    if (b.soGhe <= 0) {
        throw invalid_argument("So ghe phai lon hon 0");
    }
    }
    catch (const exception &e) {
        cout << "Loi them ban: " << e.what() << endl;
    return;
    }
    dsBan.push_back(b);
    ghiBanVaoFile(dsBan);
    cout << "Da them ban va luu vao file!\n";
}
void suaBan(vector<Ban> &dsBan) {
    int soBan;
    cout<<"Nhap so ban can sua: ";
    cin>>soBan;
    cin.ignore(1000, '\n');
    int vt = timBan(dsBan, soBan);
    if (vt == -1) {
        cout << "Khong tim thay ban!\n";
        return;
    }
    cout << "Nhap vi tri moi: ";
    getline(cin, dsBan[vt].viTri);
    cout<<"Nhap so ghe moi: ";
    cin>>dsBan[vt].soGhe;
    ghiBanVaoFile(dsBan);
    cout << "Da sua thong tin ban va luu vao file!\n";
}
void xoaBan(vector<Ban> &dsBan) {
    int soBan;
    cout<<"Nhap so ban can xoa: ";
    cin>>soBan;
    cin.ignore(1000,'\n');
    int vt = timBan(dsBan, soBan);
    if (vt == -1) {
        cout << "Khong tim thay ban!\n";
        return;
    }
    if (dsBan[vt].trangThai != "Trong") {
        cout << "Khong the xoa vi ban dang co khach!\n";
        return;
    }
    dsBan.erase(dsBan.begin() + vt);
    ghiBanVaoFile(dsBan);
    cout << "Da xoa ban va luu vao file!\n";
}
void timKiemBan(const vector<Ban> &dsBan) {
    int soBan;
    cout<<"Nhap so ban can tim: ";
    cin>>soBan;
    cin.ignore(1000,'\n');
    int vt = timBan(dsBan, soBan);
    if (vt == -1) {
        cout << "Khong tim thay ban!\n";
        return;
    }
    cout << "\n========== THONG TIN BAN ==========\n";
    cout << "So ban: " << dsBan[vt].soBan << endl;
    cout << "Vi tri: " << dsBan[vt].viTri << endl;
    cout << "So ghe: " << dsBan[vt].soGhe << endl;
    cout << "Trang thai: " << dsBan[vt].trangThai << endl;
}
void trangThai1Ban(vector<Ban> &dsBan) {
    int soBan;
    cout << "Nhap so ban can chuyen ve Trong: ";
    cin >> soBan;
    cin.ignore(1000, '\n');
    int vt = timBan(dsBan, soBan);
    if (vt == -1) {
        cout << "Khong tim thay ban!\n";
        return;
    }
    if (dsBan[vt].trangThai == "Trong") {
        cout << "Ban nay dang trong san roi!\n";
        return;
    }
    dsBan[vt].trangThai = "Trong";
    ghiBanVaoFile(dsBan);
    cout << "Da chuyen ban " << soBan << " ve trang thai Trong!\n";
}
void trangThaiAllBan(vector<Ban> &dsBan) {
    for (Ban &b : dsBan) {
        b.trangThai = "Trong";
    }
    ghiBanVaoFile(dsBan);
    cout << "Da chuyen tat ca ban ve trang thai Trong!\n";
}
void capNhatTrangThaiBan(vector<Ban> &dsBan) {
    int luaChon;
    do {
        cout << "\n========== CAP NHAT TRANG THAI BAN ==========\n";
        cout << "1. Chuyen 1 ban ve trang thai Trong\n";
        cout << "2. Chuyen tat ca ban ve trang thai Trong\n";
        cout << "0. Quay lai\n";
        cout << "Nhap lua chon: ";
        cin >> luaChon;
        cin.ignore(1000, '\n');
        switch (luaChon) {
        case 1:
            trangThai1Ban(dsBan);
            break;
        case 2:
            trangThaiAllBan(dsBan);
            break;
        case 0:
            cout << "Quay lai quan ly ban.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);
}
void quanLyBan(vector<Ban> &dsBan) {
    int luaChon;
    do {
        docBanTuFile(dsBan);
        cout << "\n========== QUAN LY BAN ==========\n";
        cout << "1. Xem danh sach ban\n";
        cout << "2. Them ban\n";
        cout << "3. Cap nhat thong tin ban\n";
        cout << "4. Xoa ban\n";
        cout << "5. Tim kiem ban\n";
        cout<<  "6. Sua thong tin ban\n";
        cout << "0. Quay lai\n";
        cout<<"Nhap lua chon: ";
        cin>>luaChon;
        switch (luaChon) {
        case 1:
            hienThiBan(dsBan);
            break;
        case 2:
            themBan(dsBan);
            break;
        case 3:
            capNhatTrangThaiBan(dsBan);
            break;
        case 4:
            xoaBan(dsBan);
            break;
        case 5:
            timKiemBan(dsBan);
            break;
        case 6:
            suaBan(dsBan);
            break;
        case 0:
            cout << "Quay lai menu chinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);
}
int layMaHoaDonTiepTheo() {
    ifstream file("DataCafe.txt");
    if (!file.is_open()) {
        return 1;
    }
    string dong;
    int maLonNhat = 0;
    while (getline(file, dong)) {
        if (dong.empty()) {
            continue;
        }
        stringstream ss(dong);
        string loaiDong;
        getline(ss, loaiDong, '|');
        if (loaiDong == "HD") {
            string maHDText;
            getline(ss, maHDText, '|');
            int maHD = stoi(maHDText);
            if (maHD > maLonNhat) {
                maLonNhat = maHD;
            }
        }
    }
    file.close();
    return maLonNhat + 1;
}
//  CHON BAN 
int chonBan(vector<Ban> &dsBan) {
    int soBan;
    hienThiBan(dsBan);
    cout << "\nNhap so ban muon chon: ";
    cin >> soBan;
    cin.ignore(1000, '\n');
    for (Ban &b : dsBan) {
        if (b.soBan == soBan) {
            if (b.trangThai == "Trong") {
                b.trangThai = "Co khach";
                cout << "Da chon ban " << soBan << endl;
                return soBan;
            }
            else {
                cout << "Ban nay dang co khach!\n";
                return -1;
            }
        }
    }
    cout << "Khong tim thay ban!\n";
    return -1;
}
void giaiPhongBan(vector<Ban> &dsBan, int soBan) {
    for (Ban &b : dsBan) {
        if (b.soBan == soBan) {
            b.trangThai = "Trong";
            return;
        }
    }
}
void hienThiMenuChinh() {
    cout << "\n========== QUAN LI QUAN CA PHE ==========\n";
    cout << "1. Tao hoa don moi\n";
    cout << "2. Xem danh sach ban\n";
    cout << "3. Xem menu do uong\n";
    cout << "4. Bao cao doanh thu\n";
    cout << "5. Quan ly menu do uong\n";
    cout << "6. Quan ly ban\n";
    cout << "0. Thoat\n";
}
void taoHoaDonMoi(vector<DoUong> &menu, vector<Ban> &dsBan, int &maHoaDonTuDong) {
    docMenuTuFile(menu);
    docBanTuFile(dsBan);
    HoaDon hd;
    hd.maHD = maHoaDonTuDong++;
    hd.soBan = 0;
    hd.thoiGian = layThoiGian();
    hd.ca = layCaHienTai();
    int hinhThuc;
    cout << "\n1. Mang di\n";
    cout << "2. Phuc vu tai quan\n";
    cout<<"Nhap hinh thuc: ";
    cin>>hinhThuc;
    if (hinhThuc == 1) {
        hd.soBan = 0;
        cout << "Ban da chon mang di.\n";
    }
    else if (hinhThuc == 2) {
        int soBan = chonBan(dsBan);
        if (soBan == -1) {
            cout << "Khong the tao hoa don vi chua chon duoc ban.\n";
            return;
        }
        hd.soBan = soBan;
        ghiBanVaoFile(dsBan);
    }
    else {
        cout << "Lua chon khong hop le!\n";
        return;
    }
    char xacNhan;
    do {
        hd.dsMon.clear();
        goiMon(menu, hd.dsMon);
        if (hd.dsMon.empty()) {
            cout << "Chua goi mon nao, huy hoa don!\n";
            if (hd.soBan != 0) {
                giaiPhongBan(dsBan, hd.soBan);
                ghiBanVaoFile(dsBan);
            }
            return;
        }
        cout << "\nDanh sach mon da goi:\n";
        for (size_t i = 0; i < hd.dsMon.size(); i++) {
            cout << i + 1 << ". "
                 << hd.dsMon[i].mon.tenMon
                 << " - SL: " << hd.dsMon[i].soLuong
                 << " - Don gia: " << fixed << setprecision(0) << hd.dsMon[i].mon.gia
                 << endl;
        }
        cout << "\nBan co chac chan chon cac mon nay khong? (y/n): ";
        cin >> xacNhan;
        cin.ignore(1000, '\n');
        if (xacNhan == 'n' || xacNhan == 'N') {
            cout << "Moi ban chon lai mon.\n";
        }
    } while (xacNhan == 'n' || xacNhan == 'N');
    char chonVIP;
    bool vip = false;
    cout << "\nKhach co the VIP khong? (y/n): ";
    cin >> chonVIP;
    cin.ignore(1000, '\n');
    if (chonVIP == 'y' || chonVIP == 'Y') {
        vip = true;
    }
    tinhTien(hd, vip);
    inHoaDon(hd, vip);
    luuDoanhThu(hd);
}
void chayChuongTrinh() {
    vector<DoUong> menu;
    vector<Ban> dsBan;
    if (!docMenuTuFile(menu)) {
        cout << "Khong co du lieu menu. Chuong trinh dung.\n";
        return;
    }
    if (!docBanTuFile(dsBan)) {
        cout << "Khong co du lieu ban. Chuong trinh dung.\n";
        return;
    }
    int luaChon;
    int maHoaDonTuDong = 1;
    do {
        hienThiMenuChinh();
        cout<<"Nhap lua chon: ";
        cin>>luaChon;
        switch (luaChon) {
        case 1:
            taoHoaDonMoi(menu, dsBan, maHoaDonTuDong);
            break;
        case 2:
            if (docBanTuFile(dsBan)) {
                hienThiBan(dsBan);
            }
            break;
        case 3:
            if (docMenuTuFile(menu)) {
                hienThiMenu(menu);
            }
            break;
        case 4:
            baoCaoDoanhThu();
            break;
        case 5:
            quanLyMenuDoUong(menu);
            break;
        case 6:
            quanLyBan(dsBan);
            break;
        case 0:
            cout << "Thoat chuong trinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);
}
//  MAIN 
int main() {
   chayChuongTrinh();
    return 0;
}