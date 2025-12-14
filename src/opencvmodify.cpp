#include <opencv2/opencv.hpp>
#include <iostream>
int main(){
    cv::Mat image = cv::imread("image/image.png");
    cv::Mat ModifiedImage;
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    cv::resize(image,ModifiedImage,cv::Size(640, 480));
    cv::imshow("Image", image);
    cv::imshow("ModifiedImage", ModifiedImage);
    if (cv::waitKey(0) == 'q') {
        cv::imwrite("image/ModifiedImage.png", ModifiedImage);
        // 释放资源
        image.release();
        ModifiedImage.release();
        cv::destroyAllWindows();
        return 0;
    }

    return 0;
}