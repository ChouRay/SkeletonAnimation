#pragma once
#include <vector>
#include <string>
#include "BvhMotionData.h"
using namespace std;
class BvhJoint
{
public:
	BvhJoint(void);
	~BvhJoint(void);
	enum channelType {Xpos,Ypos,Zpos,Xrot,Yrot,Zrot};//自由度类型 
	void SetChannelNum(int channel);
	const int GetChannelNum();
	void SetJointName(string);
	string GetJointName();
	void SetOffset(const double off[]);
	double GetOffset();         //获得offset偏移量
	void InsertOneMotion(const BvhMotionData &md);
	void GetOffset(double *off);         //获得offset偏移量	 
	vector<channelType> channels;
	BvhJoint *m_parent;        //父节点
	vector<BvhJoint*> children;    //子节点
	vector<BvhMotionData> m_motions;  //数据信息
private:
	int m_channelNum;
	string m_jointName;//节点名称
	double m_offset[3];   //偏移量     
};