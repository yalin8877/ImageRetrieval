#include "stdafx.h"
#include <limits>
#include "windows.h"
#include "Feature.h"
#include <stdio.h>  
#include <io.h>
#include <fstream>  
#include <math.h>
#include <atlstr.h>
#include <iostream>

#define M_PI 3.14159265358979323846
#define COLOR 1
#define TEXTURE 2
#define COLOR_TEXTURE 3
#define GRADIENT 4
#define FEATURE_TYPE COLOR_TEXTURE //根据需要的检索模式进行设定


// MyColor


Feature::Feature()
{
}

Feature::~Feature()
{
}

//找到一个字符串的扩展名
CString FindExtention(CString filepath)
{
     int len=filepath.GetLength();
	 filepath.MakeLower();
	 for (int i = len-1; i >= 0; i--)
	 {
		 if (filepath[i] == '.')
		 {
			 return (filepath.Mid(i+1));
		 }
	 }
	 return CString(_T(""));

}
//计算两个特征向量的曼哈顿距离
float Feature::m_ManhattanDis(float * featureQry, float * featureLibImg, int dim)
{
	float res = 0;
	for (int i=0; i<dim; i++)
	{
         float tmp = (featureQry[i] - featureLibImg[i]);
		 tmp = tmp>=0 ? tmp:(-tmp);
		 res += tmp;
	}
	return res;
}


// 计算一幅图片的颜色特征

void Feature::ComputeColorValue(CString filepath, float * p)
{
    IplImage* image = cvLoadImage(filepath, CV_LOAD_IMAGE_COLOR);
	
	if (NULL == image)
	{
		//MessageBox(NULL, "图像内存分配错误!", "提示" , MB_OK );
		return ;
	}

	if (image)
	{
		m_ImageWidth_Color=image->width;
		m_ImageHeight_Color = image->height;
    
		int ** ImageArray;
		ImageArray=new int*[m_ImageHeight_Color];
		for (int i=0;i<m_ImageHeight_Color;i++)
		{
			ImageArray[i]=new int[m_ImageWidth_Color];
		}
    
		for(int i=0;i<m_ImageHeight_Color; i++)  
		{
			for(int j=0; j<m_ImageWidth_Color; j++)
			{
				CvScalar pixelValue ;
				pixelValue = cvGet2D(image, i, j);//单个象素处理

				int r = pixelValue.val[2];
				int g = pixelValue.val[1];
				int b = pixelValue.val[0];

				//RGB空间转化为HSV空间
				int h;                 
				double s,v;
				int hlevel,slevel,vlevel;
				int max,min;

				max = ((r>g?r:g)>(g>b?g:b))?(r>g?r:g):(g>b?g:b);
				min = ((r<g?r:g)>(g<b?g:b))?(r<g?r:g):(g<b?g:b);
				v = max;
				v = v*1.0/256;        

				if(max==0||min==0)
				{
					s=0;
				}
				else
				{
					s=(max-min)*1.0/max;  
				}

				if(max!=min)
				{
					if(r==max)
					{
						h=(g-b)*60/(max-min);
					}				
					else if(g==max)
					{
						h=120+(b-r)*60/(max-min);                   
					}
					else
					{
						h=240+(r-g)*60/(max-min);
					}
				}
				else
				{
					h=0;
				}
				if(h==360) 
					h=0;			
				if(h<0)    
					h+=360;

				if(v<=0.2)
					ImageArray[i][j] = 0;
				else if((s<=0.1)&&(v>0.2))
				{
					if((v>0.2)&&(v<=0.5))
						ImageArray[i][j] = 1;
					if((v>0.5)&&(v<=0.8))
						ImageArray[i][j] = 2;
					if((v>0.8)&&(v<=1))
						ImageArray[i][j] = 3;
				}
				else
				{  
					if((h>20)&&(h<=45))
						hlevel = 1;
					else if((h>46)&&(h<=75))
						hlevel = 2;
					else if((h>76)&&(h<=165))
						hlevel = 3;
					else if((h>156)&&(h<=200))
						hlevel = 4;
					else if((h>201)&&(h<=270))
						hlevel = 5;
					else if((h>271)&&(h<=330))
						hlevel = 6;				
					else 
						hlevel = 0;

					if((s>=0.1)&&(s<=0.45))
						slevel = 0;
					else if((s>0.45)&&(s<=1))
						slevel = 1; 

					if((v>=0.2)&&(v<=0.5))
						vlevel = 0;
					else if((v>0.5)&&(v<=1))
						vlevel = 1;

					ImageArray[i][j] = 4*hlevel+2*slevel+vlevel+4;
				}
			}
		}

		m_gave = 0; 
		m_gvar = 0;
		m_gske = 0;

		for(int i=0;i<m_ImageHeight_Color; i++) 
		{
			for(int j=0; j<m_ImageWidth_Color; j++)
			{
				m_gave+= ImageArray[i][j];
			}
		}
		m_gave = (m_gave/(m_ImageHeight_Color*m_ImageWidth_Color));

		for(int i=0;i<m_ImageHeight_Color; i++)
		{
			for(int j=0; j<m_ImageWidth_Color; j++)
			{
				m_gvar+= pow((ImageArray[i][j]-m_gave),2);
			}
		}
		m_gvar = (m_gvar/(m_ImageHeight_Color*m_ImageWidth_Color));
		m_gvar = (sqrt(m_gvar));

		for(int i=0;i<m_ImageHeight_Color; i++)  
		{
			for(int j=0; j<m_ImageWidth_Color; j++)
			{
				m_gske+= pow((ImageArray[i][j]-m_gave),3);
			}
		}
		m_gske = (m_gske/(m_ImageHeight_Color*m_ImageWidth_Color));
		m_gske = (float)pow(fabs((double)m_gske),1.0/3);

		int isub,jsub; 
		int row,col;

		for(int i=0;i<4;i++)
		{
			for(int j=0;j<32;j++)
			{
				ColorCount[i][j]=0;
			}
		}
		for(row=0;row<2;row++)
		{
			for(col=0;col<2;col++)		
			{
				int subcount = row*2+col;
				for(isub=0;isub<m_ImageHeight_Color/2;isub++)
				{
					for(jsub=0;jsub<m_ImageWidth_Color/2;jsub++)
					{
						int ii=col*m_ImageHeight_Color/2+isub;					
						int jj=row*m_ImageWidth_Color/2+jsub;			 

						int colorlevel= ImageArray[ii][jj];
						ColorCount[subcount][colorlevel]+=1;		
					}		
				}
			}
		}			
		int idx=0;
		for (int i=0;i<4;i++)
		{
			for (int j=0;j<32;j++)
			{
				p[idx]=ColorCount[i][j];
				idx++;
			}
		}

		cvReleaseImage( &image );

		for (int i=0;i<m_ImageHeight_Color;i++)
		{
			delete [] ImageArray[i];
		}
		delete [] ImageArray;
	}
	else
	{
		cout<<"can't open the picture!"<<endl;
		p=NULL;
	}
}



void Feature::ComputeTextureValue(CString filepath, float * p)
{
    IplImage* image = cvLoadImage(filepath, CV_LOAD_IMAGE_COLOR);

	if (image)
	{
		m_ImageWidth_Texture=image->width;
		m_ImageHeight_Texture=image->height;
		int const subPixelWidth=4;
		int const subPixelHeight=4;

		for(int i=0;i<4;i++)
			for(int j=0;j<4;j++)
				for(int k=0;k<5;k++)
					m_EdgeType[i][j][k]=0;

		for (int i=0;i<4;i++)
			for(int j=0;j<4;j++)
			{
				int count=0;
				for(int x=0; x<m_ImageHeight_Texture/(4*subPixelHeight);x++)
					for(int y=0;y<m_ImageWidth_Texture/(4*subPixelWidth);y++)
					{
						int type=comsubedgetype(m_ImageHeight_Texture/4*i+subPixelHeight*x,  m_ImageWidth_Texture/4*j+subPixelWidth*y,image);
						switch(type)
						{
							case 0 :(m_EdgeType[i][j][0])++;count++;break;
							case 1 :(m_EdgeType[i][j][1])++;count++;break;
							case 2 :(m_EdgeType[i][j][2])++;count++;break;
							case 3 :(m_EdgeType[i][j][3])++;count++;break;
							case 4 :(m_EdgeType[i][j][4])++;count++;break;
							default:break;
						}
						CString temp;
						temp.Format("%d",type);
						//AfxMessageBox(temp);
					}
				for(int k=0;k<5;k++)
				{
					if(count!=0)
						m_EdgeType[i][j][k]/=count;
					else 
						m_EdgeType[i][j][k]=0;
					CString temp;
					temp.Format("%f",m_EdgeType[i][j][k]);
				}
		}

		for(int k=0;k<5;k++)
			m_GlobalEdgeType[k]=0;

		for(int i=0;i<4;i++)
			for(int k=0;k<5;k++)
			{
				m_RowsEdgeType[i][k]=0;
				m_ColumnsEdgeType[i][k]=0;
			}

		for(int i=0;i<5;i++)
			for(int k=0;k<5;k++)
				m_2X2EdgeType[i][k]=0;

		for(int k=0;k<5;k++)
			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
					m_GlobalEdgeType[k]+=m_EdgeType[i][j][k];	
		for(int k=0;k<5;k++)
			m_GlobalEdgeType[k]/=16;

		for(int i=0;i<4;i++)
			for(int k=0;k<5;k++)
				for(int j=0;j<4;j++)
				{
					m_RowsEdgeType[i][k]+=m_EdgeType[i][j][k];
					m_ColumnsEdgeType[j][k]+=m_EdgeType[i][j][k];
				}
		for(int i=0;i<4;i++)
			for(int k=0;k<5;k++) 
				m_RowsEdgeType[i][k]/=4;

		for(int j=0;j<4;j++)
			for(int k=0;k<5;k++) 
				m_RowsEdgeType[j][k]/=4;

		for(int i=0;i<2;i++)
			for(int j=0;j<2;j++)
				for(int k=0;k<5;k++)
					m_2X2EdgeType[0][k]+=m_EdgeType[i][j][k];

		for(int i=0;i<2;i++)
			for(int j=2;j<4;j++)
				for(int k=0;k<5;k++)
					m_2X2EdgeType[1][k]+=m_EdgeType[i][j][k];
		for(int i=2;i<4;i++)
			for(int j=0;j<2;j++)
				for(int k=0;k<5;k++)
					m_2X2EdgeType[2][k]+=m_EdgeType[i][j][k];
		for(int i=2;i<4;i++)
			for(int j=2;j<4;j++)
				for(int k=0;k<5;k++)
					m_2X2EdgeType[3][k]+=m_EdgeType[i][j][k];
		for(int i=1;i<3;i++)
			for(int j=1;j<3;j++)
				for(int k=0;k<5;k++)
					m_2X2EdgeType[4][k]+=m_EdgeType[i][j][k];

		for(int i=0;i<5;i++)
			for(int k=0;k<5;k++)
				m_2X2EdgeType[i][k]/=4;
		int idx=0;
		for (int i=0;i<5;i++)
		{
			p[idx]=m_GlobalEdgeType[i];
			idx++;
		}
		for (int i=0;i<5;i++)
			for(int j=0;j<5;j++)
		{
			p[idx]=m_2X2EdgeType[i][j];
			idx++;
		}

		cvReleaseImage( &image );
	}
	 else
	 {
		 cout<<"can't open the picture!"<<endl;
		 p=NULL;
	 }

}
int Feature::comsubedgetype(int x,int y, IplImage* myimage)
{
	int const T=11;
	int H=myimage->height;
	int W=myimage->width;
		
	float A=0,B=0,C=0,D=0;
    CvScalar pixelValue;
	for(int i=x;i<x+2;i++)
	{
		for(int j=y;j<y+2;j++)
		{
			pixelValue = cvGet2D(myimage, i, j);

			A+=(unsigned int)(((unsigned int)pixelValue.val[0]*0.114
					  +(unsigned int)pixelValue.val[1]*0.587
					  +(unsigned int)pixelValue.val[2]*0.229));
		}
		for(int j=y+2;j<y+4;j++)
		{
			pixelValue = cvGet2D(myimage, i ,j);
			B+=(unsigned int)(((unsigned int)pixelValue.val[0]*0.114+(unsigned int)pixelValue.val[1]*0.587+(unsigned int)pixelValue.val[2]*0.229));
		}
	}

	for(int i=x+2;i<x+4;i++)
	{
		for(int j=y;j<y+2;j++)
		{
			pixelValue = cvGet2D(myimage, i, j );
			C+=(unsigned int)(((unsigned int)pixelValue.val[0]*0.114+(unsigned int)pixelValue.val[1]*0.587+(unsigned int)pixelValue.val[2]*0.229));  // C+=int(myimage->GetPixelGray(j,H-i-1));
		}
		for(int j=y+2;j<y+4;j++)
		{
			pixelValue = cvGet2D(myimage,  i, j);
			D+=(unsigned int)(((unsigned int)pixelValue.val[0]*0.114	+(unsigned int)pixelValue.val[1]*0.587+(unsigned int)pixelValue.val[2]*0.229));// D+=int(myimage->GetPixelGray(j,H-i-1));
		}
	}
	A=A/4;B=B/4;C=C/4;D=D/4;

	float edge[5]={0,0,0,0,0};
	edge[0]=A-B+C-D;
	edge[1]=A+B-C-D;
	edge[2]=A*sqrt(2.0)-D*sqrt(2.0);
	edge[3]=B*sqrt(2.0)-C*sqrt(2.0);
	edge[4]=2*(A-B-C+D);
	float  max=-10000;
	int number=0;

	for(int i=0;i<5;i++)
	{
		if(abs(edge[i])>max)
		{
			max=abs(edge[i]);
			number=i;
		}
	}
	if (max>T) 
		return number;
	else 
		return 5;

}

void Feature::ComputeColorTextureValue(CString filepath, float * p)
{
	float* p1 = new float[COLOR_FEATURE_DIM];
	Feature::ComputeColorValue(filepath,p1);
    
	int min = 0, max = 0;
	for (int i=0; i<COLOR_FEATURE_DIM; i++)
	{
        if (p1[i] < min)
        {
			min = p1[i];
        }

		if (p1[i] > max)
		{
			max = p1[i];
		}
	}
	for (int i=0; i<COLOR_FEATURE_DIM; i++)
	{
		p1[i] = (p1[i] - min)/(max - min);
	}
	float* p2 = new float[TEXTURE_FEATURE_DIM];
	Feature::ComputeTextureValue(filepath,p2);

    if (p1==NULL||p2==NULL)
    {
		p=NULL;
    }
	else
	{

		int idx=0;
		for (int i=0;i<COLOR_FEATURE_DIM;i++)
		{
			p[idx]=p1[i];
			idx++;
		}
		for (int i=0;i<TEXTURE_FEATURE_DIM;i++)
		{
			p[idx]=p2[i];
			idx++;
		}
    }
	delete []p1;
	delete []p2;
}

void Feature::normalize_histogram(float* L_begin, float* L_end)
{
	float* L_iter ;
	float norm=0.0 ;

	for(L_iter = L_begin; L_iter != L_end ; ++L_iter)
		norm += (*L_iter) * (*L_iter) ;

	norm = sqrt(norm) ;

	for(L_iter = L_begin; L_iter != L_end ; ++L_iter)
	{
		if(norm!=0)
			*L_iter /= norm ;
	}
}

void Feature::descript(const unsigned char *data, vector<float>& vecFloat, int subWidth, int subHeight)
{
	int nrows = subHeight;
	int ncols = subWidth;

	float *gradModule = new float[nrows * ncols](); 
	float *gradAngle = new float[nrows * ncols]();

	for (int x = 1; x != ncols - 1; x++)
	{
		for (int y = 1; y != nrows - 1; y++)
		{

			float Dx = (float) 0.5 * (data[(x + 1) + y * ncols] - data[(x - 1) + y * ncols]);
			float Dy = (float) 0.5 * (data[x + (y + 1) * ncols] - data[x + (y - 1) * ncols]);

			if (Dx == 0)
			{
				Dx = numeric_limits<float>::epsilon(); 
			}

			gradModule[x + y * ncols] = sqrt(Dx * Dx + Dy * Dy);            
			gradAngle[x + y * ncols] = (float)(atan2(Dy, Dx) + M_PI);;     
		}
	}


	float imgCenter = float(ncols * 1.0/2 - 0.5);

	float *desc = new float[8]();

	for (int x = 1; x != ncols - 1; x++)
	{
		for (int y = 1; y != nrows - 1; y++)
		{			

			float value = gradModule[x + y * ncols];

			float angle = gradAngle[x + y * ncols];

			int bin = int(angle / (2 * M_PI / 8));

			float ratio = float(((bin + 1) * (2 * M_PI / 8) - angle) / (2 * M_PI / 8)) ;

			if (bin == 8)
			{
				bin = 0; 
			}

			if (bin != (8  - 1))
			{				
				*(desc + bin) += value * ratio;
				*(desc + bin + 1) += value * (1 - ratio);
			}
			else
			{
				*(desc + bin) += value * ratio;
				*(desc) += value * (1 - ratio);
			}
		}

	}
	
	normalize_histogram(desc, desc + 8);

	for (int i = 0; i != 8; i++)
	{
		vecFloat.push_back(desc[i]);
	}

	delete [] gradAngle;
	delete [] gradModule;
	delete [] desc;
}

void Feature::ComputeGradientValue(CString filepath, float* p)
{

	
    IplImage * image = cvLoadImage(filepath ,CV_LOAD_IMAGE_COLOR );

	if (NULL == image)
	{
		MessageBox(NULL, "ERROR!\n" ,"提示", MB_OK);
	}
	
	if (image)
	{	

		m_ImageWidth_Gradient=image->width;
		m_ImageHeight_Gradient=image->height;

		int ** ImageArray;
		ImageArray=new int*[m_ImageHeight_Gradient];
		for (int i=0;i<m_ImageHeight_Gradient;i++)
		{
			ImageArray[i]=new int[m_ImageWidth_Gradient];
		}

		for(int i=0;i<m_ImageHeight_Gradient; i++)  
			for(int j=0; j<m_ImageWidth_Gradient; j++)
			{
				CvScalar pixelValue = cvGet2D(image, i, j);
				ImageArray[i][j] = (((unsigned int)pixelValue.val[0]*0.114
					+(unsigned int)pixelValue.val[1]*0.587
					+(unsigned int)pixelValue.val[2]*0.229));	
			}

			int subHeight = (m_ImageHeight_Gradient / 3), subWidth = (m_ImageWidth_Gradient / 3);
			unsigned char * data = new unsigned char[subWidth*subHeight]();
			vector<float> vecData;
			vecData.clear();

			for(int row=0;row<3;row++)
				for(int col=0;col<3;col++)		
				{
					int startX = row * subHeight;
					int startY = col * subWidth;
					int count = 0;

					for(int i = 0; i < subHeight; i++)
						for (int j=0; j<subWidth; j++)
						{

							data[count ++]=ImageArray[i + startX][j + startY];
							data[count - 1];
						}

						descript(data, vecData, subWidth, subHeight);
				}	

				for (int i= 0 ;i<vecData.size(); i++)
				{
					p[i] = vecData.at(i);
				}

	}
	else
	{
		cout<<"can't open the picture!"<<endl;
		p=NULL;
	}
}

void Feature::m_GetFeature(CString filepath,float* featureArr)
{
     switch (FEATURE_TYPE)
     {
	 case COLOR: 
			ComputeColorValue(filepath, featureArr);
     		break;
	 case TEXTURE: 
			ComputeTextureValue(filepath, featureArr);
		    break;
	 case COLOR_TEXTURE: 
			ComputeColorTextureValue(filepath, featureArr);
			break;
	 case GRADIENT: 
		    ComputeGradientValue(filepath, featureArr);
		    break;
	 default:
		    break;
	 }
}