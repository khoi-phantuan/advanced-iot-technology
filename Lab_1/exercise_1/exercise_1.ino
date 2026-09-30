// khai báo biến ứng với nhiệm vụ điều khiển đèn LED của các chân 
int LED_3 = 13; // chân digital 13 điều khiển LED số 3 (ngoài cùng bên trái)
int LED_2 = 12; // chân digital 12 điều khiển LED số 2 (ở giữa bên trái)
int LED_1 = 11; // chân digital 11 điều khiển LED số 1 (ở giữa bên phải)
int LED_0 = 10; // chân digital 10 điều khiển LED số 0 (ngoài cùng bên phải)

// khai báo biến ứng với nhiệm vụ điều khiển nút bấm của các chân
int counter_button = 7; // chân digital 7 điều khiển nút bấm tăng/giảm giá trị đếm (nút bên trái)
int direction_button = 6; // chân digital 6 điều khiển nút bấm thay đổi chiều đếm

int value = 0; // giá trị đang đếm tới
bool direction = true; // chiều đếm hiện tại của chương trình (true là đếm LÊN, false là đếm XUỐNG)

bool last_counter = 0; // biến lưu trạng thái của việc "nút counter có bị bấm hay không?" ở lần loop trước (0 là "không", 1 là "có")
bool last_direction = 0; // biến lưu trạng thái của việc "nút direction bị bấm hay không?" ở lần loop trước (0 là "không", 1 là "có")
// bởi vì trong khoảnh khắc (dù chỉ 0.1 giây) khi ta nhấn nút, hàng ngàn vòng lặp rất nhanh của chương trình đã chạy
// -> Chỉ bấm nút trong một khoảnh khắc rất ngắn, nhưng vì các vòng lặp chạy liên tục quá nhanh, chương trình sẽ hiểu là "nút được bấm liên tục" và tăng/giảm value không phanh !

void setup() {
  // các chân này sẽ xuất điện áp mức cao/mức thấp để điều khiển trạng thái sáng/tắt của đèn LED tương ứng
  pinMode(LED_3, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_0, OUTPUT);

  // các chân này sẽ đọc trạng thái điện áp (cao/thấp) tại các nút bấm để biết nó có vừa được người dùng bấm không (có: mức cao ; không: mức thấp)
  pinMode(counter_button, INPUT);
  pinMode(direction_button, INPUT);
}

void loop() {
  bool new_counter = digitalRead(counter_button); // biến lưu trạng thái của việc "nút counter có bị bấm hay không?" ở lần loop này
  bool new_direction = digitalRead(direction_button); // biến lưu trạng thái của việc "nút direction có bị bấm hay không?" ở lần loop này

  if (new_direction && !last_direction) // nếu trạng thái của nút direction ở lần loop trước là "KHÔNG NHẤN" nhưng ở lần lặp này là "ĐƯỢC NHẤN"...
  // (đồng nghĩa với việc đây là vòng lặp đầu tiên chạy kể từ khi chương trình đọc được trạng thái của nút direction là "ĐƯỢC NHẤN"...)
  {
    direction = !direction; // lập tức đảo chiều đếm
  }

  if (new_counter && !last_counter) // nếu trạng thái của nút counter ở lần loop trước là "KHÔNG NHẤN" nhưng ở lần lặp này là "ĐƯỢC NHẤN"...
  // (đồng nghĩa với việc đây là vòng lặp đầu tiên chạy kể từ khi chương trình đọc được trạng thái của nút counter là "ĐƯỢC NHẤN"...)
  {
    // đếm lên/đếm xuống theo chiều đếm hiện tại
    if (direction)
      value++;
    else
      value--;

    Serial.print("Current value: ");
    Serial.print(value, BIN);
    Serial.print("(");
    Serial.print(value, DEC);
    Serial.print(") - Current counting direction: ");
    if (direction)
      Serial.println("UP");
    else
      Serial.println("DOWN");
  }

  if (value == 15 || value == 0)
    direction = !direction;

  last_counter = new_counter; // cập nhật trạng thái mới nhất của việc "nút counter có bị bấm hay không?" theo trạng thái đã ghi nhận trong lần lặp này
  last_direction = new_direction; // cập nhật trạng thái mới nhất của việc "nút counter có bị bấm hay không?" theo trạng thái đã ghi nhận trong lần lặp này
}
