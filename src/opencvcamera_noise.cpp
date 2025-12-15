#include <iostream>
#include <opencv2/opencv.hpp>

int main() {
  // 1. 打开摄像头 (0 表示默认摄像头)
  cv::VideoCapture cap(0);

  if (!cap.isOpened()) {
    std::cerr << "无法打开摄像头" << std::endl;
    return -1;
  }

  std::cout << "按 'q' 退出程序" << std::endl;

  cv::Mat frame, denoisedFrame;

  while (true) {
    // 2. 逐帧读取视频
    cap >> frame; // 或者 cap.read(frame);

    if (frame.empty()) {
      std::cerr << "无法获取图像帧" << std::endl;
      break;
    }

    // 3. 实时去噪处理
    // 这里使用高斯模糊作为演示，速度快，适合实时视频
    // 参数说明: 输入帧, 输出帧, 核大小(必须是奇数), X方向标准差
    cv::GaussianBlur(frame, denoisedFrame, cv::Size(7, 7), 0);

    // 如果机器性能足够好，可以尝试双边滤波(磨皮效果)，但计算量大可能会卡顿
    // cv::bilateralFilter(frame, denoisedFrame, 9, 75, 75);

    // 4. 显示结果
    cv::imshow("Original Video", frame);
    cv::imshow("Denoised Video", denoisedFrame);

    // 5. 等待按键，延时30ms (约30fps)
    if (cv::waitKey(30) == 'q') {
      break;
    }
  }

  // 释放资源
  cap.release();
  cv::destroyAllWindows();
  return 0;
}
