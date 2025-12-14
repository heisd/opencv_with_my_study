#include <iostream>
#include <opencv2/core/mat.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>


int main(){
    // 创建两个图片对象
    cv::Mat image =cv::imread("image/image.png");
    cv::Mat gray;
    cv::Mat HSV;
    cv::Mat RGB;
    // 检测是否成功加载
     if (image.empty()) {
        std::cout << "Could not open or find the image" << std::endl;
        return -1;
    }
    // 转换为灰度图
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    // 转换为 HSV 图
    cv::cvtColor(image, HSV, cv::COLOR_BGR2HSV);
    // 转换为 RGB 图
    cv::cvtColor(image, RGB, cv::COLOR_BGR2RGB);
   
    cv::imshow("Image", image);
    cv::imshow("Gray", gray);
    cv::imshow("HSV", HSV);
    cv::imshow("RGB",RGB);
    if (cv::waitKey(0) == 'q' || cv::waitKey(0) == ' ') {
        std::cout << "q or space is pressed" << std::endl;
        // 保存图片
        cv::imwrite("image/gray.png", gray);
        cv::imwrite("image/HSV.png", HSV);
        cv::imwrite("image/RGB.png", RGB);
        // 释放资源
        image.release();
        HSV.release();
        gray.release();
        cv::destroyAllWindows();
        return 0;
    }   

}
