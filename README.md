# CleanDBS

[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)


一种基于不规则采样的深部脑刺激伪迹处理方式

采用不规则采样的方法，处于伪迹区间的采样点根据伪迹区间外的点进行插值处理，对于其他采样点按照正常采样间隔采样，最后得到重采样的信号。

## 内容列表
- [安装](#安装)
- [使用说明](#使用说明)
- [示例](#示例)

## 安装

将cleandbs.h和cleandbs.cpp加入到你的工程文件中，并在调用的函数中作如下声明

```C++
#include "cleandbs.h"
```

## 使用说明

### 初始化cleanDBS对象

```C++
CleanDBS clean(fs, fsOut, preArtifact, postArtifact, thr);
```
输入：fs，fsOut，preArtifact，posArtifact， thr

- fs：包含伪迹数据的采样频率（需要高频率，大于20k）
- fsOut：去除伪迹数据后的采样频率，默认为300hz
- preArtifact：刺激伪迹尖峰前的长度，默认为0.0002s
- posArtifact：刺激伪迹尖峰后的长度，默认为0.005s
- thr：检测尖峰的阈值，默认为3000uV

示意：

```C++
// 默认参数设置
double fsOut = 300; // Output sampling rate
double preArtifact = 0.0002;
double postArtifact = 0.005;
double thr = 3000;
```

```C++
// 初始化对象
CleanDBS clean(fs, fsOut, preArtifact, postArtifact, thr);
```

### 实时处理数据
```C++
clean.recieve(sample, &sampleOut)
```
输入点为sample，输出点为sampleOut，返回值为bool值，若有输出则为True并且修改sampleOut的内容


示意：
```C++
//实时处理当前数据，判断是否有输出，有输出则返回True，没有则为False
if(clean.recieve(sample, &sampleOut)){
                /* 对输出数据进行处理*/
            }
```


## 一个在QT中的运行示例

我们在Demo文件夹中给出了代码运行的示例。在Qt 5.14.1 (Clang 10.0 (Apple), 64 bit) 中运行并测试。

你可以在qt编译该工程文件，在可执行文件中加入需要去除伪迹的文件，并运行如下代码

```shell
$ ./cleandbs [fileIn][fileOut][option][value]
```
就可以得到去除伪迹后的文件

其中，**fileIn**为待处理文件名，**fileOut**为输出文件名

这里需要说明的是，输入文件和输出文件都是.txt格式，并且对第一行必须为如下格式：

**采样频率 通道数 数据点数 0**
```
44000    2    2200000    0
```

option中可以修改preArtifact、posArtifact、thr
```
Options:                                
     *   -f, 输出采样频率                 *
     *   -a, 刺激伪迹尖峰前的长度          *
     *   -p, 刺激伪迹尖峰后的长度          *
     *   -t, 检测尖峰的阈值               *
```     

当你看见
```
Finished!
```
祝贺你已经生成了没有刺激伪迹的信号！


### 测试

在demo文件夹中放入了测试数据in.txt，可以尝试下面这句代码使用
```
$ ./cleandbs in.txt out.txt -f 300 -a 0.0002 -p 0.005 -t 3000
```
得到的的out.txt文件就是去除过伪迹的结果，参数若不加修改则为默认值
