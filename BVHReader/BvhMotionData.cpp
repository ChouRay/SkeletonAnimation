#include "BvhMotionData.h"
#include <math.h>

#define PI 3.14159265358979323846 

BvhMotionData::BvhMotionData(void)
{
	int i,j;
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			if(i == j)
			{
				m_rotationX[i][j] = 1;
				m_rotationY[i][j] = 1;
				m_rotationZ[i][j] = 1;
			}
			else
			{
				m_rotationX[i][j] = 0;
				m_rotationY[i][j] = 0;
				m_rotationZ[i][j] = 0;
			}
		}
		m_coord[i] = 0;		
		m_wordCoord[i] = 0;//最终坐标点	
	}
	m_rotX = 0;
	m_rotY = 0;
	m_rotZ = 0;
}


BvhMotionData::~BvhMotionData(void)
{
}

bool BvhMotionData::InitMatrix()
{
	int i,j;
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			if(i == j)
			{
				m_rotationX[i][j] = 1;
				m_rotationY[i][j] = 1;
				m_rotationZ[i][j] = 1;
			}
			else
			{
				m_rotationX[i][j] = 0;
				m_rotationY[i][j] = 0;
				m_rotationZ[i][j] = 0;
			}
		}
		m_coord[i] = 0;		
		m_wordCoord[i] = 0;//最终坐标点	
	}
	m_rotX = 0;
	m_rotY = 0;
	m_rotZ = 0;
	return true;
}

void BvhMotionData::SetPosition(const double offset[])
{
	for (int i=0;i<3;i++)
	{
		this->m_coord[i] = offset[i];
	}
}

void BvhMotionData::SetRotateX(const double rot_x)
{
	this->m_rotX = rot_x;
}
void BvhMotionData::SetRotateY(const double rot_y)
{
	this->m_rotY = rot_y;
}
void BvhMotionData::SetRotateZ(const double rot_z)
{
	this->m_rotZ = rot_z;
}

void BvhMotionData::TransMatrixX(double M[][4])
{
	double res[4][4];
	double trans[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	trans[0][3] = m_coord[0];
	this->MatrixMutiplication(res,M,trans);
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			M[i][j] = res[i][j];	 
		}		 
	}
}
void BvhMotionData::TransMatrixY(double M[][4])
{
	double res[4][4];
	double trans[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	trans[1][3] = m_coord[1];
	this->MatrixMutiplication(res,M,trans);
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			M[i][j] = res[i][j];	 
		}		 
	}
}
void BvhMotionData::TransMatrixZ(double M[][4])
{
	double res[4][4];
	double trans[4][4] = {{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};
	trans[2][3] = m_coord[2];
	this->MatrixMutiplication(res,M,trans);
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			M[i][j] = res[i][j];	 
		}		 
	}
}

void BvhMotionData::MatrixMutiplication(double res[][4],double A[][4],double B[][4])
{
	double sum = 0;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<4;k++)
			{
				sum+=A[i][k] * B[k][j];
			}
			res[i][j] = sum;
			sum = 0;
		}
	}
}

void BvhMotionData::RotateMatrixZ(double M[][4])
{
	double res[4][4];
	double rz = this->m_rotZ*PI/180;

	this->m_rotationZ[0][0] = cos(rz);  this->m_rotationZ[0][1] = -sin(rz); this->m_rotationZ[0][2] = 0;		this->m_rotationZ[0][3] = 0;
	this->m_rotationZ[1][0] = sin(rz);  this->m_rotationZ[1][1] = cos(rz);  this->m_rotationZ[1][2] = 0;		this->m_rotationZ[1][3] = 0;
	this->m_rotationZ[2][0] = 0;        this->m_rotationZ[2][1] = 0;      this->m_rotationZ[2][2] = 1;		this->m_rotationZ[2][3] = 0;
	this->m_rotationZ[3][0] = 0;        this->m_rotationZ[3][1] = 0;      this->m_rotationZ[3][2] = 0;		this->m_rotationZ[3][3] = 1;
	this->MatrixMutiplication(res,M,m_rotationZ);
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			M[i][j] = res[i][j];	 
		}		 
	}
}
void BvhMotionData::RotateMatrixX(double M[][4])
{
	double res[4][4];
	double rx = this->m_rotX*PI/180;
	this->m_rotationX[0][0] = 1; this->m_rotationX[0][1] = 0;        this->m_rotationX[0][2] = 0;			this->m_rotationX[0][3] = 0;
	this->m_rotationX[1][0] = 0; this->m_rotationX[1][1] = cos(rx);  this->m_rotationX[1][2] = -sin(rx);		this->m_rotationX[1][3] = 0;
	this->m_rotationX[2][0] = 0; this->m_rotationX[2][1] = sin(rx);  this->m_rotationX[2][2] = cos(rx);		this->m_rotationX[2][3] = 0;
	this->m_rotationX[3][0] = 0; this->m_rotationX[3][1] = 0;       this->m_rotationX[3][2] = 0;			this->m_rotationX[3][3] = 1;
	
	this->MatrixMutiplication(res,M,m_rotationX);
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			M[i][j] = res[i][j];	 
		}		 
	}
}
void BvhMotionData::RotateMatrixY(double M[][4])
{
	double res[4][4];
	
	double ry = this->m_rotY*PI/180;
	this->m_rotationY[0][0] = cos(ry);  this->m_rotationY[0][1] = 0; this->m_rotationY[0][2] = sin(ry);		this->m_rotationY[0][3] = 0;
	this->m_rotationY[1][0] = 0;        this->m_rotationY[1][1] = 1; this->m_rotationY[1][2] = 0;			this->m_rotationY[1][3] = 0;
	this->m_rotationY[2][0] = -sin(ry); this->m_rotationY[2][1] = 0; this->m_rotationY[2][2] = cos(ry);		this->m_rotationY[2][3] = 0;
	this->m_rotationY[3][0] = 0;        this->m_rotationY[3][1] = 0; this->m_rotationY[3][2] = 0;			this->m_rotationY[3][3] = 1;
	
	this->MatrixMutiplication(res,M,m_rotationY);
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			M[i][j] = res[i][j];	 
		}		 
	}
}

void BvhMotionData::CopyMotion(const BvhMotionData &cm)
{
	int i,j;
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			this->m_rotationX[i][j] = cm.m_rotationX[i][j];
			this->m_rotationY[i][j] = cm.m_rotationY[i][j];
			this->m_rotationZ[i][j] = cm.m_rotationZ[i][j];			
		}
		m_coord[i] = cm.m_coord[i];	
		m_wordCoord[i] = cm.m_wordCoord[i];//最终坐标点		
	} 
	m_rotX = cm.m_rotX;
	m_rotY = cm.m_rotY;
	m_rotZ = cm.m_rotZ;
}

void BvhMotionData::SetCoordination(const double ps[])
{
	for(int i=0; i<4; i++)
	{
		this->m_wordCoord[i] = ps[i];	
	}
}
void BvhMotionData::GetCoordination(double pos[])
{
	 
	int i;
	for(i=0; i<4; i++)
	{
		pos[i] = this->m_wordCoord[i];
		 
	}
	 
}