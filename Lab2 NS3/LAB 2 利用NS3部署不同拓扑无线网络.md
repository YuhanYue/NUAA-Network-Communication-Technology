# LAB 2 利用NS3部署不同拓扑无线网络

051720205 岳雨涵 1617101班

[TOC]

## 1 实验要求

利用`NS2`/`NS3`部署一个星型无线网络（一个AP，不少于5个接入点）、一个多跳无线网络（不少于6个网络节点）。并测量两种网络拓扑下的网络链路吞吐量，用图表表示。



## 2 实验环境

操作系统： Linux Ubuntu 

网络模拟器： NS3 

代码可视化：NetAnim(基于Qt 4) 

吞吐量计算： Wireshark/ thourought.awk脚本

吞吐量图标表示：Wireshark/ gnuplot



## 3 实验原理

### 3.1 星型网络

* 星型网络是有两种网络设备构成：中心节点 (此实验中即AP) 和终端节点。中心节点是整个星型网络的枢纽，所有终端节点通过无线或者有线的形式连接到中心节点，与中心节点进行信息交互。终端节点之间不能直接进行信息交互，只能通过中心节点进行信息转发，从而达到终端节点之间相互通信的作用。拓扑图如下：

<img src="/Users/yuhan/Library/Application Support/typora-user-images/Screen Shot 2020-11-09 at 10.10.45 PM.png" alt="Screen Shot 2020-11-09 at 10.10.45 PM" style="zoom:33%;" />

* ns3中实现星型网络: 使用`PointToPointStarHelper Class Reference`

  源码参见： https://www.nsnam.org/doxygen/classns3_1_1_point_to_point_star_helper.html

  

### 3.2 多跳网络

* 多跳网络的核心是让网络中的每个节点都发送和接收信号。网络中的大量终端设备连接成网状结构，每个节点都具有自动路由功能，每个节点只和临近节点通信。拓扑图如下：

  <img src="/Users/yuhan/Library/Application Support/typora-user-images/image-20201109233807110.png" alt="image-20201109233807110" style="zoom:33%;" />



* 需要初始化信道/wifi物理层.

* reference:  http://blog.chinaunix.net/uid-23982854-id-4416998.html

  

## 4 实验过程

### 4.1 环境搭建

##### 4.1.1 安装`ns3`

巨良心的教程：https://blog.51cto.com/joedlut/1825512

安装过程：

1. 先写脚本来安装所有ns3.25的依赖并执行该脚本；
2. 下载ns3 (wget或官网都可) 
3. 编译安装ns3 ,执行 `./build.py --enable-tests --enable-examples `  
4. 执行 `./waf --run hello-simulator `测试是否安装成功

之后所有的文件全部放入scratch文件夹中使用./waf运行，因为scratch文件夹是ns3的默认脚本存放地址.



##### 4.1.2 安装`NetAnim`

1. 为了使代码可视化，需要安装`NetAnim`.由于它基于qt5，则先执行

```
sudo apt-get install mercurial
sudo apt-get install qt5-default
```

​	来安装Qt5和Mecurial.

2. 安装完成后使用`qmake`进行编译.

   ```
   cd ns-allinone-3.25/netanim-3.108
   qmake ./NetAnim.pro
   ```



##### 4.1.3 `gnuplot`和`gwak`的安装(Optional)

为了求吞吐量并可视化，先安装上述两个包.

```C
sudo apt-get install gnuplot//画图
sudo apt-get install gwak
```

gnuplot是根据`ns3`运行时产生的`trace`文件生成图标的软件，可以通过`trace`文件中的数据，使用gwak和throughout.awk（求吞吐量的awk脚本）来求出吞吐量，并使用gnuplot绘图.



##### 4.1.4  安装`wireshark`

使用`gwak`+`gnuplot`计算吞吐量并绘图的方式太麻烦，直接安装wireshark，在statistic选项卡中即可查看.

安装wireshark直接 `sudo apt-get install wireshark`即可.



### 4.2 星型网络





### 4.3 多跳网络



## 5 源码













- 