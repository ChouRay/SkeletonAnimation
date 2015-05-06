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
	int GetFrames(void);         //���֡��
	void DealCoordination(int i);     //��������
	const int GetChannelsNum(void);      //���channel����
	vector<BvhJoint*> joints;  //����ڵ�
	bool print(void);        //��ӡ�����Ϣ���ļ���	 
protected:
	void clear(); 
	bool ClearFramework(BvhJoint *p);	
	void SubFree(BvhJoint *p);
	void DealCoordination(double M[][4],int n,BvhJoint *joint);  //��������
	void Translate(double x,double y,double z,double M[][4]);     //ƽ�Ʊ任	
private:
	JOINTS m_key;
	int m_jointNum;
	int m_offNum;
	int m_channel;//��¼Channel�ĸ���
	int m_frames; //֡��
	double m_frametime;
	double m_offset[3];  //ƫ����
	BvhJoint *m_root;  //ͷ���
	BvhJoint *m_curr;
	BvhJoint *temp;

	BvhMotionData m_tMotion;
	vector<node> ptemp;
	bool isadd;
	int m_channelnum;    //���ɶȸ���
	int tempnum;
public:
	int GetJointSize(void);
};

