#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::VideoCapture cap(0); // Use the first camera (0).

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
        if (key == 'q' || key == 27) break; // Press "q" or "Esc" to exit.
    }

    cap.release();
    cv::destroyAllWindows();
    std::cout << "🔚 Camera closed." << std::endl;
    return 0;
}
