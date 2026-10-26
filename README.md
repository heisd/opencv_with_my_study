# OpenCV C++ 学习与视觉实验

从图像读取、绘图、滤波与变换逐步扩展到摄像头颜色分割和 YOLO 推理的学习仓库。主体为 OpenCV C++ 示例，另附模型导出脚本与学习笔记。

## 示例地图

| 源码 | 内容 | CMake 目标 |
| --- | --- | --- |
| [src/opencv.cpp](./src/opencv.cpp) | 读取 `image/image.png` 并显示 | opencv.exe |
| [src/opencvdraw.cpp](./src/opencvdraw.cpp) | 图形绘制 | opencvdraw.exe |
| [src/opencvtranslate.cpp](./src/opencvtranslate.cpp) | 图像变换练习 | opencvtranslate.exe |
| [src/opencvmodify.cpp](./src/opencvmodify.cpp) | 图像修改练习 | opencvmodify.exe |
| [src/opencvnoise/opencvnoiseBlur.cpp](./src/opencvnoise/opencvnoiseBlur.cpp) | 均值模糊 | opencvnoise.exe |
| [src/opencvnoise/opencvnoiseGaussianBlur.cpp](./src/opencvnoise/opencvnoiseGaussianBlur.cpp) | 高斯模糊 | opencvnoisegaussian.exe |
| [src/opencvnoise/opencvnoiseMedianBlur.cpp](./src/opencvnoise/opencvnoiseMedianBlur.cpp) | 中值滤波 | opencvnoisemedian.exe |
| [src/opencvcamera_noise.cpp](./src/opencvcamera_noise.cpp) | 摄像头处理练习 | opencvcamera_noise.exe |
| [src/opencvcamera_object.cpp](./src/opencvcamera_object.cpp) | 摄像头 HSV 阈值与滑块调节 | opencvcamera_object.exe |
| [src/yolo/opencvyolo.cpp](./src/yolo/opencvyolo.cpp) | OpenCV DNN / YOLO 推理 | opencvyolo.exe |

[src/objectgrab.cpp](./src/objectgrab.cpp) 是额外实验文件，当前构建列表没有为它定义独立目标。并不是 `src/` 中每个文件都会自动编译。

## 目录结构

- [src/](./src)：源码。
- [image/](./image)：图片资源。
- [reference/](./reference)：C++ OpenCV、目标识别与 YOLO 等学习资料。
- [src/yolo/](./src/yolo)：推理代码、导出脚本与模型文件。
- [CmakeLists.txt](./CmakeLists.txt)：现有构建配置，注意文件名大小写。
- 仓库根目录还保留原环境生成的 Makefile、CMake 缓存与可执行文件，首次使用应在本机重新生成。

## 依赖

C++ 部分需要：

| 依赖 | 仓库配置 |
| --- | --- |
| 编译器 | C++11 |
| CMake | 3.10+ |
| OpenCV 开发库 | 通过 `find_package(OpenCV REQUIRED)` 查找 |
| 图形桌面 | `imshow` 和滑块窗口需要显示支持 |
| 摄像头 | 实时示例需要可访问的设备 |

Python 导出脚本的环境与 C++ 推理环境需要分别配置，参阅 [YOLO 学习记录](./reference/yolo/yolo.md) 和 [导出脚本](./src/yolo/get_yolo_onnx.py)。OpenCV DNN 是否能正确加载模型还取决于模型格式、算子和 OpenCV 版本。

## 首次构建：注意文件名

CMake 寻找 `CMakeLists.txt`，仓库当前名称是 `CmakeLists.txt`。在区分大小写的文件系统上，需要先更正；例如 Linux/macOS：

```bash
git clone --branch liquanyan https://github.com/heisd/opencv_with_my_study.git
cd opencv_with_my_study
mv CmakeLists.txt CMakeLists.txt
cmake -S . -B build-local
cmake --build build-local --target opencv.exe
./build-local/opencv.exe
```

文件重命名是使用者构建前的调整步骤，本次 README 更新没有修改构建文件。若 OpenCV 安装在非标准位置，可将实际配置目录传给 CMake：

```bash
cmake -S . -B build-local -DOpenCV_DIR=/path/to/opencv/cmake
```

上面的路径需要换成本机包含 OpenCV 配置文件的目录。使用新的 `build-local/` 可以避免直接复用仓库自带缓存。

## 运行工作目录与图片

最简单示例执行 `imread("image/image.png")`，因此应从仓库根目录运行可执行文件；从构建目录直接启动可能找不到图片。其他示例的资源路径可能不同，应逐个检查。

初始验证建议只构建 `opencv.exe`，确认编译、OpenCV 查找和窗口显示后，再构建摄像头与 DNN 目标。

## HSV 摄像头示例

`opencvcamera_object.cpp` 默认打开设备 0，创建原图、掩码与 Trackbars 窗口。当前 H 滑块范围为 0–179，S/V 范围为 0–255。

```bash
cmake --build build-local --target opencvcamera_object.exe
./build-local/opencvcamera_object.exe
```

先确认设备可以打开，再调节阈值观察掩码。目标分割效果会受到光照、背景和曝光影响，不应仅凭一次固定阈值判断识别能力。

## 常见问题

| 现象 | 检查方式 |
| --- | --- |
| 找不到 CMakeLists.txt | 修正现有文件名大小写 |
| 找不到 OpenCV | 安装开发库，核对 OpenCV_DIR 与编译器架构 |
| 缓存指向另一台机器 | 使用新的构建目录重新配置 |
| Could not open or find the image | 从仓库根目录运行并检查资源 |
| 摄像头无法打开 | 设备索引、权限以及是否被其他程序占用 |
| ONNX 加载或输出解析失败 | 核对模型版本、输入尺寸与代码使用的输出格式 |

各示例保留学习性质，仓库未声明统一的跨系统兼容或模型精度验证。
