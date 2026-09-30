// khai báo biến ứng với các chân điều khiển từng đoạn LED (A-G)
short LED_A = 13; // chân D13 điều khiển đoạn LED A
short LED_B = 12; // chân D12 điều khiển đoạn LED B
short LED_C = 11; // chân D11 điều khiển đoạn LED C
short LED_D = 10; // chân D10 điều khiển đoạn LED D
short LED_E = 9;  // chân D9 điều khiển đoạn LED E
short LED_F = 8;  // chân D8 điều khiển đoạn LED F
short LED_G = 7;  // chân D7 điều khiển đoạn LED G

short BUTTON = 2; // khai báo biến ứng với chân D2 nhận thông tin từ nút bấm
bool last_button = false; // biến lưu trạng thái của việc "nút có được chương trình nhận diện là "ĐƯỢC NHẤN" hay không" trong vòng lặp trước (true -> có ; false -> không)
unsigned long start_timing_button_bounce = 0;

// struct lưu thông tin sinh viên, gồm tên và mã số
struct SinhVien
{
  char* HoTen;
  char* MSSV;
};

// mảng sinh viên, gồm 3 sinh viên của nhóm cần hiển thị mã số lên LED 7 đoạn
SinhVien sv_arr[3] = {
  {"Phan Tuan Khoi", "24520867"},
  {"Nguyen Huynh Dang Khoa", "24520829"},
  {"Huynh Mai Phong", "25521379"}
};

short index = -1; // chỉ số của sinh viên đang được hiển thị mã số, giá trị từ 0 đến 2 (ban đầu chưa ai nhấn nút - chưa có SV nào để hiển thị mã số thì bằng -1)
bool next_num = false; // biến cho biết: đã đến lúc chuyển sang hiển thị số tiếp theo trong dãy mã số hiện tại chưa ?

char *ID = NULL; // con trỏ trỏ đến các ký tự trong chuỗi ký tự mã số sinh viên (gồm 8 chữ số và 1 ký tự kết thúc - '\0')

// định nghĩa các khoảng thời gian sẽ dùng để so sánh trong chương trình
#define DISPLAY_NUMBER_TIME 600 // thời gian hiển thị một chữ số
#define DISPLAY_HYPHEN_TIME 1000 // thời gian hiển thị dấu gạch ngang
#define BREAK_TIME 150 // thời gian giãn cách giữa 2 lần hiển thị liên tiếp (cả chữ số - chữ số và chữ số - dấu gạch ngang)

void setup() {
  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);
  pinMode(LED_C, OUTPUT);
  pinMode(LED_D, OUTPUT);
  pinMode(LED_E, OUTPUT);
  pinMode(LED_F, OUTPUT);
  pinMode(LED_G, OUTPUT);

  pinMode(BUTTON, INPUT);

  Serial.begin(115200);
}

// bật/tắt các đoạn LED tương ứng để hiển thị các chữ số và dấu gạch ngang
void display_on_led(bool state)
{
  switch(*ID)
  {
    case '0':
    {
      digitalWrite(LED_A, state);
      digitalWrite(LED_B, state);
      digitalWrite(LED_C, state);
      digitalWrite(LED_D, state);
      digitalWrite(LED_E, state);
      digitalWrite(LED_F, state);

      break;
    }

    case '1':
    {
      digitalWrite(LED_B, state);
      digitalWrite(LED_C, state);

      break;
    }

    case '2':
    {
      digitalWrite(LED_A, state);
      digitalWrite(LED_B, state);
      digitalWrite(LED_D, state);
      digitalWrite(LED_E, state);
      digitalWrite(LED_G, state);

      break;
    }

    case '3':
    {
      digitalWrite(LED_A, state);
      digitalWrite(LED_B, state);
      digitalWrite(LED_C, state);
      digitalWrite(LED_D, state);
      digitalWrite(LED_G, state);

      break;
    }

    case '4':
    {
      digitalWrite(LED_B, state);
      digitalWrite(LED_C, state);
      digitalWrite(LED_F, state);
      digitalWrite(LED_G, state);

      break;
    }

    case '5':
    {
      digitalWrite(LED_A, state);
      digitalWrite(LED_C, state);
      digitalWrite(LED_D, state);
      digitalWrite(LED_F, state);
      digitalWrite(LED_G, state);

      break;
    }

    case '6':
    {
      digitalWrite(LED_A, state);
      digitalWrite(LED_C, state);
      digitalWrite(LED_D, state);
      digitalWrite(LED_E, state);
      digitalWrite(LED_F, state);
      digitalWrite(LED_G, state);

      break;
    }

    case '7':
    {
      digitalWrite(LED_A, state);
      digitalWrite(LED_B, state);
      digitalWrite(LED_C, state);

      break;
    }

    case '8':
    {
      digitalWrite(LED_A, state);
      digitalWrite(LED_B, state);
      digitalWrite(LED_C, state);
      digitalWrite(LED_D, state);
      digitalWrite(LED_E, state);
      digitalWrite(LED_F, state);
      digitalWrite(LED_G, state);

      break;
    }

    case '9':
    {
      digitalWrite(LED_A, state);
      digitalWrite(LED_B, state);
      digitalWrite(LED_C, state);
      digitalWrite(LED_D, state);
      digitalWrite(LED_F, state);
      digitalWrite(LED_G, state);

      break;
    }

    default:
    {
      digitalWrite(LED_G, state);

      break;
    }
  }
}

unsigned long start_timing_display_number = 0; // lưu thời gian chương trình đã chạy ngay khi bắt đầu hiển thị một chữ số trên LED
unsigned long start_timing_display_hyphen = 0; // lưu thời gian chương trình đã chạy ngay khi bắt đầu hiển thị dấu gạch ngang trên LED
unsigned long start_timing_break = 0; // lưu thời gian chương trình đã chạy ngay khi bắt đầu khoảng thời gian nghỉ

// hàm xử lý sự kiện nhấn nút
void process_pressing_button()
{
  index = (index + 1) % 3; // chuyển sang sinh viên kế tiếp
  ID = sv_arr[index].MSSV; // ID bây giờ trỏ tới ký tự đầu tiên (chữ số đầu tiên) trong chuỗi số thuộc MSSV của sinh viên đang xét

  display_on_led(1);
  start_timing_display_number = millis();
}

void loop() {
  bool current_button = digitalRead(BUTTON); // biến lưu trạng thái của việc "nút có được chương trình nhận diện là "ĐƯỢC NHẤN" hay không" trong vòng lặp này

  if (current_button && !last_button) // ngay ở vòng lặp đầu tiên chương trình nhận diện được: nút từ trạng thái "KHÔNG NHẤN" chuyển sang "ĐƯỢC NHẤN"...
  {
    start_timing_button_bounce = millis(); // ghi lại mốc thời gian đó ngay
  }

  // nếu đã trôi qua 30ms kể từ mốc thời gian đó mà nút vẫn còn được nhấn...
  if ((millis() - start_timing_button_bounce >= 30) && current_button && (start_timing_button_bounce != 0))
  {
    start_timing_button_bounce = 0;
    
    // tiến hành xử lý lần nhấn nút này
    process_pressing_button();
  }

  if ((start_timing_break != 0) && (millis() - start_timing_break >= BREAK_TIME))
  {
    start_timing_break = 0;
    display_on_led(1);

    if (*ID != '\0')
      start_timing_display_number = millis();
    else
      start_timing_display_hyphen = millis();
  }

  if ((start_timing_display_number != 0) && (millis() - start_timing_display_number >= DISPLAY_NUMBER_TIME))
  {
    start_timing_display_number = 0;
    display_on_led(0);

    start_timing_break = millis();
    ID++;
  }

  if ((start_timing_display_hyphen != 0) && (millis() - start_timing_display_hyphen >= DISPLAY_HYPHEN_TIME))
  {
    start_timing_display_hyphen = 0;
    display_on_led(0);

    start_timing_break = millis();
    ID = sv_arr[index].MSSV;
  }

  last_button = current_button; // cập nhật trạng thái nhấn nút hiện tại thành trạng thái nhấn nút mới nhất
}
