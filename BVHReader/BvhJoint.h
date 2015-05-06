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
	enum channelType {Xpos,Ypos,Zpos,Xrot,Yrot,Zrot};//���ɶ����� 
	void SetChannelNum(int channel);
	const int GetChannelNum();
	void SetJointName(string);
	string GetJointName();
	void SetOffset(const double off[]);
	double GetOffset();         //���offsetƫ����
	void InsertOneMotion(const BvhMotionData &md);
	void GetOffset(double *off);         //���offsetƫ����	 
	vector<channelType> channels;
	BvhJoint *m_parent;        //���ڵ�
	vector<BvhJoint*> children;    //�ӽڵ�
	vector<BvhMotionData> m_motions;  //������Ϣ
private:
	int m_channelNum;
	string m_jointName;//�ڵ�����
	double m_offset[3];   //ƫ����     
};