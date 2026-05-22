#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <limits>
using namespace std;
struct DoUong {
    string maMon;
    string tenMon;
    string loai;
    string size;
    double gia;
    bool conHang;
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
int nhapSoNguyen(const string &thongBao) {
    int so;
    while (true) {
        cout << thongBao;
        cin >> so;
        if (cin.fail()) {
            cout << "Loi! Vui long nhap so nguyen!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return so;
        }
    }
}
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
void khoiTaoMenu(vector<DoUong> &menu) {
    menu.push_back({"CF01", "Ca phe den", "Nong", "M", 25000, true});
    menu.push_back({"CF02", "Ca phe sua", "Da", "M", 30000, true});
    menu.push_back({"TS01", "Tra sua tran chau", "Lanh", "L", 40000, true});
    menu.push_back({"ST01", "Sinh to xoai", "Da xay", "M", 35000, true});
    menu.push_back({"NC01", "Nuoc cam", "Lanh", "M", 30000, true});
}
void khoiTaoBan(vector<Ban> &dsBan) {
    dsBan.push_back({1, "Trong nha", 4, "Trong"});
    dsBan.push_back({2, "Trong nha", 4, "Trong"});
    dsBan.push_back({3, "Ngoai troi", 2, "Trong"});
    dsBan.push_back({4, "Tang 2", 6, "Trong"});
    dsBan.push_back({5, "Tang 2", 4, "Trong"});
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
             << setw(10) << fixed << setprecision(0) << x.gia
             << (x.conHang ? "Con hang" : "Het hang") << endl;
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
        else if (!menu[vt].conHang) {
            cout << "Mon nay da het hang!\n";
        }
        else {
            soLuong = nhapSoNguyen("Nhap so luong: ");
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    while (getline(file, dong)) {
        if (dong.empty()) {
            continue;
        }
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
        bg.maHD = stoi(maHDText);
        bg.ngay = stoi(ngayText);
        bg.thang = stoi(thangText);
        bg.nam = stoi(namText);
        bg.thanhTien = stod(tienText);
        ds.push_back(bg);
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
    while (getline(file, dong)) {
        if (dong.empty()) {
            continue;
        }
        stringstream ss(dong);
        string loaiDong;
        getline(ss, loaiDong, '|');
        if (loaiDong != "CT") {
            continue;
        }
        string maHDText, ngayText, thangText, namText, soLuongText, tienText;
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
        bg.maHD = stoi(maHDText);
        bg.ngay = stoi(ngayText);
        bg.thang = stoi(thangText);
        bg.nam = stoi(namText);
        bg.soLuong = stoi(soLuongText);
        bg.doanhThuMon = stod(tienText);
        ds.push_back(bg);
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
    int ngay = nhapSoNguyen("Nhap ngay: ");
    int thang = nhapSoNguyen("Nhap thang: ");
    int nam = nhapSoNguyen("Nhap nam: ");
    double tong = tinhDoanhThuTheoNgay(ngay, thang, nam);
    cout << "\nDoanh thu ngay " << ngay << "/" << thang << "/" << nam
         << ": " << fixed << setprecision(0) << tong << endl;
}
void baoCaoTheoThang() {
    int thang = nhapSoNguyen("Nhap thang: ");
    int nam = nhapSoNguyen("Nhap nam: ");
    double tong = tinhDoanhThuTheoThang(thang, nam);
    cout << "\nDoanh thu thang " << thang << "/" << nam
         << ": " << fixed << setprecision(0) << tong << endl;
}
void baoCaoTheoNam() {
    int nam = nhapSoNguyen("Nhap nam: ");
    double tong = tinhDoanhThuTheoNam(nam);
    cout << "\nDoanh thu nam " << nam
         << ": " << fixed << setprecision(0) << tong << endl;
}
void thongKeDoanhThuTheoCa() {
    vector<BanGhiHoaDon> ds = docHoaDonTuFile();
    int ngay = nhapSoNguyen("Nhap ngay: ");
    int thang = nhapSoNguyen("Nhap thang: ");
    int nam = nhapSoNguyen("Nhap nam: ");
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
        luaChon = nhapSoNguyen("Nhap lua chon: ");
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
//  CHON BAN 
int chonBan(vector<Ban> &dsBan) {
    int soBan;
    hienThiBan(dsBan);
    cout << "\nNhap so ban muon chon: ";
    cin >> soBan;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
//  MAIN 
int main() {
    vector<DoUong> menu;
    vector<Ban> dsBan;
    khoiTaoMenu(menu);
    khoiTaoBan(dsBan);
    int luaChon;
    int maHoaDonTuDong = 1;
    do {
        cout << "\n========== QUAN LI QUAN CA PHE ==========\n";
        cout << "1. Tao hoa don moi\n";
        cout << "2. Xem danh sach ban\n";
        cout << "3. Xem menu do uong\n";
        cout << "4. Bao cao doanh thu\n";
        cout << "0. Thoat\n";
        luaChon = nhapSoNguyen("Nhap lua chon: ");
        switch (luaChon) {
        case 1: {
            HoaDon hd;
            hd.maHD = maHoaDonTuDong++;
            hd.soBan = 0;
            hd.thoiGian = layThoiGian();
            hd.ca = layCaHienTai();
            int hinhThuc;
            cout << "\n1. Mang di\n";
            cout << "2. Phuc vu tai quan\n";
            hinhThuc = nhapSoNguyen("Nhap hinh thuc: ");
            if (hinhThuc == 1) {
                hd.soBan = 0;
                cout << "Ban da chon mang di.\n";
            }
            else if (hinhThuc == 2) {
                int soBan = chonBan(dsBan);
                if (soBan == -1) {
                    cout << "Khong the tao hoa don vi chua chon duoc ban.\n";
                    break;
                }
                hd.soBan = soBan;
            }
            else {
                cout << "Lua chon khong hop le!\n";
                break;
            }
            goiMon(menu, hd.dsMon);
            if (hd.dsMon.empty()) {
                cout << "Chua goi mon nao, huy hoa don!\n";
                if (hd.soBan != 0) {
                    giaiPhongBan(dsBan, hd.soBan);
                }
                break;
            }
            char chonVIP;
            bool vip = false;
            cout << "\nKhach co the VIP khong? (y/n): ";
            cin >> chonVIP;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (chonVIP == 'y' || chonVIP == 'Y') {
                vip = true;
            }
            tinhTien(hd, vip);
            inHoaDon(hd, vip);
            luuDoanhThu(hd);
            if (hd.soBan != 0) {
                giaiPhongBan(dsBan, hd.soBan);
            }
            break;
        }
        case 2:
            hienThiBan(dsBan);
            break;
        case 3:
            hienThiMenu(menu);
            break;
        case 4:
            baoCaoDoanhThu();
            break;
        case 0:
            cout << "Thoat chuong trinh.\n";
            break;
        default:
            cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);
    return 0;
}
