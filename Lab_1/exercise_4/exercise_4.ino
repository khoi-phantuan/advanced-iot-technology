// Lưu ý: LED 7 đoạn sử dụng là loại ANODE CHUNG (dây chung - chân Common - sẽ được nối với nguồn) !

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
  {"Sinh vien A", "12345678"},
  {"Sinh vien B", "87654321"},
  {"Sinh vien C", "13572468"}
};

short sv_index = -1; // chỉ số của sinh viên đang được hiển thị mã số, giá trị từ 0 đến 2 (ban đầu chưa ai nhấn nút - chưa có SV nào để hiển thị mã số thì bằng -1)

char *ID = NULL; // con trỏ trỏ đến các ký tự trong chuỗi ký tự mã số sinh viên (gồm 8 chữ số và 1 ký tự kết thúc - '\0')

// định nghĩa các khoảng thời gian sẽ dùng để so sánh trong chương trình
#define DISPLAY_NUMBER_TIME 600 // thời gian hiển thị một chữ số
#define DISPLAY_HYPHEN_TIME 1000 // thời gian hiển thị dấu gạch ngang
#define BREAK_TIME 150 // thời gian giãn cách giữa 2 lần hiển thị liên tiếp (cả chữ số - chữ số và chữ số - dấu gạch ngang)

void setup() {
  /*
  Nếu không ghi lệnh gì trước, thì ngay khoảnh khắc mode chuyển sang OUTPUT, các chân digital của Arduino xuất mức thấp (mặc định). 
  Với cách hoạt động của LED 7 đoạn Anode chung, điều này sẽ làm cho các đoạn LED sáng lên ngay khi chương trình khởi động (vì chân điều khiển chúng 
  đang xuất điện áp thấp). Vì vậy, ta cần phải tự viết logic cho các chân này xuất điện áp cao ngay khi chương trình khởi động, và ngay trước khi 
  các chân được khai báo là OUTPUT luôn, để đảm bảo các chân này sẽ không xuất điện áp thấp ngay từ đầu (đối với Arduino Uno này), từ đó 
  khiến các đoạn LED luôn tắt khi khởi động mạch. Còn về logic của việc xuất điện áp mức cao/mức thấp, xem kĩ hơn ở phần giải thích trong hàm display_on_led() bên dưới.
  */
  digitalWrite(LED_A, HIGH);
  digitalWrite(LED_B, HIGH);
  digitalWrite(LED_C, HIGH);
  digitalWrite(LED_D, HIGH);
  digitalWrite(LED_E, HIGH);
  digitalWrite(LED_F, HIGH);
  digitalWrite(LED_G, HIGH);
  
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
  state = !state; 
  /* Vì sao phải đảo state ?

  - Ví dụ, khi hàm chính truyền tham số '1' vào, tức lúc đó chương trình đang muốn bật đèn. 
  - Tuy nhiên, như đã nói, LED 7 đoạn sử dụng là loại Anode chung, tức là mỗi đoạn LED đều đã được nối với nguồn điện 5V thông qua dây Common rồi.
  - Vì vậy, nếu muốn LED sáng (có dòng điện chạy qua), ta phải cho chân điều khiển đoạn LED xuất ra một mức điện áp thấp (tín hiệu LOW - ở đây là 0V) 
  để dòng điện có thể đi từ nơi có điện áp cao đến nơi có điện áp thấp theo đúng nguyên lý hoạt động của dòng điện.
  
  Luồng chạy của dòng điện trong trường hợp này là: 
  Nguồn 5V của Arduino -> Chân Common của L7Đ -> Các chân đèn của L7Đ -> Điện trở -> Chân điều khiển tương ứng của Arduino)

  - Nếu ta cho chân điều khiển xuất điện áp cao (tín hiệu HIGH - ở đây là 5V), lúc này, theo đúng sơ đồ luồng trên, chênh lệch điện áp giữa nguồn 
  5V và chân điều khiển là không có -> Không có dòng điện nào chạy qua mạch cả -> Không có đoạn LED nào sáng được.
  - Vì vậy, khi các hàm bên ngoài gọi đến hàm này, ta vẫn giữ ngữ nghĩa "0 là tắt, 1 là sáng" cho đơn giản và dễ hiểu với người đọc. Tuy nhiên, bên
  trong hàm display_on_led() này, ta phải hiểu là khi chương trình muốn đèn sáng, các chân tương ứng phải xuất điện LOW, và ngược lại.
  */
  
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

void print_log()
{
  Serial.print("Currently displaying ID of: ");
  Serial.print(sv_arr[sv_index].HoTen);
  Serial.print(" - ");
  Serial.println(sv_arr[sv_index].MSSV);
}

// hàm xử lý sự kiện nhấn nút
void process_pressing_button()
{
  if (sv_index >= 0)
    display_on_led(0);

  start_timing_display_number = 0;
  start_timing_display_hyphen = 0;

  start_timing_break = millis();
  
  sv_index = (sv_index + 1) % 3; // chuyển sang sinh viên kế tiếp
  ID = sv_arr[sv_index].MSSV; // ID bây giờ trỏ tới ký tự đầu tiên (chữ số đầu tiên) trong chuỗi số thuộc MSSV của sinh viên đang xét

  print_log();
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
    ID = sv_arr[sv_index].MSSV;
  }

  last_button = current_button; // cập nhật trạng thái nhấn nút hiện tại thành trạng thái nhấn nút mới nhất
}
