
// Image Retrieval SystemDlg.h : 头文件

#pragma once
#include "ImagePreProcessFunc.h"
#include "Feature.h"
#include <fstream>
#include "afxwin.h"

//此处像素值应该与屏幕分辨率相关 
#define uiImageHeight 180
#define uiImageWidth 180
#define uiResultHeight 120
#define uiResultWidth 120
#define IMAGE_CHANNEL_NUM 3

// CImageRetrievalSystemDlg 对话框
class CIRSystemDlg : public CDialogEx
{
// 构造
public:
	CIRSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

	Feature sampleImgFeature;

	bool isImageLibReady;
	bool isImageSampleReady;

// 对话框数据
	enum { IDD = IDD_IMAGERETRIEVALSYSTEM_DIALOG };

	CString uiImagePath;
	CString uiImageLibPath;
	CString uiFeatureFilePath;

	CStatic uiImagePic;
	CButton uiExtractButton;
	CButton uiSearchButton;
	CButton uiNextPageButton;
	CButton uiFrontPageButton;

	CButton uiMethodRadio;

	IplImage*  image;
	IplImage*  result1;
	IplImage*  result2;
	IplImage*  result3;
	IplImage*  result4;
	IplImage*  result5;
	IplImage*  result6;
	IplImage*  result7;
	IplImage*  result8;
	IplImage*  result9;

	int currentPage;
	int totalPage;
	int totalImgNum;
	vector<CString> imageLibVector;
	int* idx_min2max ;//存储相应的索引

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void ResizeImage1(IplImage* img);
	void ResizeImage2(IplImage* img, IplImage* myImg);
	void ShowImage( IplImage* img, unsigned int uiID ) ;
	//void scanDir(CString dirPath, ofstream& imgDBfeaturesFile);
	 void scanDir(CString dirPath, vector<CString> & imageLibVector);

	afx_msg void OnBnClickedButtonChooseImage();
	afx_msg void OnBnClickedButtonChooseImageDB();
	afx_msg void OnBnClickedButtonFeatureExtraction();
	afx_msg void OnBnClickedButtonLoadFeatureFile();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonFrontPage();
	afx_msg void OnBnClickedButtonNextPage();
	//afx_msg void OnBnClickedRadioColor();
	//afx_msg void OnBnClickedRadioTexture();
	//afx_msg void OnBnClickedRadioGradient();
	//afx_msg void OnBnClickedRadioColortexture();

};
