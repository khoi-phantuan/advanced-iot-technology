# Lab 1 — Giới thiệu về Arduino

*đang cập nhật*

<!-->

## Mục tiêu lab
Trong bài lab này, mình sẽ tìm hiểu về:
- Các khái niệm cơ bản trong lập trình nhúng,
- Nền tảng cơ bản về Arduino, bao gồm mạch phát triển, framework, môi trường/công cụ sử dụng, phong cách lập trình để tạo ra những ứng dụng IoT đơn giản đầu tiên,
- Cách kết hợp bo mạch Arduino với một số linh kiện điện tử cơ bản như đèn LED, điện trở, biến trở, breadboard, nút bấm,... để thực hiện thêm nhiều kịch bản ứng dụng IoT khác.

## Linh kiện / Thiết bị
- Arduino Uno R3
- LED 7 đoạn

## Môi trường / Công cụ
- Arduino IDE

## Mình đã làm gì?

## Khó khăn và cách giải quyết
1. Bài 1:
- Cắm mạch vật lý thấy value và LED nhảy loạn xạ, lúc không nhấn nút thì chuyển trạng thái, lúc nhấn nút thì không phản ứng gì, logic nút counter và direction lẫn lộn nhau,...  
=> Do chưa nối phía còn lại của 2 nút bấm về GND của Arduino. 2 nút bấm chỉ đang có một phía được nối vào nguồn 5V, chưa có đường về GND -> loạn tín hiệu.
2. Bài 2:
- Phát hiện LED 7 đoạn mượn từ phòng lab là loại Anode chung - không phải Cathode chung như dự kiến.
- Nạp code mạch không chạy, LED 7 đoạn không sáng  
=> Nối dây nguồn chung còn thiếu.

## Mình đã học được gì?

## (tùy chọn) Hướng phát triển thêm -->
