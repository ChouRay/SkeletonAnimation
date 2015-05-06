#pragma once
class BvhMotionData
{
public:
	BvhMotionData(void);
	~BvhMotionData(void);
	bool InitMatrix();
	void SetPosition(const double offset[]);  //根节点添加坐标
	void SetRotateZ(const double rot_z);
	void SetRotateX(const double rot_x);
	void SetRotateY(const double rot_y);
	
	
	void TransMatrixX(double M[][4]);
	void TransMatrixY(double M[][4]);
	void TransMatrixZ(double M[][4]);

	void RotateMatrixZ(double M[][4]);
	void RotateMatrixX(double M[][4]);
	void RotateMatrixY(double M[][4]);
	void MatrixMutiplication(double res[][4],double A[][4],double B[][4]);
	
	void CopyMotion(const BvhMotionData &cm);
	void SetCoordination(const double ps[]); //设置坐标点
	void GetCoordination(double pos[]);
private:
	double m_coord[4];
	double m_wordCoord[4];
	double m_rotZ;
	double m_rotX;
	double m_rotY;
	
	double m_rotationZ[4][4];
	double m_rotationX[4][4];
	double m_rotationY[4][4];
};

