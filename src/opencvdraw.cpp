#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
int main(){
    cv::Mat Ground = cv::imread("/Users/liquanyan/Desktop/OpenCV_CPP/image/DrawBackGround.png");
    if(Ground.empty()){
        printf("文件打开失败,没有这个文件");
        return -1;
    }
    // pronlem is there Ground.size() == 0
    std::cout<<"图片的高度为"<<Ground.size().height<<std::endl;
    std::cout<<"图片的宽度为"<<Ground.size().width<<std::endl;
    std::cout<<"图片的面积为"<<Ground.size().area()<<std::endl;
    // opencv的绘图工具通常是原图操作,rectangle是绘制矩形,Scalar是颜色,最后一个参数是线宽,-1是填充,1是画边缘线
    // 画图工具有长方形,圆和线
    cv::rectangle(Ground, cv::Point(100, 100), cv::Point(200, 200), cv::Scalar(0, 0, 0), 1);
    cv::imshow("Ground", Ground);
    if(cv::waitKey(0) == 'q'){
        cv::imwrite("/Users/liquanyan/Desktop/OpenCV_CPP/image/Rectangle.png", Ground);
        Ground.release();
        cv::destroyAllWindows();
        return 0;
    }

    return 0;

}