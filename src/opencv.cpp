#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    cv::Mat image = cv::imread("image/image.png");
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    cv::imshow("Image", image);
    if (cv::waitKey(0) == 'q') {
        return 0;
    }
    // 释放资源
    image.release();
    cv::destroyAllWindows();

    return 0;
}