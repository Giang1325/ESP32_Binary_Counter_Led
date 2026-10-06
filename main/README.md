# ESP32 GPIO Register-Level Binary Counter

Bài tập thực hành **Module 2** trong lộ trình tự học Embedded Systems: điều khiển 6 LED hiển thị bộ đếm nhị phân, lập trình trực tiếp ở mức thanh ghi (register-level) trên ESP32 — không dùng driver có sẵn của ESP-IDF (`gpio_set_level()`, `gpio_config()`...).

## Mục tiêu

- Thao tác trực tiếp thanh ghi phần cứng thông qua con trỏ `volatile`.
- Dùng cơ chế **W1TS / W1TC** để set/clear bit an toàn, tránh race condition.
- Dùng bitwise (`(counter >> i) & 1`) để tách từng bit của một số, ánh xạ ra trạng thái LED tương ứng.

## Phần cứng sử dụng

| Thành phần | Số lượng | Ghi chú |
|---|---|---|
| ESP32 DevKitC (WROOM-32) | 1 | Board chính |
| LED đơn (nhiều màu) | 6 | GPIO 2, 4, 18, 19, 21, 23 |
| Điện trở 220Ω 1/4W | 6 | Giới hạn dòng cho mỗi LED |
| Breadboard + dây jumper | - | Lắp mạch không hàn |

> GPIO 6-11 (nối Flash nội bộ), GPIO 34-39 (chỉ input), GPIO 1/3 (UART0) không dùng làm output trong project này.

## Sơ đồ nối mạch

```
ESP32 GPIO(x) ──[Điện trở 220Ω]── Anode LED (+)
                                        │
                                  Cathode LED (−)
                                        │
                                       GND
```
Lặp lại cho cả 6 chân GPIO đã chọn.

## Cơ chế kỹ thuật (tóm tắt)

Mỗi 1 giây, chương trình tăng biến `counter` (0 → 63) và "vẽ" lại toàn bộ 6 LED theo dạng nhị phân của nó: bit nào = 1 thì LED tương ứng sáng, bit = 0 thì tắt. Việc bật/tắt dùng thanh ghi `GPIO_OUT_W1TS_REG` / `GPIO_OUT_W1TC_REG` (ghi thẳng, không đọc-sửa-ghi) để đảm bảo thao tác an toàn, không xung đột khi có nhiều task truy cập GPIO cùng lúc.

```c
#define GPIO_SET_BIT(bit)    (GPIO_OUT_W1TS_REG = (1U << (bit)))
#define GPIO_CLEAR_BIT(bit)  (GPIO_OUT_W1TC_REG = (1U << (bit)))
```

## Build & Flash

```bash
idf.py build
idf.py -p <PORT> flash monitor
```

---

*Phần của lộ trình tự học Embedded Systems hướng tới vị trí Embedded Software Engineer Fresher.*
