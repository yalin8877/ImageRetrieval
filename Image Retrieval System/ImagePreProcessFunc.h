#include "StdAfx.h"
#include <cv.h>

using namespace std;





char* ConvertLPWSTRToLPSTR (LPWSTR lpwszStrIn);

//HBITMAP IplIamgeToDIB(IplImage *pImg); 




IplImage** RGB2HSVplanes(IplImage* image);

//void smooth(IplImage** plane);

//* using the method in <<基于色彩量化及索引的图像检索>>,汪华章，何小海，宰文姣，王炜 */
void ColorQuantization(IplImage** planes, IplImage* quantized);
