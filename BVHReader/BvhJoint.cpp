#include "BvhJoint.h"


BvhJoint::BvhJoint(void)
{
	m_jointName = "";   
	m_channelNum = 0;
	m_parent = NULL; 
	/*for(int i=0;i<3;i++)
	{
		m_offset[i] = 0.0f;
	}*/
	m_offset[0] = 0.0f;
	m_offset[1] = 0.0f;
	m_offset[2] = 0.0f; 
}


BvhJoint::~BvhJoint(void)
{
}

void BvhJoint::SetChannelNum(int channel)
{
	this->m_channelNum = channel;
}
const int BvhJoint::GetChannelNum()
{
	return this->m_channelNum;
}
void BvhJoint::SetJointName(string name)
{
	this->m_jointName = name;
}
string BvhJoint::GetJointName()
{
	return this->m_jointName;
}
void BvhJoint::SetOffset(const double off[])
{
	this->m_offset[0] = off[0];
	this->m_offset[1] = off[1];
	this->m_offset[2] = off[2];
}
double BvhJoint::GetOffset()
{
	return *m_offset;
}

void BvhJoint::InsertOneMotion(const BvhMotionData &md)
{
	BvhMotionData tMd;
	tMd.CopyMotion(md);
	this->m_motions.push_back(tMd);
}
void BvhJoint::GetOffset(double *off)
{
	off[0] = this->m_offset[0];
	off[1] = this->m_offset[1];
	off[2] = this->m_offset[2];
}    