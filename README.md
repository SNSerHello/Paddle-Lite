#  Paddle Lite

## 在Windows下，编译Paddle-Lite

```
1. where python
2. lite\tools\build_windows.bat with_profile with_extra with_static_mkl with_opencl use_vs2017
```
**注意**：

* 在Windows下，with_precision_profile有bugs，暂时不支持，所以编译的时候请不要加入
* paddle_lit_opt无法在windows下直接运行，所以使用`python your_path\paddle_lite_opt --help`来获知HOWTO
* 当前Paddle-Lite仅仅支持python3.5，python3.6与python3.7，其他版本可能存在兼容性问题

## 在Linux下，编译Paddle-Lite

```
$ lite/tools/build_linux.sh --arch=x86 --toolchain=gcc --with_extra=ON --with_python=ON --python_version=3.7 --with_log=ON --with_exception=ON --with_profile=ON --with_precision_profile=ON --with_static_mkl=ON --with_avx=ON --with_opencl=ON
```

## 通过paddle_lite_opt制作模型优化文件

以mobilenet v1模型为样例

```
$ cd test
$ wget http://paddle-inference-dist.bj.bcebos.com/mobilenet_v1.tar.gz
$ tar xvzf mobilenet_v1.tar.gz
$ paddle_lite_opt --model_dir mobilenet_v1 --optimize_out_type naive_buffer --optimize_out mobilenet_v1_opencl --valid_targets opencl
$ python3 test_paddlelite_opencl.py
```

## 如何查看LOG信息

### Windows

```
setx GLOG_v 5
```

### Linux

```
export GLOG_v=5
```

## Debug Paddle-Lite程序

### Linux

```
strace python xxx.py
```

## 如何在Windows下编译运行C++推断程序

```
1. cd test
2. mkdir build
3. cd build
4. cmake -DWIN64=1 -G "Visual Studio 15 2017 Win64" ..
5. set curr_dir=%cd%
6. "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Visual Studio 2017\Visual Studio Tools\VC\x64 Native Tools Command Prompt for VS 2017.lnk"
7. cd %curr_dir% 
8. msbuild /p:Configuration=Release test_paddlelite_opencl.vcxproj
9. cd ..
10. build\Release\test_paddlelite_opencl.exe
```

## 如何在Linux下编译运行C++推断程序

```
01. sudo apt install libopencv-dev
02. cd test
03. wget http://paddle-inference-dist.bj.bcebos.com/mobilenet_v1.tar.gz
04. tar xvzf mobilenet_v1.tar.gz
05. paddle_lite_opt --model_dir mobilenet_v1 --optimize_out_type naive_buffer --optimize_out mobilenet_v1_opencl --valid_targets opencl
06. mkdir build
07. cd build
08. cmake ..
09. make
10. cd ..
11. python3 test_redis.py
12. ./build/test_redis
13. ./build/test_paddlelite_opencl
14. python3 test_paddlelite_opencl.py
```

## 使用ONNX推断Predict-Lite模型

```
$ cd test
$ pip3 install --upgrade paddle2onnx==0.8.2
$ paddle2onnx --model_dir mobilenet_v1 --opset_version 9 --save_file mobilenet_v1.onnx --enable_onnx_checker True
$ pip3 install --upgrade onnxruntime
$ python3 test_paddlelite_onnx.py
```

## 下载最新Netron

访问[Netron Release](https://github.com/lutzroeder/netron/releases)，下载最新的netron版本。

2022年5月31日当前最新版本是5.8.2，可以通过如下命令下载：
```
$ wget https://github.com/lutzroeder/netron/releases/download/v5.8.2/Netron-Setup-5.8.2.exe
```

## Linux Docker Building Environment in DockerHub

[anaconda3 in dockerhub](https://hub.docker.com/r/snser/anaconda3)

```
$ docker login
$ docker pull snser/anaconda3
$ docker run -itv your_path/Paddle-Lite:/workspace -w /workspace snser/anaconda3 /bin/bash
$ conda env list
$ conda activate python3.7
$ lite/tools/build_linux.sh --arch=x86 --toolchain=gcc --with_extra=ON --with_python=ON --python_version=3.7 --with_log=ON --with_exception=ON --with_profile=ON --with_precision_profile=ON --with_static_mkl=ON --with_avx=ON --with_opencl=ON
```


[English](README_en.md) | 简体中文

  [![Documentation Status](https://img.shields.io/badge/中文文档-最新-brightgreen.svg)](https://www.paddlepaddle.org.cn/lite)  [![Release](https://img.shields.io/github/release/PaddlePaddle/Paddle-Lite.svg)](https://github.com/PaddlePaddle/Paddle-Lite/releases)  [![License](https://img.shields.io/badge/license-Apache%202-blue.svg)](LICENSE) 

Paddle Lite 是一个高性能、轻量级、灵活性强且易于扩展的深度学习推理框架，定位于支持包括移动端、嵌入式以及边缘端在内的多种硬件平台。

当前 Paddle Lite 不仅在百度内部业务中得到全面应用，也成功支持了众多外部用户和企业的生产任务。

## 快速入门

使用 Paddle Lite，只需几个简单的步骤，就可以把模型部署到多种终端设备中，运行高性能的推理任务，使用流程如下所示：

**一. 准备模型**

Paddle Lite 框架直接支持模型结构为 [PaddlePaddle](https://github.com/PaddlePaddle/Paddle) 深度学习框架产出的模型格式。目前 PaddlePaddle 用于推理的模型是通过 [save_inference_model](https://www.paddlepaddle.org.cn/documentation/docs/zh/develop/api/paddle/static/save_inference_model_cn.html#save-inference-model) 这个 API 保存下来的。
如果您手中的模型是由诸如 Caffe、Tensorflow、PyTorch 等框架产出的，那么您可以使用 [X2Paddle](https://github.com/PaddlePaddle/X2Paddle) 工具将模型转换为 PaddlePaddle 格式。

**二. 模型优化**

Paddle Lite 框架拥有优秀的加速、优化策略及实现，包含量化、子图融合、Kernel 优选等优化手段。优化后的模型更轻量级，耗费资源更少，并且执行速度也更快。
这些优化通过 Paddle Lite 提供的 opt 工具实现。opt 工具还可以统计并打印出模型中的算子信息，并判断不同硬件平台下 Paddle Lite 的支持情况。您获取 PaddlePaddle 格式的模型之后，一般需要通过该 opt 工具做模型优化。opt 工具的下载和使用，请参考[模型优化方法](https://www.paddlepaddle.org.cn/lite/develop/user_guides/model_optimize_tool.html)。

**三. 下载或编译**

Paddle Lite 提供了 Android/iOS/x86/macOS 平台的官方 Release 预测库下载，我们优先推荐您直接下载 [Paddle Lite 预编译库](https://www.paddlepaddle.org.cn/lite/develop/quick_start/release_lib.html)，或者从 Release notes 处获取最新的[预编译编译库](https://github.com/PaddlePaddle/Paddle-Lite/releases)。

Paddle Lite 已支持多种环境下的源码编译，为了避免复杂、繁琐的环境搭建过程，我们建议您使用 [Docker 统一编译环境搭建](https://www.paddlepaddle.org.cn/lite/develop/source_compile/docker_env.html) 进行编译。当然，您也可以根据宿主机和目标设备的 CPU 架构和操作系统，在[源码编译](https://www.paddlepaddle.org.cn/lite/develop/source_compile/compile_env.html)中找到相应的环境搭建及编译指南，自行完成编译环境的搭建。

**四. 预测示例**

Paddle Lite 提供了 C++、Java、Python 三种 API，并且提供了相应 API 的完整使用示例:

- [C++ 完整示例](https://www.paddlepaddle.org.cn/lite/develop/user_guides/cpp_demo.html)
- [Java 完整示例](https://www.paddlepaddle.org.cn/lite/develop/user_guides/java_demo.html)
- [Python 完整示例](https://www.paddlepaddle.org.cn/lite/develop/user_guides/python_demo.html)

您可以参考示例中的说明快速了解使用方法，并集成到您自己的项目中去。

针对不同的硬件平台，Paddle Lite 提供了各个平台的完整示例：

- [Android apps](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/android_app_demo.html) [[图像分类]](https://paddlelite-demo.bj.bcebos.com/apps/android/mobilenet_classification_demo.apk)  [[目标检测]](https://paddlelite-demo.bj.bcebos.com/apps/android/yolo_detection_demo.apk) [[口罩检测]](https://paddlelite-demo.bj.bcebos.com/apps/android/mask_detection_demo.apk)  [[人脸关键点]](https://paddlelite-demo.bj.bcebos.com/apps/android/face_keypoints_detection_demo.apk) [[人像分割]](https://paddlelite-demo.bj.bcebos.com/apps/android/human_segmentation_demo.apk)
- [iOS apps](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/ios_app_demo.html)
- [Linux apps](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/linux_arm_demo.html)
- [Arm](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/arm_cpu.html)
- [x86](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/x86.html)
- [OpenCL](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/opencl.html)
- [Metal](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/metal.html)
- [华为麒麟 NPU](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/huawei_kirin_npu.html)
- [华为昇腾 NPU](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/huawei_ascend_npu.html)
- [昆仑芯 XPU](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/kunlunxin_xpu.html)
- [昆仑芯 XTCL](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/kunlunxin_xtcl.html)
- [高通 QNN](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/qualcomm_qnn.html)
- [寒武纪 MLU](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/cambricon_mlu.html)
- [(瑞芯微/晶晨/恩智浦) 芯原 TIM-VX](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/verisilicon_timvx.html)
- [Android NNAPI](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/android_nnapi.html)
- [联发科 APU](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/mediatek_apu.html)
- [颖脉 NNA](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/imagination_nna.html)
- [Intel OpenVINO](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/intel_openvino.html)
- [亿智 NPU](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/eeasytech_npu.html)



## 主要特性

- 支持多平台：涵盖 Android、iOS、嵌入式 Linux 设备、Windows、macOS 和 Linux 主机
- 支持多种语言：包括 Java、Python、C++
- 轻量化和高性能：针对移动端设备的机器学习进行优化，压缩模型和二进制文件体积，高效推理，降低内存消耗

## 持续集成

| System | x86 Linux | ARM Linux | Android (GCC/Clang) | iOS |
|:-:|:-:|:-:|:-:|:-:|
| CPU(32bit) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) |
| CPU(64bit) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) |
| OpenCL | - | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - |
| Metal | - | - | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) |
| 华为麒麟 NPU | - | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - |
| 华为昇腾 NPU | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - | - |
| 昆仑芯 XPU | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - | - |
| 昆仑芯 XTCL | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - | - |
| 高通 QNN | - | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - |
| 寒武纪 MLU | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - | - | - |
| (瑞芯微/晶晨/恩智浦) 芯原 TIM-VX | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - |
| Android	NNAPI | - | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - |
| 联发科 APU | - | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - |
| 颖脉 NPU | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - | - |
| Intel OpenVINO | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - | - | - |
| 亿智 NPU | - | ![Build Status](https://img.shields.io/badge/build-passing-brightgreen.svg) | - | - |

## 架构设计

Paddle Lite 的架构设计着重考虑了对多硬件和平台的支持，并且强化了多个硬件在一个模型中混合执行的能力，多个层面的性能优化处理，以及对端侧应用的轻量化设计。

<p align="center"><img width="500" src="https://paddlelite-demo.bj.bcebos.com/devices/generic/paddle_lite_with_nnadapter.jpg"/></p>

其中，Analysis Phase 包括了 MIR(Machine IR) 相关模块，能够对原有的模型的计算图针对具体的硬件列表进行算子融合、计算裁剪 在内的多种优化。Execution Phase 只涉及到 Kernel 的执行，且可以单独部署，以支持极致的轻量级部署。

## 进一步了解 Paddle Lite

如果您想要进一步了解 Paddle Lite，下面是进一步学习和使用 Paddle Lite 的相关内容：
### 文档和示例
- 完整文档： [Paddle Lite 文档](https://www.paddlepaddle.org.cn/lite)
-  API文档：
	- [C++ API 文档](https://www.paddlepaddle.org.cn/lite/develop/api_reference/cxx_api_doc.html)
	- [Java API 文档](https://www.paddlepaddle.org.cn/lite/develop/api_reference/java_api_doc.html)
	- [Python API 文档](https://www.paddlepaddle.org.cn/lite/develop/api_reference/python_api_doc.html)
	- [CV 图像处理 API 文档](https://www.paddlepaddle.org.cn/lite/develop/api_reference/cv.html)
- Paddle Lite 工程示例： [Paddle-Lite-Demo](https://github.com/PaddlePaddle/Paddle-Lite-Demo)
### 关键技术
- 模型量化：
	- [静态离线量化](https://www.paddlepaddle.org.cn/lite/develop/user_guides/quant/quant_post_static.html)
	- [动态离线量化](https://www.paddlepaddle.org.cn/lite/develop/user_guides/quant/quant_post_dynamic.html)
- 调试分析：[调试和性能分析工具](https://www.paddlepaddle.org.cn/lite/develop/user_guides/profiler.html)
- 移动端模型训练：点击[了解一下](https://www.paddlepaddle.org.cn/lite/develop/demo_guides/cpp_train_demo.html)
- 飞桨预训练模型库：试试在 [PaddleHub](https://www.paddlepaddle.org.cn/hublist?filter=hot&value=1) 浏览和下载 Paddle 的预训练模型
- 飞桨推理 AI 硬件统一适配框架 NNAdapter：点击[了解一下](https://www.paddlepaddle.org.cn/lite/develop/develop_guides/nnadapter.html)
### FAQ
- FAQ：常见问题，可以访问 [FAQ](https://www.paddlepaddle.org.cn/lite/develop/quick_start/faq.html)、搜索 Issues、或者通过页面底部的联系方式联系我们
### 贡献代码
- 贡献代码：如果您想一起参与 Paddle Lite 的开发，贡献代码，请访问[开发者共享文档](https://www.paddlepaddle.org.cn/lite/develop/develop_guides/for-developer.html)


##  交流与反馈
* AIStudio 实训平台端测部署系列课程：https://aistudio.baidu.com/aistudio/course/introduce/22690
* 欢迎您通过 [Github Issues](https://github.com/PaddlePaddle/Paddle-Lite/issues) 来提交问题、报告与建议
* 技术交流微信群：添加 wechat id:baidupaddle或扫描下方微信二维码，添加并回复小助手“端侧”，系统自动邀请加入；技术群 QQ 群: 一群696965088（已满） ；二群，959308808；

<p align="center"><img width="200" height="200"  src="https://user-images.githubusercontent.com/63448337/162189409-6c0ef74f-82fd-48c9-9fa7-fc3473428a63.png"/>&#8194;&#8194;&#8194;&#8194;&#8194;<img width="200" height="200" margin="500" src="https://github.com/PaddlePaddle/Paddle-Lite/blob/develop/docs/images/qq-group-chat.png"/></p>
<p align="center">&#8194;&#8194;&#8194;微信公众号&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;官方技术交流QQ群 


* 如果您对我们的工作感兴趣，也欢迎[加入我们](https://github.com/PaddlePaddle/Paddle-Lite/issues/6091) ！

## 版权和许可证
Paddle Lite由 [Apache-2.0 license](LICENSE) 提供。
