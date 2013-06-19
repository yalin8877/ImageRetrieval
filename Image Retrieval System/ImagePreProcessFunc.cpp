#include "StdAfx.h"
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "ImagePreProcessFunc.h"

using namespace std;
using namespace cv;

//#define DEBUG_QUANTISITION



char* ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn)
{
	LPSTR pszOut = NULL;
	if (lpwszStrIn != NULL)
	{
		int nInputStrLen = wcslen (lpwszStrIn);

		// Double NULL Termination
		int nOutputStrLen = WideCharToMultiByte (CP_ACP, 0, lpwszStrIn, nInputStrLen, NULL, 0, 0, 0) + 2;
		pszOut = new char [nOutputStrLen];

		if (pszOut)
		{
			memset (pszOut, 0x00, nOutputStrLen);
			WideCharToMultiByte(CP_ACP, 0, lpwszStrIn, nInputStrLen, pszOut, nOutputStrLen, 0, 0);
		}
	}
	return pszOut;
}

//HBITMAP IplIamgeToDIB(IplImage *pImg)  
//{  
//	HDC hDC = ::CreateCompatibleDC(0);  
//	BYTE tmp[sizeof(BITMAPINFO)+255*4];  
//	BITMAPINFO *bmi = (BITMAPINFO*)tmp;  
//	HBITMAP hBmp;  
//	int i;  
//	memset(bmi,0,sizeof(BITMAPINFO));  
//	bmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);  
//	bmi->bmiHeader.biWidth = pImg->width;  
//	bmi->bmiHeader.biHeight = pImg->height;  
//	bmi->bmiHeader.biPlanes = 3;  
//	bmi->bmiHeader.biBitCount = pImg->nChannels * pImg->depth;  
//	bmi->bmiHeader.biCompression = BI_RGB;  
//	bmi->bmiHeader.biSizeImage = pImg->width*pImg->height*1;  
//	bmi->bmiHeader.biClrImportant =0 ;  
//	switch(pImg->nChannels * pImg->depth)  
//	{  
//	case 8 :  
//		for(i=0 ; i < 256 ; i++)  
//		{  
//			bmi->bmiColors[i].rgbBlue = i;  
//			bmi->bmiColors[i].rgbGreen= i;  
//			bmi->bmiColors[i].rgbRed= i;  
//		}  
//		break;  
//	case 32:  
//	case 24:  
//		((DWORD*) bmi->bmiColors)[0] = 0x00FF0000; /* red mask */  
//		((DWORD*) bmi->bmiColors)[1] = 0x0000FF00; /* green mask */  
//
//		((DWORD*) bmi->bmiColors)[2] = 0x000000FF; /* blue mask */  
//
//		break;  
//
//	}  
//
//	hBmp = ::CreateDIBSection(hDC,bmi,DIB_RGB_COLORS,NULL,0,0);  
//
//	SetDIBits(hDC,hBmp,0,pImg->height,pImg->imageData,bmi,DIB_RGB_COLORS);  
//
//	::DeleteDC(hDC);  
//	return hBmp;  
//}  

//* convert to HSV */
//* wrong : the pointer area will be released when the function ends */
IplImage** RGB2HSVplanes(IplImage* rgb)
{
	IplImage* hsv = cvCreateImage( cvGetSize(rgb), 8, 3 );
	IplImage* h_plane = cvCreateImage( cvGetSize(rgb), 8, 1 );
	IplImage* s_plane = cvCreateImage( cvGetSize(rgb), 8, 1 );
	IplImage* v_plane = cvCreateImage( cvGetSize(rgb), 8, 1 );
	IplImage* planes[] = { h_plane, s_plane , v_plane };

	cvCvtColor( rgb, hsv, CV_BGR2HSV );
	cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );

	return planes;
}

////* smooth the image */
//void smooth(IplImage** plane)
//{
//	for(int p_num=0;p_num<3;p_num++)
//	{
//		for(int i=1;i<plane[p_num]->height-1;i++)
//		{
//			for(int j=1;j<plane[p_num]->width-1;j++)
//			{
//				char target=*(plane[p_num]->imageData+i*plane[p_num]->widthStep+j);
//				char left=*(plane[p_num]->imageData+i*plane[p_num]->widthStep+j-1);
//				char right=*(plane[p_num]->imageData+i*plane[p_num]->widthStep+j+1);
//				char up=*(plane[p_num]->imageData+(i-1)*plane[p_num]->widthStep+j);
//				char down=*(plane[p_num]->imageData+(i+1)*plane[p_num]->widthStep+j);
//				char upleft=*(plane[p_num]->imageData+(i-1)*plane[p_num]->widthStep+j-1);
//				char upright=*(plane[p_num]->imageData+(i-1)*plane[p_num]->widthStep+j+1);
//				char downleft=*(plane[p_num]->imageData+(i+1)*plane[p_num]->widthStep+j-1);
//				char downright=*(plane[p_num]->imageData+(i+1)*plane[p_num]->widthStep+j+1);
//				*(plane[p_num]->imageData+i*plane[p_num]->widthStep+j)=(target*8+left+right+up+down+upleft+upright+downleft+downright)/16;
//			}
//		}
//	}
//}

//* using the method in <<基于色彩量化及索引的图像检索>>,汪华章，何小海，宰文姣，王炜 */
void ColorQuantization(IplImage** planes, IplImage* quantized)
{
	IplImage* h_plane=planes[0];
	IplImage* s_plane=planes[1];
	IplImage* v_plane=planes[2];

	int width=h_plane->width; 
	int height=h_plane->height;

	char* p_quantized=quantized->imageData;
	int wstep=quantized->widthStep;

#ifdef DEBUG_QUANTISITION
	int bin[32];
	for(int m=0;m<32;m++)
	{
		bin[m]=0;
	}
#endif

	for(int i=0;i<height;i++)
	{
		for(int j=0;j<width;j++)
		{
			//* h:0-179; s:0-255; v:0-255 */
			//* "(uchar*)" is necessary */
			int h=*((uchar*)(h_plane->imageData+i*h_plane->widthStep+j));
			int s=*((uchar*)(s_plane->imageData+i*s_plane->widthStep+j));
			int v=*((uchar*)(v_plane->imageData+i*v_plane->widthStep+j));

			if(v<=0.1*255)
			{
				p_quantized[i*wstep+j]=0;
#ifdef DEBUG_QUANTISITION
				bin[0]++;
#endif

			}
			else if((s<=0.1*255)&&(v>0.1*255)&&(v<=0.4*255))
			{
				p_quantized[i*wstep+j]=1;
#ifdef DEBUG_QUANTISITION
				bin[1]++;
#endif
			}
			else if((s<=0.1*255)&&(v>0.4*255)&&(v<=0.7*255))
			{
				p_quantized[i*wstep+j]=2;
#ifdef DEBUG_QUANTISITION
				bin[2]++;
#endif
			}
			else if((s<=0.1*255)&&(v>0.7*255)&&(v<=1*255))
			{
				p_quantized[i*wstep+j]=3;
#ifdef DEBUG_QUANTISITION
				bin[3]++;
#endif
			}
			else if((h>=0.0/360.0*180 && h<=20.0/360.0*180) || (h>330.0/360.0*180 && h<360.0/360.0*180)) //* "int/360" would always return 0 when int<360 */
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=4;
#ifdef DEBUG_QUANTISITION
					bin[4]++;
#endif
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=5;
#ifdef DEBUG_QUANTISITION
					bin[5]++;
#endif
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					p_quantized[i*wstep+j]=6;
#ifdef DEBUG_QUANTISITION
					bin[6]++;
#endif
				}
				else
				{
					p_quantized[i*wstep+j]=7;
#ifdef DEBUG_QUANTISITION
					bin[7]++;
#endif
				}
			}
			else if(h>20.0/360.0*180&&h<=45.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=8;
#ifdef DEBUG_QUANTISITION
					bin[8]++;
#endif
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=9;
#ifdef DEBUG_QUANTISITION
					bin[9]++;
#endif
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					p_quantized[i*wstep+j]=10;
#ifdef DEBUG_QUANTISITION
					bin[10]++;
#endif
				}
				else
				{
					p_quantized[i*wstep+j]=11;
#ifdef DEBUG_QUANTISITION
					bin[11]++;
#endif
				}
			}
			else if(h>45.0/360.0*180&&h<=75.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=12;
#ifdef DEBUG_QUANTISITION
					bin[12]++;
#endif
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=13;
#ifdef DEBUG_QUANTISITION
					bin[13]++;
#endif
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					p_quantized[i*wstep+j]=14;
#ifdef DEBUG_QUANTISITION
					bin[14]++;
#endif
				}
				else
				{
					p_quantized[i*wstep+j]=15;
#ifdef DEBUG_QUANTISITION
					bin[15]++;
#endif
				}
			}
			else if(h>75.0/360.0*180&&h<=155.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=16;
#ifdef DEBUG_QUANTISITION
					bin[16]++;
#endif
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=17;
#ifdef DEBUG_QUANTISITION
					bin[17]++;
#endif
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					p_quantized[i*wstep+j]=18;
#ifdef DEBUG_QUANTISITION
					bin[18]++;
#endif
				}
				else
				{
					p_quantized[i*wstep+j]=19;
#ifdef DEBUG_QUANTISITION
					bin[19]++;
#endif
				}
			}
			else if(h>155.0/360.0*180&&h<=210.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=20;
#ifdef DEBUG_QUANTISITION
					bin[20]++;
#endif
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=21;
#ifdef DEBUG_QUANTISITION
					bin[21]++;
#endif
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					p_quantized[i*wstep+j]=22;
#ifdef DEBUG_QUANTISITION
					bin[22]++;
#endif
				}
				else
				{
					p_quantized[i*wstep+j]=23;
#ifdef DEBUG_QUANTISITION
					bin[23]++;
#endif
				}
			}
			else if(h>210.0/360.0*180&&h<=270.0/360.0*180)
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=24;
#ifdef DEBUG_QUANTISITION
					bin[24]++;
#endif
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=25;
#ifdef DEBUG_QUANTISITION
					bin[25]++;
#endif
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					p_quantized[i*wstep+j]=26;
#ifdef DEBUG_QUANTISITION
					bin[26]++;
#endif
				}
				else
				{
					p_quantized[i*wstep+j]=27;
#ifdef DEBUG_QUANTISITION
					bin[27]++;
#endif
				}
			}
			else
			{
				if(s<=0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=28;
#ifdef DEBUG_QUANTISITION
					bin[28]++;
#endif
				}
				else if(s>0.5*255&&v<=0.5*255)
				{
					p_quantized[i*wstep+j]=29;
#ifdef DEBUG_QUANTISITION
					bin[29]++;
#endif
				}
				else if(s<=0.5*255&&v>0.5*255)
				{
					p_quantized[i*wstep+j]=30;
#ifdef DEBUG_QUANTISITION
					bin[30]++;
#endif
				}
				else
				{
					p_quantized[i*wstep+j]=31;
#ifdef DEBUG_QUANTISITION
					bin[31]++;
#endif
				}
			}
		}
	}
#ifdef DEBUG_QUANTISITION
	for(int m=0;m<32;m++)
	{
		cout<<m<<" :"<<bin[m]<<endl;
	}
#endif
}