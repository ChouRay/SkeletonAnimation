#pragma once
#include <iostream>
#include <string>
#include <vector>

#include "BvhJoint.h"
#include "BvhMotionData.h"
using namespace std;

struct node{
	double x;
	double y;
	double z;
	double w;
};

enum JOINTS{HIERACHY,ROOT,JOINT,OFFSET,CHANNELS,END,SITE,MOTION,FRAMES,FRAME,TIME,NONE,MOTIONDATA};
class BvhRead
{
public:
	BvhRead(void);
	~BvhRead(void);
	bool ReadBvh(string filename);
	void BuildSkeleton(string line);
	int GetFrames(void);         //获得帧数
	void DealCoordination(int i);     //计算坐标
	const int GetChannelsNum(void);      //获得channel个数
	vector<BvhJoint*> joints;  //保存节点
	bool print(void);        //打印相关信息到文件中	 
protected:
	void clear(); 
	bool ClearFramework(BvhJoint *p);	
	void SubFree(BvhJoint *p);
	void DealCoordination(double M[][4],int n,BvhJoint *joint);  //计算坐标
	void Translate(double x,double y,double z,double M[][4]);     //平移变换	
private:
	JOINTS m_key;
	int m_jointNum;
	int m_offNum;
	int m_channel;//记录Channel的个数
	int m_frames; //帧数
	double m_frametime;
	double m_offset[3];  //偏移量
	BvhJoint *m_root;  //头结点
	BvhJoint *m_curr;
	BvhJoint *temp;

	BvhMotionData m_tMotion;
	vector<node> ptemp;
	bool isadd;
	int m_channelnum;    //自由度个数
	int tempnum;
public:
	int GetJointSize(void);
};

