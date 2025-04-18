#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0); // 使用第一个摄像头（0）

    if (!cap.isOpened()) {
        std::cerr << "❌ Failed to open the camera!" << std::endl;
        return -1;
    }

    cv::Mat frame;
    std::cout << "✅ Camera opened successfully. Press 'q' to quit." << std::endl;

    while (true) {
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "⚠️ Empty frame received!" << std::endl;
            break;
        }

        cv::imshow("📷 Live Camera Preview", frame);

        char key = (char)cv::waitKey(1);
        if (key == 'q' || key == 27) break; // 按 q 或 Esc 退出
    }

    cap.release();
    cv::destroyAllWindows();
    std::cout << "🔚 Camera closed." << std::endl;
    return 0;
}
