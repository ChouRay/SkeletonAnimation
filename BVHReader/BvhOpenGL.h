#if !defined(AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_)
#define AFX_BAISCOBJ_H__6F90C6A0_F5E4_4482_BA6B_136D5C922B31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "BvhRead.h"
#include <string>

using namespace std;


class BvhOpenGL
{
public:
	BvhOpenGL(void);
	~BvhOpenGL(void);
private:
	BvhRead *m_bvhread;
public:
	GLvoid	DrawGround();
	BOOL		DisplayScene();
	void Light0(void);
	bool GetFileName(string filename);
	void DrawSkeleton(int frames);
	int GetFrameNum();
private:
	GLfloat m_angle;
	GLfloat m_eyex;
	GLfloat m_move;
	GLfloat m_ud;
};
#endif
