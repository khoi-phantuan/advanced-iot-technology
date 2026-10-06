# Lab 1 — Giới thiệu về Arduino

> README hiện tại chỉ gồm nội dung thực hiện bài 1 và bài 4 (phần việc mình được giao trong phần làm nhóm). Các bài còn lại sẽ được bổ sung sau khi mình tự thực hành.

## Mục tiêu lab
Trong bài lab này, mình đã tìm hiểu cơ bản về nền tảng Arduino, bao gồm bo mạch, môi trường lập trình và cách lập trình nhúng đơn giản sử dụng hàm và thư viện có sẵn (trừu tượng). Sau đó ứng dụng bo mạch này vào để xây dựng các kịch bản theo yêu cầu.

## Công cụ
- **IDE:** Arduino IDE 2.3.10
  - **Package**: Arduino AVR Boards 1.8.8
- **Mô phỏng**: Tinkercad

---

## Bài 1: Bộ đếm nhị phân 4 bit

### Đề bài
Xây dựng một chương trình đếm số nhị phân 4 bit lên/xuống - thể hiện qua trạng thái sáng/tắt của các đèn LED. Dùng 2 nút bấm: 1 nút đếm sang giá trị tiếp theo - 1 nút đảo chiều đếm hiện tại.

### Linh kiện sử dụng
- 1 Arduino Uno R3
- 4 LED đỏ
- 2 nút bấm
- 4 điện trở 220 Ohm
- 2 điện trở 10k Ohm
- 1 breadboard
- 11 dây jumper

### Mạch
- Mạch điện mô phỏng:
![Mạch điện mô phỏng](exercise_1/simulated_circuit.png)

- Mạch điện thực tế:
![Mạch điện thực tế](exercise_1/practical_circuit.jpg)

### Code
[Source code](exercise_1/exercise_1.ino)

### Khó khăn và cách giải quyết
| Hiện tượng | Nguyên nhân | Cách phát hiện | Cách xử lý |
|----------|----------|----------|---|
| Cắm mạch vật lý thấy value và LED nhảy loạn xạ. Lúc không nhấn nút thì chuyển trạng thái, lúc nhấn nút thì không phản ứng gì, logic nút counter và direction lẫn lộn nhau. | Chưa nối phía còn lại của 2 nút bấm xuống GND (qua điện trở). Chân digital không có đường về GND nên lúc chưa nhấn nút bị "floating", đọc tín hiệu loạn. | Kiểm tra lại đường đi dây của mạch điện thực tế, đối chiếu kỹ càng với mạch điện mô phỏng. | Nối lại các đường dây còn thiếu. |

---

## Bài 4: Mã số sinh viên của nhóm là gì?

### Đề bài
Xây dựng một chương trình hiển thị các chữ số trong mã số sinh viên của các thành viên nhóm lên LED 7 đoạn. Dùng 1 nút bấm: nhấn nút thì chuyển sang hiển thị chuỗi số trong MSSV thành viên kế tiếp — **việc chuyển đổi này phải có hiệu lực ngay lập tức!**

### Linh kiện sử dụng
- 1 Arduino Uno R3  
- 1 LED 7 đoạn (loại Anode chung)
- 1 nút bấm
- 7 điện trở 220 Ohm
- 1 điện trở 10k Ohm
- 18 dây jumper
- 2 breadboard

### Mạch
- Mạch điện mô phỏng:
![Mạch điện mô phỏng](exercise_4/simulated_circuit.png)

- Mạch điện thực tế:
![Mạch điện thực tế](exercise_4/practical_circuit.jpg)

### Code
[Source code](exercise_4/exercise_4.ino)

### Khó khăn và cách giải quyết

<table>
  <thead>
    <tr>
      <th>Hiện tượng</th>
      <th>Nguyên nhân</th>
      <th>Cách phát hiện</th>
      <th>Cách xử lý</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td rowspan="2">Nạp code vào mạch điện thực tế thấy các đoạn LED không sáng như kì vọng - nhấn nút không hiển thị mã số nào.</td>
      <td>LED 7 đoạn mượn từ lab là loại Anode chung, không phải Cathode chung như loại đã dùng để mô phỏng trước đó.</td>
      <td>Cắm chân chung của LED 7 đoạn vào chân GND Arduino và cho chân nguồn của Arduino chạm vào từng chân điều khiển các đoạn LED thì không có đoạn nào sáng.</td>
      <td>Sửa code lại để khớp logic: muốn bật đèn thì xuất điện áp thấp - muốn tắt đèn thì xuất điện áp cao.</td>
    </tr>
    <tr>
      <td>Chưa nối chân Common của LED 7 đoạn vào chân nguồn 5V của Arduino.</td>
      <td>Nhìn hành vi của LED 7 đoạn trong mạch thực tế.</td>
      <td>Nối đoạn dây còn thiếu đó.</td>
    </tr>
  </tbody>
</table>


---

## Mình đã học được gì?
- Tranh thủ thời gian mô phỏng hoạt động của mạch ở nhà bằng phần mềm trước khi lên lab mượn thiết bị để cắm mạch thực tế.
- Tập thói quen sử dụng hàm `millis()` và một biến ghi lại mốc thời gian để tính giờ kích hoạt sự kiện, thay vì dùng `delay()` - một blocking function ("đóng băng" toàn bộ chương trình).
- Khi làm việc với nút bấm thì rất nên có logic chống nhiễu:
  - Nhiễu nút bấm là hiện tượng: Vì đặc tính cơ học, khi ta nhấn nút thật, hai lá kim loại bên trong nút chạm nhau rồi nảy lên nảy xuống liên tục trong vài mili giây trước khi ổn định. Và Arduino đọc được tín hiệu này thì lại "nghĩ" là người dùng nhấn nút nhiều lần liên tục.
  - Chống nhiễu là việc ta đợi khoảng vài chục ms cho tín hiệu tại nút bấm ổn định rồi mới xử lý.
- Bài 4: Tư duy lập trình kiểu máy trạng thái (state machine) *(dù mình không chủ động nhận ra điều đó khi thiết kế chương trình)*.
- Bài 4: Tự cắm mạch đơn giản để kiểm tra LED 7 đoạn là loại Anode chung hay Cathode chung trước khi dùng, tránh mất thời gian debug oan khi mạch không hoạt động như ý sau đó.