#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
// 这个是图像边缘处理和噪音处理的函数
// 也是我们图像识别的核心思路
int main(){
    cv::Mat OriginImage =cv::imread("/Users/liquanyan/Desktop/OpenCV_CPP/image/Origin.jpg");
    cv::Mat RemovalNoiseImage;
    if(OriginImage.empty()){
        std::cout<<"图片加载失败"<<std::endl;
        return -1;
    }
    // 噪音处理,使用中值去除噪声,ksize值一般选择3
    cv::blur(OriginImage,RemovalNoiseImage ,cv::Size(7,7));
    cv::imshow("RemovalNoiseImage",RemovalNoiseImage);
    if(cv::waitKey(0)=='q'){
        cv::imwrite("/Users/liquanyan/Desktop/OpenCV_CPP/image/blur/ksize7.jpg", RemovalNoiseImage);
        std::cout<<"图片保存成功"<<std::endl;
        OriginImage.release();
        RemovalNoiseImage.release();
        cv::destroyAllWindows();
        return 0;
    }
    return 0;
}