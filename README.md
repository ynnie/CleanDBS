# CleanDBS

[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)


A deep brain stimulation artifact processing method based on irregular sampling

Using the irregular sampling method, the sampling points in the artifact interval are interpolated according to the points outside the artifact interval, and for the other sampling points are sampled according to the normal sampling interval, and finally the resampled signal is obtained.

## Table of Contents
- [Install](#Install)
- [Usage](#Usage)
- [Demo](#Demo)

## Install
Add **cleandbs.h** and **cleandbs.cpp** from the lib file to your project file and make the following declaration in the beginning.

```C++
#include "cleandbs.h"
```

## Usage

### Initialize the cleanDBS object

```C++
CleanDBS clean(fs, fsOut, preArtifact, postArtifact, thr);
```
Input：fs，fsOut，preArtifact，posArtifact， thr

- fs: sampling frequency (need high frequency, more than 20k)
- fsOut: output sampling rate, default 300Hz
- preArtifact: skip 'a' second ahead of each artifact peak, default 0.0002s
- posArtifact: skip 'p' second after each artifact peak, default 0.005s
- thr: threshold for artifact peak detection, default 3000uV

Example code：

```C++
// Default parameter settings
double fsOut = 300; // Output sampling rate
double preArtifact = 0.0002;
double postArtifact = 0.005;
double thr = 3000;
```

```C++
// Initializing objects
CleanDBS clean(fs, fsOut, preArtifact, postArtifact, thr);
```

### Real-time data processing
```C++
clean.recieve(sample, &sampleOut)
```
The input point is **sample**, the output point is **sampleOut**, the return value is **bool**.
If there is output, the return value is **True** and modify the content of sampleOut.


Example code：
```C++
//Process the current data in real time, determine whether there is output, if there is output, return True, if not, False.
if(clean.recieve(sample, &sampleOut)){
                /* Processing of output data */
            }
```


## A running Demo in Qt 

Test Version:Qt5.14.1 (Clang 10.0 (Apple), 64 bit)

We give a Demo of the code running in the **Demo folder**. 


### Demo files include
- main: main function file
- cleandbs: remove stimulus artifacts
- nspfile: read and write txt files
- progressbar: show processing progress bar

### Use

You can compile the project file in Qt, add the files to the executable that need to remove the artifacts, and run the following code to get the file **fileOut** with artifacts removed.

```shell
$ ./cleandbs [fileIn][fileOut][option][value]
```
where **fileIn** is the name of the file to be processed and **fileOut** is the name of the output file.

It should be noted here that both the input and output files are in .txt format, and for the first line must be in the following format.

**Sampling frequency Number of channels Number of data points 0**

```
44000    2    2200000    0
```

option中可以修改preArtifact、posArtifact、thr
```
Options:                                                            *
*   -f, output sampling rate, default 300Hz                         *
*   -a, skip 'a' second ahead of each artifact peak, default 0.0002 *
*   -p, skip 'p' second after each artifact peak, default 0.005     *
*   -t, threshold for artifact peak detection, default 3000         *
```     

When you see:
```
Finished!
```
Congratulations on having generated a signal without stimulus artifacts!


### Test

With the test data **in.txt** placed in the **demo folder**, try using the following code and obtain **out.txt** file is the result of removing the artifacts
```
$ ./cleandbs in.txt out.txt -f 300 -a 0.0002 -p 0.005 -t 3000
```
Default value if parameter is not modified.

Note.

The **in.txt** file is the STN nucleus LFP data containing the stimulus signal at 44k sampling frequency with a length of 50s, 1-channel stimulus frequency of 130hz and 2-channel stimulus frequency of 90hz.




# CleanDBS

[![standard-readme compliant](https://img.shields.io/badge/readme%20style-standard-brightgreen.svg?style=flat-square)](https://github.com/RichardLitt/standard-readme)


一种基于不规则采样的深部脑刺激伪迹处理方式

采用不规则采样的方法，处于伪迹区间的采样点根据伪迹区间外的点进行插值处理，对于其他采样点按照正常采样间隔采样，最后得到重采样的信号。

## 内容列表
- [安装](#安装)
- [使用说明](#使用说明)
- [示例](#示例)

## 安装
将lib文件中的**cleandbs.h**和**cleandbs.cpp**加入到你的工程文件中，并在调用的函数中作如下声明：

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
输入点为sample，输出点为sampleOut，返回值为bool值，若有输出则为True并且修改sampleOut的内容。


示意：
```C++
//实时处理当前数据，判断是否有输出，有输出则返回True，没有则为False
if(clean.recieve(sample, &sampleOut)){
                /* 对输出数据进行处理 */
            }
```


## 一个在Qt中运行的示例

测试版本:Qt5.14.1 (Clang 10.0 (Apple), 64 bit)

我们在**Demo文件夹**中给出了代码运行的示例。

### Demo文件包括
- main：主函数文件
- cleandbs：去除刺激伪迹
- nspfile：读写txt文件
- progressbar：显示处理进度条

### 使用

你可以在Qt编译该工程文件，在可执行文件中加入需要去除伪迹的文件，并运行如下代码就可以得到去除伪迹后的文件**fileOut**：

```shell
$ ./cleandbs [fileIn][fileOut][option][value]
```
其中，**fileIn**为待处理文件名，**fileOut**为输出文件名。

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

在demo文件夹中放入了测试数据in.txt，可以尝试使用下面这句代码得到去除过伪迹的结果**out.txt文件**
```
$ ./cleandbs in.txt out.txt -f 300 -a 0.0002 -p 0.005 -t 3000
```
参数若不加修改则为默认值

Note：

**in.txt**文件是44k采样频率下，含有刺激信号的STN核团LFP数据，长度为50s，1通道刺激频率为130hz，2通道刺激频率为90hz。
