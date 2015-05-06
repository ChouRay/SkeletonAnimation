#include "stdafx.h"
#include "BvhOpenGL.h"
#include "glut.h"
BvhOpenGL::BvhOpenGL(void)
{
	m_angle = 0.0f;
	m_move = 60;
	m_ud = 0;
	m_bvhread = new BvhRead();
}


BvhOpenGL::~BvhOpenGL(void)
{
}
bool BvhOpenGL::GetFileName(string filename)
{
	string fName = "D:/Draft/BVHReader/BVH/"+filename+".bvh";
	
	if(m_bvhread->ReadBvh(fName))
	{
		return true;
	}
	return false;
}

int BvhOpenGL::GetFrameNum()
{
	return m_bvhread->GetFrames();
}

BOOL BvhOpenGL::DisplayScene()
{
	/*float speed=0.4f;	
	if (KEY_DOWN(VK_SHIFT))  speed =speed*4;
	if (KEY_DOWN(VK_LEFT))   g_Angle-=speed*2;
	if (KEY_DOWN(VK_RIGHT))  g_Angle+=speed*2;
	rad_xz = float (3.1415926* g_Angle/180.0f);
	if (KEY_DOWN(VK_PRIOR)) g_elev +=0.2f;//pgup
	if (KEY_DOWN(VK_NEXT)) g_elev -=0.2f;//pgdn
	if (g_elev<-100)	g_elev = -100;
	if (g_elev> 100)	g_elev = 100;
	if (KEY_DOWN(VK_UP))
	{ 
		g_eye[2]+=sin(rad_xz)*speed;
		g_eye[0]+=cos(rad_xz)*speed;
	}
	if (KEY_DOWN(VK_DOWN))
	{	g_eye[2]-=sin(rad_xz)*speed;
		g_eye[0]-=cos(rad_xz)*speed;
	}
	if (g_eye[0]<-(MAP*2-20))	g_eye[0]= -(MAP*2-20);
	if (g_eye[0]> (MAP*2-20))	g_eye[0]=  (MAP*2-20);
	if (g_eye[2]<-(MAP*2-20))	g_eye[2]= -(MAP*2-20);
	if (g_eye[2]> (MAP*2-20))	g_eye[2]=  (MAP*2-20);
	g_eye[1] =1.8;

	g_look[0] = float(g_eye[0] + 100*cos(rad_xz));
	g_look[2] = float(g_eye[2] + 100*sin(rad_xz));
	g_look[1] = g_eye[1];
	gluLookAt(g_eye[0],g_eye[1],g_eye[2],g_look[0],g_look[1]+g_elev,g_look[2],0.0,1.0,0.0);
	*/
	float speed = 1.0f;

	if (KEY_DOWN(VK_LEFT))
		m_angle= m_angle + speed;
	if (KEY_DOWN(VK_RIGHT))
		m_angle = m_angle - speed;
	if (KEY_DOWN(VK_UP))
		m_move = m_move - speed;
	if (KEY_DOWN(VK_DOWN))
		m_move = m_move +speed;
	if(KEY_DOWN(VK_PRIOR))
		m_ud  = m_ud + speed;
	if (KEY_DOWN(VK_NEXT))
		m_ud = m_ud - speed;
	gluLookAt(0,m_ud,m_move,0,0,0,0,1,0);
	glRotatef(m_angle,0.00,1.00,0.00);
	glTranslatef(0,-2,0);
	return TRUE;
}

GLvoid BvhOpenGL::DrawGround()
{ 
	glPushAttrib(GL_CURRENT_BIT);
	glEnable(GL_BLEND);
	glPushMatrix();
	glColor3f(0.5f, 0.7f, 1.0f);
	//glTranslatef(0,0,0);
	int size0=(int)(MAP)*2;
	glLineWidth(1.0);
	glBegin(GL_LINES);
	for (int x = -size0; x < size0; x+=1)
	{
		glVertex3i(x, 0, -size0); 
		glVertex3i(x, 0,  size0);
	}
	for (int z = -size0; z < size0;z+=1)
	{
		glVertex3i(-size0, 0, z);
		glVertex3i( size0, 0, z);
	}
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);
	glPopAttrib();
}

void BvhOpenGL::DrawSkeleton(int frames)
{
	double pos[4];
	double ppos[4];
	BvhMotionData *p = NULL;
	BvhMotionData *pp = NULL;
	
	for(int i=1;i<m_bvhread->GetJointSize(); i++)
	{
		p = &(m_bvhread->joints[i]->m_motions[frames]);
		pp= &(m_bvhread->joints[i]->m_parent->m_motions[frames]);

		p->GetCoordination(pos);
		pp->GetCoordination(ppos);
			
		////将每个节点在世界坐标系中的坐标缩小8倍
		for(int i=0;i<3; i++)
		{
			ppos[i] = ppos[i]/8;
			pos[i] = pos[i]/8;
		}
		//绘制图形
		glPushMatrix();
			glTranslated(pos[0],pos[1],pos[2]);
			glutSolidSphere(0.2,20,20);
		glPopMatrix();
		glPushMatrix();
			glTranslated(ppos[0],ppos[1],ppos[2]);
			glutSolidSphere(0.2,20,20);
		glPopMatrix();
		glLineWidth(2.0);
		glBegin(GL_LINES);
			glVertex3f(pos[0],pos[1],pos[2]);
			glVertex3f(ppos[0],ppos[1],ppos[2]);	
	    glEnd();
	}
}

void BvhOpenGL::Light0(void)
{
	GLfloat light_position[] = {1.0,10.0,-51.0,10.0};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
}
