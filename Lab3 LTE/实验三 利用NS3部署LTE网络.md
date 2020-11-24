# 实验三 利用NS3部署LTE网络

051720205 岳雨涵 1617101班

[TOC]



## 1 实验要求

部署的LTE网络应满足如下参数：

![image-20201124183938709](C:\Users\YUEYUHAN\AppData\Roaming\Typora\typora-user-images\image-20201124183938709.png)

在仿真完成后，应绘制无线环境地图(REM图).



## 2 实验环境

操作系统： Linux Ubuntu 

网络模拟器： NS3 

无线环境地图绘制：gnuplot



## 3 实验基础原理理解

### 3.1 LTE网络

LTE其实和平时我们熟知的GSM、CDMA、GPRS、EDGE等一样，都是一种网络制式。LTE（Long Term Evolution，长期演进）项目是3G的演进，但LTE并非人们普遍误解的4G技术，而是3G与4G技术之间的一个过渡。

### 3.2 RSRP

RSRP (Reference Signal Receiving Power，参考信号接收功率) 是LTE网络中可以代表无线信号强度的关键参数以及物理层测量需求之一，是在某个符号内承载参考信号的所有RE(资源粒子)上接收到的信号功率的平均值。通俗得理解，RSRP的功率值 代表了每个子载波的功率值，用处和规范都等同于WCDMA中的RSCP（Received Signal Code Power）接收信号码功率。

### 3.3 SINR

SINR：信号与干扰加噪声比 （Signal to Interference plus Noise Ratio）是指接收到的有用信号的强度与接收到的干扰信号（噪声和干扰）的强度的比值；可以简单的理解为“信噪比”。



### 3.4 A1/A2/A3/A4事件

为了理解命令行中`ns3::LteHelper::HandoverAlgorithm=ns3::NoOpHandoverAlgorithm`下的`ns3::NoOpHandoverAlgorithm`/`:A3RsrpHandoverAlgorithm`/`:A2A4RsrqHandoverAlgorithm`是什么意思，我查询了相关资料理解A1-A4事件。

转自：https://baike.1688.com/doc/view-d40621976.html

LTE切换时需要UE上报测量的结果（包括RSRP，RSRQ等），而上报又分为周期性上报和事件触发的上报。周期性上报由基站配置，UE直接上报测量的结果。事件触发的上报又分为同频系统的事件和不同系统间的事件：同频切换报告事件包括：

1.事件A1，服务小区好于绝对门限；这个事件可以用来关闭某些小区间的测量。

2.事件A2，服务小区差于绝对门限；这个事件可以用来开启某些小区间的测量，因为这个事件发生后可能发生切换等操作。

3.事件A3，邻居小区好于服务小区；这个事件发生可以用来决定UE是否切换到邻居小区。

4.事件A4，邻居小区好于绝对门限；

5.事件A5，服务小区差于一个绝对门限并且邻居小区好于一个绝对门限；这个事件也可以用来支持切换





## 4 实验过程

### 4.1  官方文档

官方文档 ： https://www.nsnam.org/docs/models/html/lte-user.html

根据文档，我学习了LTE的概述、如何配置LTE模型参数，如何绘制REM图，以及最后配置好参数模拟出了本实验要求中的LTE网络.具体的实验过程如下。



### 4.2  实验过程记录

#### 4.2.1 全局变量

路径` src/lte/examples/ `包含一些示例仿真程序，这些例子表明如何仿真不同的LTE场景，其中，示例程序  `src/lte/examples/lena-dual-stripe.cc` 就可以模拟出我们实验要求中的LTE网络。该示例程序的特点是有很多可配置的参数，可以通过修改相关的全局变量来自定义。可以使用`./waf --run lena-dual-stripe --command-template="%s --PrintGlobals"	`命令来获取所有这些全局变量的列表.下图为输入命令后得到的部分全局变量.

 <img src="C:\Users\YUEYUHAN\AppData\Roaming\Typora\typora-user-images\image-20201124195832892.png" alt="image-20201124195832892" style="zoom:67%;" />



![image-20201124195925467](C:\Users\YUEYUHAN\AppData\Roaming\Typora\typora-user-images\image-20201124195925467.png)



#### 4.2.2 配置LTE参数

然后我们必须配置程序的参数来满足我们的仿真需求。下图表格表示我们如何配置参数，使满足实验要求。

<img src="C:\Users\YUEYUHAN\AppData\Roaming\Typora\typora-user-images\image-20201124200134924.png" alt="image-20201124200134924" style="zoom:50%;" />

需要注意的是要进行配置主要是`nMacroAEnbSites`,`nMacroEnbSiteX`,`interSiteDistance`, `macreoEnbTxPowerDbm`四个参数，其他参数都可使用default值就可满足实验要求。



#### 4.2.3 传递配置参数到仿真中

当开始每个单独的仿真时，我们通过附加参数及其值到  waf 调用中来实现. 因此，在本次实验中，我们分别进行如下命令行的运行：

```c
$ ./waf --run="lena-dual-stripe
  --simTime=50 --nBlocks=0 --nMacroEnbSites=7 --nMacroEnbSitesX=2
  --epc=1 --useUdp=0 --outdoorUeMinSpeed=16.6667 --outdoorUeMaxSpeed=16.6667
  --ns3::LteHelper::HandoverAlgorithm=ns3::NoOpHandoverAlgorithm
  --ns3::RadioBearerStatsCalculator::DlRlcOutputFilename=no-op-DlRlcStats.txt
  --ns3::RadioBearerStatsCalculator::UlRlcOutputFilename=no-op-UlRlcStats.txt
  --ns3::PhyStatsCalculator::DlRsrpSinrFilename=no-op-DlRsrpSinrStats.txt
  --ns3::PhyStatsCalculator::UlSinrFilename=no-op-UlSinrStats.txt
  --RngRun=1" > no-op.txt

$ ./waf --run="lena-dual-stripe
  --simTime=50 --nBlocks=0 --nMacroEnbSites=7 --nMacroEnbSitesX=2
  --epc=1 --useUdp=0 --outdoorUeMinSpeed=16.6667 --outdoorUeMaxSpeed=16.6667
  --ns3::LteHelper::HandoverAlgorithm=ns3::A3RsrpHandoverAlgorithm
  --ns3::RadioBearerStatsCalculator::DlRlcOutputFilename=a3-rsrp-DlRlcStats.txt
  --ns3::RadioBearerStatsCalculator::UlRlcOutputFilename=a3-rsrp-UlRlcStats.txt
  --ns3::PhyStatsCalculator::DlRsrpSinrFilename=a3-rsrp-DlRsrpSinrStats.txt
  --ns3::PhyStatsCalculator::UlSinrFilename=a3-rsrp-UlSinrStats.txt
  --RngRun=1" > a3-rsrp.txt

$ ./waf --run="lena-dual-stripe
  --simTime=50 --nBlocks=0 --nMacroEnbSites=7 --nMacroEnbSitesX=2
  --epc=1 --useUdp=0 --outdoorUeMinSpeed=16.6667 --outdoorUeMaxSpeed=16.6667
  --ns3::LteHelper::HandoverAlgorithm=ns3::A2A4RsrqHandoverAlgorithm
  --ns3::RadioBearerStatsCalculator::DlRlcOutputFilename=a2-a4-rsrq-DlRlcStats.txt
  --ns3::RadioBearerStatsCalculator::UlRlcOutputFilename=a2-a4-rsrq-UlRlcStats.txt
  --ns3::PhyStatsCalculator::DlRsrpSinrFilename=a2-a4-rsrq-DlRsrpSinrStats.txt
  --ns3::PhyStatsCalculator::UlSinrFilename=a2-a4-rsrq-UlSinrStats.txt
  --RngRun=1" > a2-a4-rsrq.txt
```

编译时间较长，大约为40min左右。耐心等待后，可以看到`ns-3.25`文件夹下新生成的文件`no-op-DlRlcStats.txt`和`no-op-UlRlcStats.txt`。另外两份文件由于实验四计算RSRP等才需要用到，在这里不做提及。



需要注意的是，由于要绘制出REM图，所以应该在命令行末尾加上生成仿真REM的参数` --generateRem=1`.



#### 4.2.4 绘制REM图

首先在`./waf`同级文件夹下编写绘制REM的gnuplot脚本，命名为`my_plot_script`.

```c
set view map;
set xlabel "X"
set ylabel "Y"
set cblabel "SINR (dB)"
unset key
plot "lena-dual-stripe.rem" using ($1):($2):(10*log10($4)) with image
```

lena-dual-stripe 示例程序也会生成` gnuplot-compatible `输出文件，包含用户与基站节点以及建筑物的位置信息，分别为文件 `ues.txt`、 `enbs.txt` 和` buildings.txt `。使用 gnuplot 时，这些可以很容易包含在内。

最后运行 `gnuplot -p enbs.txt ues.txt buildings.txt my_plot_scrip`t得到绘制出的REM图.

<img src="C:\Users\YUEYUHAN\AppData\Roaming\Typora\typora-user-images\image-20201124201647151.png" alt="image-20201124201647151" style="zoom:67%;" />



## 5 部分源码

完整源码见打包文件夹。

```c
/* 参数配置PART */

//site数量
static ns3::GlobalValue g_nMacroEnbSites ("nMacroEnbSites",
                                          "How many macro sites there are",
                                          ns3::UintegerValue (7),
                                          ns3::MakeUintegerChecker<uint32_t> ());
//正六边形2-3-2 排列
static ns3::GlobalValue g_nMacroEnbSitesX ("nMacroEnbSitesX",
                                           "(minimum) number of sites along the X-axis of the hex grid",
                                           ns3::UintegerValue (2),
                                           ns3::MakeUintegerChecker<uint32_t> ());
//基站间距
static ns3::GlobalValue g_interSiteDistance ("interSiteDistance",
                                             "min distance between two nearby macro cell sites",
                                             ns3::DoubleValue (500),
                                             ns3::MakeDoubleChecker<double> ());
static ns3::GlobalValue g_areaMarginFactor ("areaMarginFactor",
                                            "how much the UE area extends outside the macrocell grid, "
                                            "expressed as fraction of the interSiteDistance",
                                            ns3::DoubleValue (0.5),
                                            ns3::MakeDoubleChecker<double> ());
static ns3::GlobalValue g_macroUeDensity ("macroUeDensity",
                                          "How many macrocell UEs there are per square meter",
                                          ns3::DoubleValue (0.00002),
                                          ns3::MakeDoubleChecker<double> ());
static ns3::GlobalValue g_homeEnbDeploymentRatio ("homeEnbDeploymentRatio",
                                                  "The HeNB deployment ratio as per 3GPP R4-092042",
                                                  ns3::DoubleValue (0.2),
                                                  ns3::MakeDoubleChecker<double> ());
static ns3::GlobalValue g_homeEnbActivationRatio ("homeEnbActivationRatio",
                                                  "The HeNB activation ratio as per 3GPP R4-092042",
                                                  ns3::DoubleValue (0.5),
                                                  ns3::MakeDoubleChecker<double> ());
static ns3::GlobalValue g_homeUesHomeEnbRatio ("homeUesHomeEnbRatio",
                                               "How many (on average) home UEs per HeNB there are in the simulation",
                                               ns3::DoubleValue (1.0),
                                               ns3::MakeDoubleChecker<double> ());
//基站发射功率
static ns3::GlobalValue g_macroEnbTxPowerDbm ("macroEnbTxPowerDbm",
                                         "TX power [dBm] used by macro eNBs",
                                              ns3::DoubleValue (46.0),
```



```c
/* REM文件生成PART */ 

  Ptr<RadioEnvironmentMapHelper> remHelper;
  if (generateRem)
    {
      PrintGnuplottableBuildingListToFile ("buildings.txt");
      PrintGnuplottableEnbListToFile ("enbs.txt");
      PrintGnuplottableUeListToFile ("ues.txt");

      remHelper = CreateObject<RadioEnvironmentMapHelper> ();
      remHelper->SetAttribute ("ChannelPath", StringValue ("/ChannelList/0"));
      remHelper->SetAttribute ("OutputFile", StringValue ("lena-dual-stripe.rem"));
      remHelper->SetAttribute ("XMin", DoubleValue (macroUeBox.xMin));
      remHelper->SetAttribute ("XMax", DoubleValue (macroUeBox.xMax));
      remHelper->SetAttribute ("YMin", DoubleValue (macroUeBox.yMin));
      remHelper->SetAttribute ("YMax", DoubleValue (macroUeBox.yMax));
      remHelper->SetAttribute ("Z", DoubleValue (1.5));//用户高度

      if (remRbId >= 0)
        {
          remHelper->SetAttribute ("UseDataChannel", BooleanValue (true));
          remHelper->SetAttribute ("RbId", IntegerValue (remRbId));
        }

      remHelper->Install ();
      // simulation will stop right after the REM has been generated
    }
  else
    {
      Simulator::Stop (Seconds (simTime));
    }
```

