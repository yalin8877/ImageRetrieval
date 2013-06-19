#ifndef _MYCT_H_
#define _MYCT_H_

#include <vector>
#include <atlstr.h>

#include <cv.h>
#include <highgui.h>
#include "ImagePreProcessFunc.h"

#define COLOR_FEATURE_DIM 128
#define TEXTURE_FEATURE_DIM 30
#define SIFT_FEATURE_DIM 72

class Feature
{
public:
	Feature();

	virtual ~Feature();

	int m_ImageWidth_Color;
	int m_ImageHeight_Color;
	int m_ImageWidth_Gradient;
	int m_ImageHeight_Gradient;
	int m_ImageHeight_Texture;
	int m_ImageWidth_Texture;
  
	float m_gave;
	float m_gvar;
	float m_gske;

	float  ColorCount[4][32];

	float m_GlobalEdgeType[5];
	float m_EdgeType[4][4][5];
	float m_RowsEdgeType[4][5];
	float m_ColumnsEdgeType[4][5];
	float m_2X2EdgeType[5][5];

	void ComputeColorTextureValue(CString filepath, float * featureArr);
	void ComputeColorValue(CString filepath, float * featureArr);
	void ComputeTextureValue(CString filepath, float * featureArr);
	int comsubedgetype(int x,int y,IplImage* myimage);
	void normalize_histogram(float* L_begin, float* L_end);
	void descript(const unsigned char *data, std::vector <float> &vecFloat, int subWidth ,int subHeight);
	void ComputeGradientValue(CString filepath, float * featureArr);
	void m_GetFeature(CString filepath, float* featureArr);

	float m_ManhattanDis(float * featureQry, float * featureLibImg, int dim);
};
#endif