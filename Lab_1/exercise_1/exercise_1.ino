// khai báo biến ứng với nhiệm vụ điều khiển LED của các chân
int LED_3 = 13; // chân digital 13 điều khiển LED số 3 (ngoài cùng bên trái)
int LED_2 = 12; // chân digital 12 điều khiển LED số 2 (ở giữa bên trái)
int LED_1 = 11; // chân digital 11 điều khiển LED số 1 (ở giữa bên phải)
int LED_0 = 10; // chân digital 10 điều khiển LED số 0 (ngoài cùng bên phải)

// khai báo biến ứng với nhiệm vụ điều khiển nút bấm của các chân
int counter_button = 7; // chân digital 7 điều khiển nút bấm tăng/giảm giá trị đếm (nút bên trái)
int direction_button = 6; // chân digital 6 điều khiển nút bấm thay đổi chiều đếm

int value = 0; // giá trị đang đếm tới
bool direction = true; // chiều đếm của mạch (true là đếm LÊN, false là đếm XUỐNG)

bool last_counter = 0; // biến lưu trạng thái có/không của việc nút counter bị bấm ở lần loop trước
bool last_direction = 0; // biến lưu trạng thái có/không của việc nút direction bị bấm ở lần loop trước
// bởi vì trong khoảnh khắc chỉ 0.1 giây khi ta nhấn nút, hàng ngàn lần lặp của chương trình đã diễn ra 
// -> Chỉ bấm nút 1 lần và thả tay nhưng vì các vòng lặp chạy liên tục quá nhanh, giá trị đếm sẽ tăng vọt (hoặc giảm liên tiếp) trong thời gian ngắn !

void setup() {
  // các chân này sẽ xuất điện áp mức cao/mức thấp để điều khiển trạng thái sáng/tắt của đèn LED tương ứng
  pinMode(LED_3, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_0, OUTPUT);

  // các chân này sẽ đọc trạng thái điện áp (cao/thấp) tại các nút bấm để biết nó có vừa được người dùng bấm không (nếu có bấm thì điện áp đọc được sẽ ở mức cao)
  pinMode(counter_button, INPUT);
  pinMode(direction_button, INPUT);
}

void loop() {
  bool new_counter = digitalRead(counter_button); // biến lưu trạng thái nhấn nút counter ở lần lặp này
  bool new_direction = digitalRead(direction_button); // biến lưu trạng thái nhấn nút direction ở lần lặp này

  if (direction_button && !last_direction) // nếu trạng thái của nút direction ở lần lặp trước là "KHÔNG NHẤN" nhưng ở lần lặp này là "ĐƯỢC NHẤN"...
  // (đồng nghĩa với việc đây là vòng lặp xuất hiện đầu tiên kể từ khi đọc được trạng thái của nút là "ĐƯỢC NHẤN")
  {
    direction != direction; // đảo chiều đếm
  }

  if (new_counter && !last_counter) // nếu trạng thái của nút counter ở lần lặp trước là "KHÔNG NHẤN" nhưng ở lần lặp này là "ĐƯỢC NHẤN"...
  // (đồng nghĩa với việc đây là vòng lặp xuất hiện đầu tiên kể từ khi đọc được trạng thái của nút là "ĐƯỢC NHẤN")
  {
    if (direction)
      value++;
    else
      value--;
  }

  if (value == 15 || value == 0)
    direction = !direction;

}
