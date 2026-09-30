// khai báo biến ứng với các chân điều khiển từng đoạn LED (A-G)
short LED_A = 13; // chân D13 điều khiển đoạn LED A
short LED_B = 12; // chân D12 điều khiển đoạn LED B
short LED_C = 11; // chân D11 điều khiển đoạn LED C
short LED_D = 10; // chân D10 điều khiển đoạn LED D
short LED_E = 9;  // chân D9 điều khiển đoạn LED E
short LED_F = 8;  // chân D8 điều khiển đoạn LED F
short LED_G = 7;  // chân D7 điều khiển đoạn LED G

short BUTTON = 2; // khai báo biến ứng với chân D2 nhận thông tin từ nút bấm

struct SinhVien
{
  char* HoTen;
  char* MSSV;
};

SinhVien sv_arr[3] = {
  {"Phan Tuan Khoi", "24520867"},
  {"Nguyen Huynh Dang Khoa", "24520829"},
  {"Huynh Mai Phong", "25521379"}
};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
