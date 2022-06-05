# Paddle Lite

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
01. cd test
02. mkdir build
03. cd build
04. cmake -DWIN64=1 -G "Visual Studio 15 2017 Win64" ..
05. set curr_dir=%cd%
06. "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\Visual Studio 2017\Visual Studio Tools\VC\x64 Native Tools Command Prompt for VS 2017.lnk"
07. cd %curr_dir% 
08. msbuild /p:Configuration=Release test_paddlelite_opencl.vcxproj
09. cd ..
10. build\Release\test_paddlelite_opencl.exe
```

## 如何在Linux下编译运行C++推断程序

```
01. sudo apt install libopencv-dev
02. sudo docker login
03. sudo docker pull redis:latest 
04. sudo docker run -itd --name transformer -p 6379:6379 redis # default ip address: 127.0.0.1
    OR service on host address used by outside network
    sudo docker run -itd --name transformer -p `hostname -I | cut -d ' ' -f 1`:6379:6379 redis
05. sudo apt install redis-tools
06. redis-cli
127.0.0.1:6379> ping
PONG
127.0.0.1:6379> exit
07. sudo pip3 install --upgrade redis
08. cd test
09. wget http://paddle-inference-dist.bj.bcebos.com/mobilenet_v1.tar.gz
10. tar xvzf mobilenet_v1.tar.gz
11. paddle_lite_opt --model_dir mobilenet_v1 --optimize_out_type naive_buffer --optimize_out mobilenet_v1_opencl --valid_targets opencl
12. mkdir build
14. cd build
15. cmake ..
16. make
17. cd ..
18. python3 test_redis.py
19. ./build/test_redis
20. ./build/test_paddlelite_opencl
21. python3 test_paddlelite_opencl.py
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
$ wget https://github.com/lutzroeder/netron/releases/download/v5.8.4/Netron-Setup-5.8.4.exe
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


[简体中文](README.md) | English

[![Build Status](https://travis-ci.org/PaddlePaddle/Paddle-Lite.svg?branch=develop&longCache=true&style=flat-square)](https://travis-ci.org/PaddlePaddle/Paddle-Lite)  [![Documentation Status](https://img.shields.io/badge/中文文档-最新-brightgreen.svg)](https://paddle-lite.readthedocs.io/zh/develop/)  [![Release](https://img.shields.io/github/release/PaddlePaddle/Paddle-Lite.svg)](https://github.com/PaddlePaddle/Paddle-Lite/releases)  [![License](https://img.shields.io/badge/license-Apache%202-blue.svg)](LICENSE)


Paddle Lite is an updated version of Paddle-Mobile, an open-open source deep learning framework designed to make it easy to perform inference on mobile, embeded, and IoT devices. It is compatible with PaddlePaddle and pre-trained models from other sources.

For tutorials, please see [PaddleLite Document](https://paddle-lite.readthedocs.io/zh/develop/).

## Key Features

- Multiple platform support, covering Android and iOS devices, embedded Linux, Windows, macOS and Linux computer.
- Diverse language support, which includes Java, C++, and Python.
- High performance and light weight: optimized for on-device machine learning, reduced model and binary size, efficient inference and reduced memory usage.


## Architecture

Paddle Lite is designed to support a wide range of hardwares and devices, and it enables mixed execution of a single model on multiple devices, optimization on various phases, and leight-weighted applications on devices.

![img](https://paddlelite-demo.bj.bcebos.com/devices/generic/paddle_lite_with_nnadapter.png)

As is shown in the figure above, analysis phase includes Machine IR module, and it enables optimizations like Op fusion and redundant computation pruning. Besides, excecution phase only involves Kernal execution, so it can be deployed on its own to ensure maximum light-weighted deployment.

## Key Info about the Update

The earlier Paddle-Mobile was designed to be compatible with PaddlePaddle and multiple hardwares, including ARM CPU, Mali GPU, Adreno GPU, FPGA, ARM-Linux and Apple's GPU Metal. Within Baidu, inc, many product lines have been using Paddle-Mobile.

As an update of Paddle-Mobile, Paddle Lite has incorporated many older capabilities into the [new architecture](https://github.com/PaddlePaddle/Paddle-Lite/tree/develop/lite).

## Special Thanks

Paddle Lite has referenced the following open-source projects:

- [ARM compute library](https://github.com/ARM-software/ComputeLibrary)
- [Anakin](https://github.com/PaddlePaddle/Anakin). The optimizations under Anakin has been incorporated into Paddle Lite, and so there will not be any future updates of Anakin. As another high-performance inference project under PaddlePaddle, Anakin has been forward-looking and helpful to the making of Paddle Lite.  



## Feedback and Community Support

- Questions, reports, and suggestions are welcome through Github Issues!
- Forum: Opinions and questions are welcome at our [PaddlePaddle Forum](https://ai.baidu.com/forum/topic/list/168)！
- WeChat Official Account: PaddlePaddle
- QQ Group Chat: 696965088
<p align="center"><img width="200" height="200"  src="https://user-images.githubusercontent.com/45189361/64117959-1969de80-cdc9-11e9-84f7-e1c2849a004c.jpeg"/>&#8194;&#8194;&#8194;&#8194;&#8194;<img width="200" height="200" margin="500" src="https://user-images.githubusercontent.com/45189361/64117844-cb54db00-cdc8-11e9-8c08-24bbe594608e.jpeg"/></p>
<p align="center">&#8194; WeChat Official Account&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;&#8194;QQ Group Chat&#8194;&#8194;&#8194;&#8194;&#8194;</p>
