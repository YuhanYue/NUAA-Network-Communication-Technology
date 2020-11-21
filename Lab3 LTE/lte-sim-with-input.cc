#include"ns3/core-module.h"
#include"ns3/network-module.h"
#include"ns3/mobility-module.h"
#include"ns3/lte-module.h"
#include"ns3/buildings-helper.h"
#include"ns3/config-store.h"
/*
 * create an LTE-only simulation without EPC
 */
 
using namespace ns3;
NS_LOG_COMPONENT_DEFINE("LteFirstExample");
int main(int argc,char*argv[]){
 
    //加入命令行以方便修改参数
CommandLine cmd;
cmd.Parse(argc,argv);
ConfigStore inputConfig;
inputConfig.ConfigureDefaults();//注意先Load再save
cmd.Parse(argc,argv);
NS_LOG_INFO("Create LTE NET");
LogComponentEnable("LteFirstExample",LOG_LEVEL_INFO);
//create a ltehelper object  epc object
Ptr<LteHelper> lte=CreateObject<LteHelper>();
/*lte->SetFadingModel("ns3::TraceFadingLossModel");
lte->SetFadingModelAttribute("TraceFilename",StringValue("src/lte/model/fading-traces/fading_trace_EPA_3kmph.fad"));
lte->SetFadingModelAttribute("TraceLength",TimeValue(Seconds(10.0)));
lte->SetFadingModelAttribute("SamplesNum",UintegerValue(10000));
lte->SetFadingModelAttribute("WindowSize",TimeValue(Seconds(0.5)));
lte->SetFadingModelAttribute("RbNum",UintegerValue(100));*/
//为UE eNB创建节点
NodeContainer enbNodes;
enbNodes.Create(1);
NodeContainer ueNodes;
ueNodes.Create(2);
//为节点配置移动模型
MobilityHelper mobility;
mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
mobility.Install(enbNodes);//初始位置(0,0,0)
//mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
//mobility.Install(ueNodes);
mobility.SetPositionAllocator ("ns3::RandomDiscPositionAllocator", 
"X", StringValue ("100.0"), 
"Y", StringValue ("100.0"), 
"Rho", StringValue ("ns3::UniformRandomVariable[Min=0|Max=30]")); 
mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel", 
"Mode", StringValue ("Time"), 
"Time", StringValue ("2s"), 
"Speed", StringValue ("ns3::ConstantRandomVariable[Constant=1.0]"), 
"Bounds", StringValue ("0|200|0|200")); 
mobility.Install(ueNodes); 
BuildingsHelper::Install (ueNodes); 
// Default scheduler is PF, uncomment to use RR 
//lteHelper->SetSchedulerType ("ns3::RrFfMacScheduler"); 
//装载LTE协议栈
NetDeviceContainer enbdevice;
enbdevice=lte->InstallEnbDevice(enbNodes);
NetDeviceContainer uedevice;
uedevice=lte->InstallUeDevice(ueNodes);
//关联UE 和基站eNB，根据eNB配置来配置每一个UE并创建UE-ENB之间的RRC连接
lte->Attach(uedevice,enbdevice.Get(0));
 
//激活EPS承载包括UE-ENB之间的无线承载
enum EpsBearer::Qci q=EpsBearer::GBR_CONV_VOICE;
EpsBearer bearer(q);
lte->ActivateDataRadioBearer(uedevice,bearer);
// configure all the simulation scenario here...
//输出PHY MAC RLC PDCP层KPI信息
lte->EnablePhyTraces ();
lte->EnableMacTraces ();
lte->EnableRlcTraces ();
lte->EnablePdcpTraces ();
//设置仿真参数
Simulator::Stop(Seconds(0.005));
Simulator::Run();
Simulator::Destroy();
return 0;
 
}