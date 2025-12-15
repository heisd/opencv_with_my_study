
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>

// COCO 数据集 80 个类别名称
const std::vector<std::string> class_names = {
    "person",        "bicycle",      "car",
    "motorcycle",    "airplane",     "bus",
    "train",         "truck",        "boat",
    "traffic light", "fire hydrant", "stop sign",
    "parking meter", "bench",        "bird",
    "cat",           "dog",          "horse",
    "sheep",         "cow",          "elephant",
    "bear",          "zebra",        "giraffe",
    "backpack",      "umbrella",     "handbag",
    "tie",           "suitcase",     "frisbee",
    "skis",          "snowboard",    "sports ball",
    "kite",          "baseball bat", "baseball glove",
    "skateboard",    "surfboard",    "tennis racket",
    "bottle",        "wine glass",   "cup",
    "fork",          "knife",        "spoon",
    "bowl",          "banana",       "apple",
    "sandwich",      "orange",       "broccoli",
    "carrot",        "hot dog",      "pizza",
    "donut",         "cake",         "chair",
    "couch",         "potted plant", "bed",
    "dining table",  "toilet",       "tv",
    "laptop",        "mouse",        "remote",
    "keyboard",      "cell phone",   "microwave",
    "oven",          "toaster",      "sink",
    "refrigerator",  "book",         "clock",
    "vase",          "scissors",     "teddy bear",
    "hair drier",    "toothbrush"};

// 预处理常量
const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.2; // 置信度阈值
const float NMS_THRESHOLD = 0.4;   // 非极大值抑制阈值
const float CONFIDENCE_THRESHOLD = 0.4;

int main() {
  // 1. 加载模型
  // 警告：确保 'yolov5s.onnx' 文件在 src/yolo/ 目录下！
  std::string model_path = "src/yolo/yolov5s.onnx";
  cv::dnn::Net net;
  try {
    net = cv::dnn::readNet(model_path);
  } catch (cv::Exception &e) {
    std::cerr << "OpenCV 模型加载异常: " << e.what() << std::endl;
    std::cerr
        << "通常这是因为 ONNX 版本不兼容。请尝试使用 --opset 12 重新导出模型。"
        << std::endl;
    return -1;
  }

  if (net.empty()) {
    std::cerr << "加载模型失败！请确保 '" << model_path << "' 存在。"
              << std::endl;
    return -1;
  }

  // 如果有 GPU，可以使用 CUDA 加速（这里默认使用 CPU）
  net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
  net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

  cv::VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cerr << "无法打开摄像头" << std::endl;
    return -1;
  }

  cv::Mat frame;
  while (true) {
    cap >> frame;
    if (frame.empty())
      break;

    // 2. 预处理：图像转 Blob
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1.0 / 255.0,
                           cv::Size(INPUT_WIDTH, INPUT_HEIGHT), cv::Scalar(),
                           true, false);
    net.setInput(blob);

    // 3. 推理
    std::vector<cv::Mat> outputs;
    net.forward(outputs, net.getUnconnectedOutLayersNames());

    // DEBUG: 打印输出层信息
    static bool printed_debug = false;
    if (!printed_debug) {
      std::cout << "Model loaded successfully!" << std::endl;
      std::cout << "Number of output layers: " << outputs.size() << std::endl;
      for (size_t i = 0; i < outputs.size(); ++i) {
        std::cout << "Output layer " << i << " dims: " << outputs[i].dims
                  << ", size: [";
        for (int j = 0; j < outputs[i].dims; ++j) {
          std::cout << outputs[i].size[j]
                    << (j < outputs[i].dims - 1 ? " x " : "");
        }
        std::cout << "]" << std::endl;
      }
      printed_debug = true;
    }

    // 4. 后处理
    // YOLOv5 的输出通常是一个矩阵，每一行代表一个检测框
    // 格式: [x, y, w, h, confidence, class1_score, class2_score, ...]

    float *data = (float *)outputs[0].data;
    // 输出的维度：rows = 25200 (640x640网格预测数), cols = 85 (xywh + conf + 80
    // classes) opencv dnn 输出维度顺序可能不同，这里按标准 yolov5 onnx 结构处理
    // rows 是框的数量，cols 是特征维度
    int rows = outputs[0].size[1];
    int cols = outputs[0].size[2];
    // 但是 opencv 读取 onnx 具体维度依赖版本，通常是 [1, 25200, 85]
    // 所以 data 指针遍历时要注意

    if (outputs[0].dims == 3) {
      rows = outputs[0].size[1];
      cols = outputs[0].size[2];
    }

    std::vector<int> class_ids;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    // 图像缩放比例
    float x_factor = frame.cols / INPUT_WIDTH;
    float y_factor = frame.rows / INPUT_HEIGHT;

    // 遍历所有预测框
    // 注意：data 的布局如果是 [1, 25200, 85]，则总元素是 rows * cols
    // OpenCV 4.5+ 直接返回 [1, 25200, 85]

    for (int i = 0; i < rows; ++i) {
      float confidence = data[4];
      if (confidence >= CONFIDENCE_THRESHOLD) {
        float *classes_scores = data + 5;
        cv::Mat scores(1, class_names.size(), CV_32FC1, classes_scores);
        cv::Point class_id;
        double max_class_score;
        minMaxLoc(scores, 0, &max_class_score, 0, &class_id);

        if (max_class_score > SCORE_THRESHOLD) {
          confidences.push_back(confidence);
          class_ids.push_back(class_id.x);

          float x = data[0];
          float y = data[1];
          float w = data[2];
          float h = data[3];

          int left = int((x - 0.5 * w) * x_factor);
          int top = int((y - 0.5 * h) * y_factor);
          int width = int(w * x_factor);
          int height = int(h * y_factor);
          boxes.push_back(cv::Rect(left, top, width, height));
        }
      }
      // 移动到下一行数据
      data += cols;
    }

    // 5. NMS (非极大值抑制) 去重
    std::vector<int> nms_result;
    cv::dnn::NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD,
                      nms_result);

    // 6. 绘图
    for (int idx : nms_result) {
      cv::Rect box = boxes[idx];
      int class_id = class_ids[idx];
      float conf = confidences[idx];

      cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);

      std::string label =
          class_names[class_id] + ": " + cv::format("%.2f", conf);
      int baseLine;
      cv::Size labelSize =
          cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

      int top = std::max(box.y, labelSize.height);
      cv::rectangle(frame, cv::Point(box.x, top - labelSize.height),
                    cv::Point(box.x + labelSize.width, top + baseLine),
                    cv::Scalar(0, 255, 0), -1);
      cv::putText(frame, label, cv::Point(box.x, top), cv::FONT_HERSHEY_SIMPLEX,
                  0.5, cv::Scalar(0, 0, 0), 1);
    }

    cv::imshow("YOLOv5 Detection", frame);
    if (cv::waitKey(1) == 'q')
      break;
  }

  cap.release();
  cv::destroyAllWindows();
  return 0;
}
