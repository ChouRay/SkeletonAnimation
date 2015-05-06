#include "StdAfx.h"
#include "BvhRead.h"
#include <fstream>
#include <iterator>

ofstream outBvhError("./error_Bvhtest.txt");

BvhRead::BvhRead(void)
{
	this->m_jointNum = 0;
	this->m_offNum = 0;
	this->m_channel = 0;
	this->m_frames = 0;
	double m_frametime=0;
	this->m_curr = NULL;
	this->m_root = NULL;
	this->temp = NULL;
	this->m_tMotion.InitMatrix();
	isadd = false;
	tempnum = 0;
}


BvhRead::~BvhRead(void)
{
	clear();
	outBvhError.close();
}

void BvhRead::clear(void)
{	
	if(!this->ClearFramework(this->m_root))
	{
		outBvhError<<"**  Free Error!  **"<<endl;
	}
	else
	{
		outBvhError<<"** Free Framework successfully **\n";
		outBvhError<<"*********************************\n";
	}
	this->m_root = NULL;
	this->m_curr = NULL;

}
bool BvhRead::ClearFramework(BvhJoint *p)
{
	if(p == NULL)
	{
		return false;
	}	
	this->SubFree(p);
	return true;
}
void BvhRead::SubFree(BvhJoint *p)
{
	if((p->children.size()) == 0)
	{
		return;
	}

	unsigned int i=0;
	for(i=0; i<(p->children.size()); i++)
	{
		this->SubFree(p->children[i]);
		delete p->children[i];		 
	}	 
}
bool BvhRead::ReadBvh(string filename)
{
	ifstream infile(filename.c_str());
	
	if (!infile)
	{
		outBvhError << "**  The file  opened  unsuccessfully **\n";
		return false;
	}
	istream_iterator<string> first(infile);
	istream_iterator<string> end;
	vector<string> lines;
	copy(first,end,back_inserter(lines));

	for(int i=0;i<lines.size();i++)
	{
		this->BuildSkeleton(lines[i]);	//建立骨架
	}
	
	for(int i=0;i<m_frames; i++)  //计算坐标值
	{
	    this->DealCoordination(i);
	}
	this->print();//打印坐标值 输入到文件 
	return true;
}

void BvhRead::BuildSkeleton(string line)
{
		if (line=="HIERACHY")
		{
			return;
		}
		else if(line=="ROOT")
		{
			this->m_key = ROOT;
		}
		else if(line=="OFFSET")
		{
			this->m_key = OFFSET;
			this->m_offNum = 0;
		}
		else if(line=="CHANNELS")
		{
			this->m_key = CHANNELS;
			this->m_channel = -1;
			this->m_curr->SetChannelNum(0);
		}
		else if(line=="JOINT")
		{
			this->m_key = JOINT;
		}
		else if(line=="End")
		{
			temp = new BvhJoint;
			temp->SetJointName("EndSite");
			temp->m_parent = m_curr;
			temp->SetChannelNum(0);

			this->m_curr->children.push_back(temp);
			this->m_curr = temp;
			joints.push_back(this->m_curr);   //
			temp=NULL;
			this->m_key = END;
		}
		else if(line=="SITE")
		{
			this->m_key = SITE;
		}
		else if(line=="{")
		{
			return;
		}
		else if(line=="}")
		{
			this->m_key = NONE;

			if(this->m_curr != this->m_root)
			{
				this->m_curr = this->m_curr->m_parent;
			}
		}
		else if(line=="MOTION")
		{
			this->m_key = MOTION;
		}
		else if(line=="Frames:")
		{
			this->m_key = FRAMES;
		}
		else if(line=="Frame")
		{
			this->m_key = FRAME;
		}
		else if(line=="Time:")
		{
			this->m_key = TIME;
		}
		else
		{
			switch(this->m_key)
			{
			case ROOT:
				this->m_curr = new BvhJoint;
				this->m_curr->SetJointName(line);
				this->m_curr->m_parent = NULL;
				this->m_root = this->m_curr;
				this->m_key = NONE;
				joints.push_back(this->m_curr);
				break;
			case JOINT:
				temp = new BvhJoint;
				temp->SetJointName(line);
				temp->m_parent = m_curr;
				temp->SetChannelNum(0);
				this->m_curr->children.push_back(temp);
				this->m_curr = temp;
				joints.push_back(this->m_curr);	
				this->m_key = NONE;
				temp = NULL;
				break;
			case SITE:
				break;
			case OFFSET:
				this->m_offset[this->m_offNum] = atof(line.c_str());
				this->m_offNum++;
				if(m_offNum == 3)
				{
					this->m_key = NONE;
					this->m_curr->SetOffset(this->m_offset);
				}
				break;
			case CHANNELS:
				this->m_channel++;
				if(this->m_curr->GetChannelNum() ==0)
				{
					this->m_curr->SetChannelNum(atof(line.c_str()));///
				}
				else if(this->m_channel ==this->m_curr->GetChannelNum())
				{
					this->m_key = NONE;
				}
	
				if (line == "Xposition")
				{
					this->m_curr->channels.push_back(BvhJoint::Xpos);
				}
				else if(line=="Yposition")
				{
					this->m_curr->channels.push_back(BvhJoint::Ypos);
				}
				else if(line=="Zposition")
				{
					this->m_curr->channels.push_back(BvhJoint::Zpos);
				}
				else if(line=="Zrotation")
				{
					this->m_curr->channels.push_back(BvhJoint::Zrot);
				}
				else if(line=="Xrotation")
				{
					this->m_curr->channels.push_back(BvhJoint::Xrot);
				}
				else if(line=="Yrotation")
				{
					this->m_curr->channels.push_back(BvhJoint::Yrot);
				}
				break;
			case FRAMES:
				this->m_frames = atoi(line.c_str());
				this->m_key = NONE;
				break;
			case FRAME:
				break;
			case NONE:
				break;
			case TIME:
				this->m_frametime = atof(line.c_str());
				this->m_key = MOTIONDATA;
				this->m_jointNum = 0;
				this->m_channel = 0;
				break;
			case MOTIONDATA:
				switch(joints[this->m_jointNum]->channels.at(this->m_channel))
				{
				case BvhJoint::Xpos:
					this->m_offset[0] = atof(line.c_str());
					this->m_channel++;
					break;
				case BvhJoint::Ypos:
					this->m_offset[1] = atof(line.c_str());
					this->m_channel++;
					break;
				case BvhJoint::Zpos:
					this->m_offset[2] = atof(line.c_str());
					this->m_channel++;
					break;
				case BvhJoint::Zrot:
					this->m_tMotion.SetRotateZ(atof(line.c_str()));
					this->m_channel++;
					break;
				case BvhJoint::Xrot:
					this->m_tMotion.SetRotateX(atof(line.c_str()));
					this->m_channel++;
					break;
				case BvhJoint::Yrot:
					this->m_tMotion.SetRotateY(atof(line.c_str()));
					this->m_channel++;
					break;
				default:
					break;
				}

				if(this->m_channel == joints[this->m_jointNum]->GetChannelNum())
				{
					if(this->m_channel==6)
					{
						this->m_tMotion.SetPosition(this->m_offset); //根节点更新偏移量
					}
					joints[this->m_jointNum]->InsertOneMotion(m_tMotion);//添加旋转矩阵
					this->m_jointNum++;		//下一节点
					this->m_channel = 0;    //自由度归零

					m_tMotion.InitMatrix();	//m_tMotion重新初始化

					if(joints[m_jointNum]->GetJointName() == "EndSite")
					{
						joints[this->m_jointNum]->InsertOneMotion(m_tMotion);//添加旋转矩阵 
						this->m_jointNum++;
						m_tMotion.InitMatrix();
					
						if(this->m_jointNum == joints.size())//如果是最后的end结点 则从根结点再读数据
						{
							this->m_jointNum = 0;
						}
					}
				}
				break;
			default:
				break;
			}
		}
} 

void BvhRead::DealCoordination(int i)
{
	if(i >= this->m_frames)
	{
		i=0;
	}	 
	double M[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};     
	isadd = false;
	DealCoordination(M,i,joints[0]);
	
}

void BvhRead::DealCoordination(double M[][4],int n,BvhJoint *point)// 第n帧
{
	BvhMotionData *temp = &(point->m_motions[n]);//转换成指针操作    
	int i,j,k;	 
	double S[4] = {0,0,0,1};
	double E[4];
	double res1[4];
	double res2[4];
	double off[3];
	double sum1 = 0; 
	double sum2 = 0;	 
	
	// 设置起始点 和 终止点的偏移量	
	point->GetOffset(off);
	S[0] = 0;	S[1] = 0;       	S[2] = 0;       	S[3] = 1;
	E[0] = off[0];	E[1] = off[1];		E[2] = off[2];		E[3] = 1;	 
	//E = M * E;  S = M * S 乘以旋转矩阵 获得旋转后的坐标点
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{			
			sum1 = sum1 + M[i][j] * S[j];
			sum2 = sum2 + M[i][j] * E[j];			
		}
		res1[i] = sum1;
		res2[i] = sum2;
		sum1 = 0;
		sum2 = 0;		
	}
	for(i=0;i<4;i++)
	{
		S[i] = res1[i];
		E[i] = res2[i];
	}


 	//每个E应该是该节点的坐标，但是由于递归条件则头结点时E为0无效 
	if(point->m_parent != NULL)
	{
		node nt;
		double s[3];
		point->m_parent->GetOffset(s);
		if(point->m_parent->m_parent == NULL && !isadd)//第一次求得的S为root 但是只能添加一次
		{
			nt.x = S[0]; nt.y = S[1]; nt.z = S[2]; nt.w = S[3];
		    ptemp.push_back(nt);
		    
			
			nt.x = E[0]; nt.y = E[1]; nt.z = E[2]; nt.w = E[3];
			ptemp.push_back(nt);

			isadd = true;
			
		}
		else
		{
			nt.x = E[0]; nt.y = E[1]; nt.z = E[2]; nt.w = E[3];
			ptemp.push_back(nt);
		}	    		 	
		 
	}  	

	//乘以平移矩阵	 
	Translate(off[0],off[1],off[2],M);
	//乘以旋转矩阵
	for (i=0; i<point->GetChannelNum(); i++) 
	{
		//float amount = (*i)->GetValue(frame);
		switch (point->channels[i]) 
		{
		case BvhJoint::Xrot:
			temp->RotateMatrixX(M); 			
			break;
		case BvhJoint::Yrot:
			temp->RotateMatrixY(M); 			
			break;
		case BvhJoint::Zrot:	
			temp->RotateMatrixZ(M); 		
			break;
		case BvhJoint::Xpos:	
			temp->TransMatrixX(M); 			
			break;
		case BvhJoint::Ypos:	
			temp->TransMatrixY(M); 			
			break;
		case BvhJoint::Zpos:	
			temp->TransMatrixZ(M); 			
			break;
		default:
			break;
		} 		
	}
 
	double res[4][4];
	for(k=0; k < point->children.size(); k++) 
	{

		//保存M 因为直接递归的话M会变化 为了保存当前的M
		for(i=0;i<4;i++)
		{
			for(j=0;j<4;j++)
			{
				res[i][j] = M[i][j];
			}
		}
		//递归
		DealCoordination(res,n,point->children[k]);
	}
	
}	

void BvhRead::Translate(double x,double y,double z,double M[][4])
{
	int i,j,k;
	double res[4][4];
	double sum = 0;
	double T[4][4]={{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}};

	T[0][3] = x;
	T[1][3] = y;
	T[2][3] = z;
	
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			for(k=0; k<4; k++)
			{
				sum = sum + M[i][k] * T[k][j];
			}
			res[i][j] = sum;			 
			sum = 0;
		}
	} 

	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
		
			M[i][j] = res[i][j];
		}
	} 
}
int BvhRead::GetFrames()
{
	return this->m_frames;
}

int BvhRead::GetJointSize(void)
{
	return this->joints.size();
}

bool BvhRead::print(void)
{
	int i,j;
	//打开文件
	ofstream outfile("D:/Draft/BVHReader/joint.txt");	
    //失败
	if(!outfile)
	{
		return false;
	}
	//成功
	outfile<<"joints  "<<this->joints.size()<<"\n";
	for(i=0; i<joints.size(); i++)
	{
		outfile<<joints[i]->GetJointName()<<" ";
		if(i == 0)
		{
			outfile<<"NULL\n";
		}
		else
		{
			outfile<<joints[i]->m_parent->GetJointName()<<"\n";
		}
	}
	
	//从vector中取出坐标点 设置节点坐标值并输出到文件
	
	double s[4] = {0,0,0,0};
	 
	for(i = 0; i<m_frames; i++)
	{
		for(j=0;j<joints.size();j++)
		{
			 
 			node *nd = &(ptemp[tempnum]);			
			s[0] = nd->x; s[1] = nd->y; s[2] = nd->z; s[3] = nd->w;	
			outfile<<joints[j]->GetJointName()<<"  "<<s[0]<<"  "<<s[1]<<"  "<<s[2]<<"  "<<s[3]<<"\n";
	        joints[j]->m_motions[i].SetCoordination(s);

			tempnum++;
 
		}
	}

	outfile.close();
	return true;
}