# opencv_with_my_study

OpenCV C++ 学习与视觉实验，包含图像绘制、几何变换、滤波、摄像头处理和 YOLO 相关示例。

## 内容导航

- [src](./src)：C++ 源码
- [src/opencvnoise](./src/opencvnoise)：滤波示例
- [src/yolo](./src/yolo)：YOLO 推理与模型导出相关文件
- [reference](./reference)：学习笔记
- [image](./image)：示例图片
- [CmakeLists.txt](./CmakeLists.txt)：现有构建配置

## 构建前检查

项目配置使用 C++11，并通过 `find_package(OpenCV REQUIRED)` 查找 OpenCV 开发库。

当前构建文件名为 `CmakeLists.txt`，与 CMake 要求的 `CMakeLists.txt` 大小写不同；在区分大小写的系统上须先修正文件名。仓库内的缓存和可执行文件来自原开发环境，应在本机重新生成。

各示例使用的图片、摄像头与模型路径需要按源码核对，YOLO 相关说明见 `reference/yolo/yolo.md`。
