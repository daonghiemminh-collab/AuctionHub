1. Đối với mỗi thuật toán, giải thích (2–3 câu) lý do thực tế khi sử dụng nó trong module tương ứng:

1.1. Insertion Sort (Live Auction Monitor):

* Luồng dữ liệu đặt giá trực tiếp đến hệ thống gần như đã có thứ tự tăng dần theo thời gian.
* Insertion Sort đạt độ phức tạp thời gian tuyến tính O(N) trên tập dữ liệu gần như đã sắp xếp với số lượng nghịch thế rất ít.
* Đây là thuật toán dạng online, cho phép chèn ngay lượt bid mới vào danh sách với số phép dịch chuyển tối thiểu mà không cần sắp xếp lại toàn bộ.

1.2. Selection Sort (Top-K Bid Finder):

* Bảng điều khiển đấu giá thường chỉ yêu cầu hiển thị Top-K mức giá cao nhất từ nhóm dữ liệu đang hoạt động.
* Selection Sort có thể dừng lại ngay sau đúng K vòng lặp ngoài, đạt thời gian thực thi O(K \* N) thay vì phải sắp xếp toàn bộ tập dữ liệu.
* Thuật toán thao tác trực tiếp tại chỗ không tốn bộ nhớ phụ và giảm thiểu tối đa số lần hoán đổi dữ liệu.

1.3. Interchange Sort (Price Anomaly Validator):

* Việc kiểm tra các lô dữ liệu nhỏ (từ 50 bản ghi trở xuống) ưu tiên tính đơn giản và khả năng kiểm toán mã nguồn hơn là tốc độ tiệm cận lý thuyết.
* Với kích thước dữ liệu nhỏ, tổng số phép toán là không đáng kể và hoàn thành trong chưa đầy hai microsecond.
* Cơ chế so sánh và đổi chỗ trực tiếp cung cấp bộ đếm số lần hoán vị cụ thể, giúp đo lường trực tiếp mức độ sai lệch thứ tự giá.

1.4. Bubble Sort with Early Stop (Stabilization Detector):

* Luồng giá được coi là ổn định khi các mức đặt giá ngừng biến động và duy trì thứ tự không giảm.
* Bubble Sort kết hợp cờ dừng sớm có thể xác thực một mảng đã có thứ tự sẵn chỉ qua một lượt duyệt duy nhất gồm N-1 phép so sánh.
* Nếu không có lượt hoán đổi nào xảy ra ở lượt đầu tiên, hệ thống lập tức nhận diện trạng thái ổn định với độ phức tạp tốt nhất là O(N).

1.5. Quick Sort (Full Historical Sorter):

* Tác vụ xử lý báo cáo cuối ngày cần sắp xếp hàng triệu bản ghi lịch sử, trong đó thông lượng xử lý cao là tiêu chí bắt buộc.
* Quick Sort cung cấp độ phức tạp trung bình O(N log N) với hệ số hằng số thấp và tận dụng tối ưu bộ nhớ đệm.
* Thuật toán mang lại tốc độ thực thi rất cao trên các tập dữ liệu lớn khi kết hợp kỹ thuật chọn phần tử chốt median-of-three.
2. Báo cáo thời gian chạy thực tế của Quick Sort trên 100k bản ghi:
* Bộ dữ liệu kiểm thử: test\_data/large.txt (100,000 lượt đặt giá).
* Thời gian thực thi: 10.41 ms (khoảng 0.010 giây).
* Tốc độ thông lượng: 9,605,687 lượt đặt giá/giây.
* Trạng thái kiểm tra tính đúng đắn: ĐẠT (xác thực mảng được sắp xếp bằng std::is\_sorted).
3. Mô tả một lỗi bạn gặp phải và cách bạn (hoặc LLM) chẩn đoán lỗi đó:
* Lỗi gặp phải: Lỗi biên dịch biến largeBids chưa được khai báo trong hàm main ở Task E, cùng với lỗi tham chiếu pivot bị thay đổi giá trị giữa chừng trong vòng lặp phân hoạch khi có các mức giá bằng nhau.
* Cách chẩn đoán và khắc phục: Chẩn đoán thông qua thông báo lỗi của trình biên dịch g++ và phân tích luồng truy cập con trỏ; khắc phục bằng cách khai báo nạp dữ liệu cho largeBids và lưu pivot dưới dạng bản sao giá trị độc lập thay vì biến tham chiếu trỏ vào mảng.
4. Liệt kê ba câu lệnh prompt bạn đã gửi cho LLM và xếp loại Xanh / Vàng / Đỏ theo chính sách sử dụng:
* Prompt 1: "Giải thích nguyên lý chọn phần tử chốt median-of-three trong Quick Sort và cách nó ngăn ngừa trường hợp xấu nhất." -> Xanh.
* Prompt 2: "Chỉnh lại lời văn và tách các ý ra thành gạch đầu dòng và các mục" -> Xanh.
* Prompt 3: "Kiểm tra giúp tôi tại sao logic phân hoạch gây lỗi truy cập ngoài mảng khi các lượt đặt giá có cùng số tiền." -> Vàng.
5. 
6. Nêu độ phức tạp trong trường hợp xấu nhất của Quick Sort với pivot median-of-three. Liệu nó có còn là O(n²) không? Tại sao có hoặc tại sao không?:
* Độ phức tạp trường hợp xấu nhất: O(n^2).
* Liệu có còn là O(n^2) không: Có.
* Lý do:
* Kỹ thuật median-of-three chỉ loại bỏ trường hợp xấu nhất trên các mảng đã có thứ tự tăng dần hoặc giảm dần, nhưng không thể triệt tiêu hoàn toàn trường hợp O(n^2) về mặt lý thuyết.
* Người ta vẫn có thể thiết kế một chuỗi dữ liệu đối kháng đặc biệt sao cho trung vị của ba phần tử đầu, giữa và cuối liên tục rơi vào phần tử nhỏ thứ hai hoặc lớn thứ hai trong phân vùng.
* Khi đó, mảng luôn bị chia lệch thành hai phần kích thước 1 và n - 2 sau mỗi bước, dẫn đến n mức đệ quy và tổng thời gian thực thi vẫn bị đẩy lên mức bình phương O(n^2).

