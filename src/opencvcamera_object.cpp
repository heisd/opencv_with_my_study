#include <iostream>
#include <opencv2/opencv.hpp>

// 全局变量，用于 Trackbar 调节
int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;

void on_trackbar(int, void *) {
  // 回调函数，什么都不用做，或者可以在这里打印值
}

int main() {
  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "无法打开摄像头" << std::endl;
    return -1;
  }

  // 创建窗口
  cv::namedWindow("Trackbars");
  cv::resizeWindow("Trackbars", 640, 200);
  cv::namedWindow("Original");
  cv::namedWindow("Mask");

  // 创建滑动条，用于实时调整 HSV 阈值
  // HSV 范围: H(0-179), S(0-255), V(0-255)
  cv::createTrackbar("H Min", "Trackbars", &hmin, 179, on_trackbar);
  cv::createTrackbar("H Max", "Trackbars", &hmax, 179, on_trackbar);
  cv::createTrackbar("S Min", "Trackbars", &smin, 255, on_trackbar);
  cv::createTrackbar("S Max", "Trackbars", &smax, 255, on_trackbar);
  cv::createTrackbar("V Min", "Trackbars", &vmin, 255, on_trackbar);
  cv::createTrackbar("V Max", "Trackbars", &vmax, 255, on_trackbar);
  

  cv::Mat frame, hsv, mask;

  while (true) {
    cap >> frame;
    if (frame.empty())
      break;

    // 1. 预处理：高斯模糊去噪
    cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0);

    // 2. 颜色空间转换 BGR -> HSV
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    // 3. 颜色分割：根据 Trackbar 的值生成掩码
    cv::Scalar lower(hmin, smin, vmin);
    cv::Scalar upper(hmax, smax, vmax);
    cv::inRange(hsv, lower, upper, mask);

    // 4. 形态学操作：去噪
    // 开运算：去除背景中的小白点
    // 闭运算：填充物体内部的小黑洞
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    // 5. 轮廓检测
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL,
                     cv::CHAIN_APPROX_SIMPLE);

    // 6. 查找最大轮廓并画框
    double maxArea = 0;
    int maxIdx = -1;

    for (size_t i = 0; i < contours.size(); i++) {
      double area = cv::contourArea(contours[i]);
      // 过滤掉太小的噪点
      if (area > 1000) {
        if (area > maxArea) {
          maxArea = area;
          maxIdx = i;
        }
      }
    }

    if (maxIdx != -1) {
      // 获取外接矩形
      cv::Rect box = cv::boundingRect(contours[maxIdx]);
      // 在原图上画框
      cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
      // 画中心点
      cv::circle(frame,
                 cv::Point(box.x + box.width / 2, box.y + box.height / 2), 5,
                 cv::Scalar(0, 0, 255), -1);

      // 打印坐标
      std::string text =
          "x:" + std::to_string(box.x) + " y:" + std::to_string(box.y);
      cv::putText(frame, text, cv::Point(box.x, box.y - 10),
                  cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }

    // 显示图像
    cv::imshow("Original", frame);
    cv::imshow("Mask", mask);

    if (cv::waitKey(30) == 'q')
      break;
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
