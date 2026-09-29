// khai báo biến ứng với nhiệm vụ điều khiển LED của các chân
int LED_3 = 13; // chân digital 13 điều khiển LED số 3 (ngoài cùng bên trái)
int LED_2 = 12; // chân digital 12 điều khiển LED số 2 (ở giữa bên trái)
int LED_1 = 11; // chân digital 11 điều khiển LED số 1 (ở giữa bên phải)
int LED_0 = 10; // chân digital 10 điều khiển LED số 0 (ngoài cùng bên phải)

// khai báo biến ứng với nhiệm vụ điều khiển nút bấm của các chân
int counter_button = 7; // chân digital 7 điều khiển nút bấm tăng/giảm giá trị đếm (nút bên trái)
int direction_button = 6; // chân digital 6 điều khiển nút bấm thay đổi chiều đếm

int value = 0; // giá trị đang đếm tới
bool last_counter = 0; // biến lưu trạng thái có/không của việc nút counter bị bấm ở lần loop trước
bool last_direction = 0; // biến lưu trạng thái có/không của việc nút direction bị bấm ở lần loop trước
// bởi vì trong khoảnh khắc chỉ 0.1 giây khi ta nhấn nút, hàng ngàn lần lặp của chương trình đã diễn ra !s

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
