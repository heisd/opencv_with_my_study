#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
int main(){
    cv::Mat image = cv::imread("image/image.png");
    cv::Mat ModifiedImage;
    cv::Mat HSVImage;
    cv::Mat ContoursImage;
    if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    // 去噪
    cv::GaussianBlur(image, ModifiedImage, cv::Size(3, 3), 0);    
    // 转换为HSV
    cv::cvtColor(ModifiedImage, HSVImage, cv::COLOR_BGR2HSV);
    // 定义上下阈值
    cv::Scalar lower = cv::Scalar(0, 0, 0);
    cv::Scalar upper = cv::Scalar(180, 255, 255);
    // 二值化
    cv::inRange(HSVImage, lower, upper, ModifiedImage);
    // 膨胀,系数设置成为5   
    cv::dilate(ModifiedImage, ModifiedImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    // 腐蚀
    cv::erode(ModifiedImage, ModifiedImage, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));
    // 轮廓检测
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(ModifiedImage, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    // 绘制轮廓
    cv::drawContours(image, contours, -1, cv::Scalar(0, 255, 0), 2);
    // 显示结果
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