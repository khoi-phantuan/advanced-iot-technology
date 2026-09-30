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

#define MSB 3 // index của bit có trọng số lớn nhất trong chuỗi bit cần thao tác

void setup() {
  // các chân này sẽ xuất điện áp mức cao/mức thấp để điều khiển trạng thái sáng/tắt của đèn LED tương ứng
  pinMode(LED_3, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_1, OUTPUT);
  pinMode(LED_0, OUTPUT);

  // các chân này sẽ đọc trạng thái điện áp (cao/thấp) tại các nút bấm để biết nó có vừa được người dùng bấm không (có: mức cao ; không: mức thấp)
  pinMode(counter_button, INPUT);
  pinMode(direction_button, INPUT);

  Serial.begin(115200); // bắt đầu giao tiếp serial ở tốc độ 115200 bit/s (để in log ra màn hình)

  print_log();
}

// sử dụng phép toán dịch bit để in từng bit của dãy bit ra màn hình (theo thứ tự từ MSB đến LSB)
void print_binary_value()
{
  int i = MSB;
  while (i >= 0)
  {
    Serial.print((value >> i) & 1);

    i--;
  }
}

// bật/tắt các đèn LED tương ứng với trạng thái của bit điều khiển nó trong dãy bit value
void led_on()
{
  digitalWrite(LED_3, (value >> 3) & 1);
  digitalWrite(LED_2, (value >> 2) & 1);
  digitalWrite(LED_1, (value >> 1) & 1);
  digitalWrite(LED_0, (value >> 0) & 1);
}

// in log ra màn hình Serial Monitor theo yêu cầu
void print_log()
{
    Serial.print("Current value: ");
    print_binary_value(); // in giá trị nhị phân của value
    
    Serial.print(" - Decimal: ");
    Serial.print(value); // in giá trị thập phân của value

    // in chiều đếm hiện tại của chương trình
    if (direction)
      Serial.println(" - Direction: UP");
    else
      Serial.println(" - Direction: DOWN");
}

int start_checking_bounce_direction = 0;
int start_checking_bounce_counter = 0;

void loop() {
  bool new_counter = digitalRead(counter_button); // biến lưu trạng thái của việc "nút counter có bị bấm hay không?" ở lần loop này
  bool new_direction = digitalRead(direction_button); // biến lưu trạng thái của việc "nút direction có bị bấm hay không?" ở lần loop này

  if (new_direction && !last_direction) // ngay khi trạng thái của nút direction chuyển từ "KHÔNG NHẤN" sang "ĐƯỢC NHẤN"... (chưa cần biết do nhiễu hay do người bấm thật)
  {
    start_checking_bounce_direction = millis(); // ghi lại thời điểm đó ngay
  }

  /*
  Ba điều kiện của khối if bên dưới tương ứng với các ràng buộc sau:
  1. Trước khi xử lý bất cứ thứ gì, phải chờ X ms (ở đây dùng 50) để tín hiệu đọc được tại nút direction được ổn định
  2. Sau khoảng thời gian chờ trên, trạng thái của nút direction vẫn phải còn đang là "ĐƯỢC NHẤN"
  3. Đây là loop đầu tiên mà 2 điều kiện bên trên được thỏa 
  
  Giải thích thêm cho ý 3:
  - 'start_checking_bounce_direction' đóng vai trò như một flag. Giá trị của nó (bằng hoặc khác 0) sẽ quyết định chương trình có cần xử lý tín hiệu
  "ĐƯỢC NHẤN" của nút direction trong loop hiện tại hay không.
  - Vì sao lại như vậy? Vì mặc định là biến này sẽ bằng 0. Nó sẽ được set thành một giá trị khác 0 KHI VÀ CHỈ KHI chương trình đọc được sự thay đổi 
  trạng thái của nút direction từ "KHÔNG NHẤN" sang "ĐƯỢC NHẤN".
  - Khi mà cả 2 điều kiện đầu của khối if bên dưới được thỏa, cái ta cần kiểm tra là: tín hiệu "ĐƯỢC NHẤN" lần này đã được xử lý hay chưa ? Nếu rồi, 
  giá trị của flag sẽ bằng 0. Nếu chưa, giá trị của flag sẽ khác 0.
  - Vì sao lại như vậy? Vì biến này sẽ được set về lại 0 KHI VÀ CHỈ KHI chương trình đi vào được khối if bên dưới và thực thi được các lệnh bên trong 
  đó. Và ngay trong cái loop đầu tiên chạy kể từ khi 2 điều kiện đầu được thỏa, ta đã set ngay flag này về lại 0 rồi. Từ đó, những loop chạy sau đó 
  dù vẫn đọc được tín hiệu nút direction là "ĐƯỢC NHẤN" cũng không thể thực thi được khối if này.
  - Vì như đã nói ở trên, trong khoảnh khắc ta nhấn nút thì chương trình đã chạy qua hàng ngàn vòng lặp. Rất có thể, những loop chạy sau đó 
  vẫn đọc được tín hiệu là "ĐƯỢC NHẤN" chỉ vì ta chưa thả tay (đó không phải là một lần bấm nút mới thực sự) -> Không xử lý.

  Giải thuật này đòi hỏi người bấm cần giữ tay lâu một chút khi bấm nút (đừng nhấn-nhả quá nhanh trong dưới 30ms)
  */

  if ((millis() - start_checking_bounce_direction >= 30) && new_direction && start_checking_bounce_direction != 0)
  {
    start_checking_bounce_direction = 0;
    
    direction = !direction; // lập tức đảo chiều đếm

    Serial.print("Counting direction changed to: ");
    Serial.println(direction ? "UP" : "DOWN"); // in log chiều đếm mới sau khi được đảo
  }

  // áp dụng tư duy xử lý tương tự lên nút counter
  if (new_counter && !last_counter)
  {
    start_checking_bounce_counter = millis();
  }

  if ((millis() - start_checking_bounce_counter >= 30) && new_counter && start_checking_bounce_counter != 0)
  {
    start_checking_bounce_counter = 0;
    
    // đếm lên/đếm xuống theo chiều đếm hiện tại
    if (direction)
      value++;
    else
    {
      if (value != 0) // chỉ tiếp tục đếm xuống khi value đang khác 0 (tránh đếm xuống số âm làm hỏng logic bật đèn)
        value--;
    }

    if (value > 15) // khi value hơn max, đưa về 0 theo yêu cầu
      value = 0;

    led_on();

    print_log();
  }

  last_counter = new_counter; // cập nhật trạng thái mới nhất của việc "nút counter có bị bấm hay không?" theo trạng thái đã ghi nhận trong lần lặp này
  last_direction = new_direction; // cập nhật trạng thái mới nhất của việc "nút counter có bị bấm hay không?" theo trạng thái đã ghi nhận trong lần lặp này
}
