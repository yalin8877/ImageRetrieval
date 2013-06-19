
// Image Retrieval SystemDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Image Retrieval System.h"
#include "Image Retrieval SystemDlg.h"
#include "afxdialogex.h"
#include <string>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>
#include "CvvImage.h"
#include "io.h"

using namespace std;
using namespace cv;


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImageRetrievalSystemDlg 对话框

CIRSystemDlg::CIRSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CIRSystemDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIRSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ImagePath, uiImagePath);  
	DDX_Text(pDX, IDC_ImageDBPath, uiImageLibPath);   
	DDX_Text(pDX, IDC_FeatureFilePath, uiFeatureFilePath); 
	DDX_Control(pDX, IDC_ImagePic, uiImagePic); 
	DDX_Control(pDX,IDC_BUTTON_FeatureExtraction, uiExtractButton);
	DDX_Control(pDX,IDC_BUTTON_Search, uiSearchButton);
	DDX_Control(pDX,IDC_BUTTON_FrontPage, uiFrontPageButton);
	DDX_Control(pDX,IDC_BUTTON_NextPage, uiNextPageButton);
	DDX_Control(pDX, IDC_RADIO_COLORTEXTURE, uiMethodRadio);
}

BEGIN_MESSAGE_MAP(CIRSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ChooseImage, &CIRSystemDlg::OnBnClickedButtonChooseImage)
	ON_BN_CLICKED(IDC_BUTTON_ChooseImageDB, &CIRSystemDlg::OnBnClickedButtonChooseImageDB)
	ON_BN_CLICKED(IDC_BUTTON_FeatureExtraction, &CIRSystemDlg::OnBnClickedButtonFeatureExtraction)
	ON_BN_CLICKED(IDC_BUTTON_LoadFeatureFile, &CIRSystemDlg::OnBnClickedButtonLoadFeatureFile)
	ON_BN_CLICKED(IDC_BUTTON_Search, &CIRSystemDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_FrontPage, &CIRSystemDlg::OnBnClickedButtonFrontPage)
	ON_BN_CLICKED(IDC_BUTTON_NextPage, &CIRSystemDlg::OnBnClickedButtonNextPage)
	//ON_BN_CLICKED(IDC_RADIO_COLOR, &CIRSystemDlg::OnBnClickedRadioColor)
	//ON_BN_CLICKED(IDC_RADIO_TEXTURE, &CIRSystemDlg::OnBnClickedRadioTexture)
	//ON_BN_CLICKED(IDC_RADIO_GRADIENT, &CIRSystemDlg::OnBnClickedRadioGradient)
	//ON_BN_CLICKED(IDC_RADIO_COLORTEXTURE, &CIRSystemDlg::OnBnClickedRadioColortexture)
END_MESSAGE_MAP()


// CImageRetrievalSystemDlg 消息处理程序

BOOL CIRSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CvSize size;
	size.height = uiImageHeight;//IMAGE_HEIGHT;
	size.width = uiImageWidth;//IMAGE_WIDTH;
	image = cvCreateImage( size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );

	size.height = uiResultHeight;
	size.width = uiResultWidth;
	result1 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result2 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result3 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result4 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result5 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result6 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result7 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result8 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result9 = cvCreateImage(size, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );

	/*m_flag_edit1 = false;
	m_flag_edit2 = false;*/

	CDialog::UpdateWindow();
	// 更新windows窗口，如果无这步调用，图片显示还会出现问题
	ShowImage( image, IDC_ImagePic );
	ShowImage( result1, IDC_Result1 );
	ShowImage( result2, IDC_Result2 );
	ShowImage( result3, IDC_Result3 );
	ShowImage( result4, IDC_Result4 );
	ShowImage( result5, IDC_Result5 );
	ShowImage( result6, IDC_Result6 );
	ShowImage( result7, IDC_Result7 );
	ShowImage( result8, IDC_Result8 );
	ShowImage( result9, IDC_Result9 );

	isImageLibReady=false;
	isImageSampleReady=false;

	uiExtractButton.EnableWindow(false);
	uiSearchButton.EnableWindow(false);
	uiNextPageButton.EnableWindow(false);
	uiFrontPageButton.EnableWindow(false);

	uiMethodRadio.SetCheck(true);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CIRSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CIRSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CIRSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CIRSystemDlg::ResizeImage1(IplImage* img)
{
	// 读取图片的宽和高
	int w = img->width;
	int h = img->height;

	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;

	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = (float)((float)max/(float)uiImageHeight);

	// 缩放后图片的宽和高
	int nw = (int)(w/scale);
	int nh = (int)(h/scale);

	// 为了将缩放后的图片存入 image 的正中部位，需计算图片在 image 左上角的期望坐标值
	int tlx = (nw > nh)?0:(int)(uiImageHeight-nw)/2;
	int tly = (nw > nh)?(int)(uiImageHeight-nh)/2:0;

	// 设置 image 的 ROI 区域，用来存入图片 img
	cvSetImageROI(image, cvRect(tlx, tly, nw, nh));

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize(img, image);

	// 重置 image 的 ROI 准备读入下一幅图片
	cvResetImageROI(image);

}

void CIRSystemDlg::ResizeImage2(IplImage* img, IplImage* myImg)
{
	// 读取图片的宽和高
	int w = img->width;
	int h = img->height;

	// 找出宽和高中的较大值者
	int max = (w > h)? w: h;

	// 计算将图片缩放到TheImage区域所需的比例因子
	float scale = (float)((float)max/(float)uiResultHeight);

	// 缩放后图片的宽和高
	int nw = (int)(w/scale);
	int nh = (int)(h/scale);

	// 为了将缩放后的图片存入 TheImage 的正中部位，需计算图片在 TheImage 左上角的期望坐标值
	int tlx = (nw > nh)? 0: (int)(uiResultHeight-nw)/2;
	int tly = (nw > nh)? (int)(uiResultHeight-nh)/2: 0;

	// 设置 TheImage 的 ROI 区域，用来存入图片 img
	cvSetImageROI( myImg, cvRect( tlx, tly, nw, nh) );

	// 对图片 img 进行缩放，并存入到 TheImage 中
	cvResize( img, myImg );

	// 重置 TheImage 的 ROI 准备读入下一幅图片
	cvResetImageROI( myImg );
}

void CIRSystemDlg::ShowImage( IplImage* img, unsigned int uiID )
{
	// 获得显示控件的 DC
	CDC* pDC = GetDlgItem( uiID ) ->GetDC();
	// 获取 HDC(设备句柄) 来进行绘图操作		
	HDC hDC = pDC ->GetSafeHdc();				

	CRect rect;
	GetDlgItem(uiID) ->GetClientRect( &rect );
	// 求出图片控件的宽和高
	int rw = rect.right - rect.left;			
	int rh = rect.bottom - rect.top;
	// 读取图片的宽和高
	int iw = img->width;	
	int ih = img->height;
	// 使图片的显示位置正好在控件的正中
	int tx = (int)(rw - iw)/2;	
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );
	// 复制图片
	CvvImage cimg;
	cimg.CopyOf( img );
	// 将图片绘制到显示控件的指定区域内	
	cimg.DrawToHDC( hDC, &rect );	

	ReleaseDC( pDC );
}

void CIRSystemDlg::OnBnClickedButtonChooseImage()
{
	//* load image */
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal()==IDOK)
		FilePathName=dlg.GetPathName();

	IplImage* src=cvLoadImage(FilePathName.GetBuffer(),CV_LOAD_IMAGE_COLOR);
	while(src==NULL)
	{
		MessageBox(TEXT("加载图片失败，请重新选择！"));
		if(dlg.DoModal()==IDOK)
		{
			FilePathName=dlg.GetPathName();
			src=cvLoadImage(FilePathName.GetBuffer(),CV_LOAD_IMAGE_COLOR);
		}
		else
			break;
	}
	if(src!=NULL)
	{
		uiImagePath=FilePathName;
		UpdateData(FALSE);

		// 对上一幅显示的图片数据清零
		if(image)	
			cvZero(image);

		// 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
		ResizeImage1(src);	
		// 调用显示图片函数
		ShowImage(image, IDC_ImagePic);
		// 释放 ipl 占用的内存	
		cvReleaseImage(&src);

		isImageSampleReady=true;
	}
	else
	{
		isImageSampleReady=false;
	}

	if(isImageLibReady==true&&isImageSampleReady==true)
		uiSearchButton.EnableWindow(true);
}

void CIRSystemDlg::OnBnClickedButtonChooseImageDB()
{
	isImageLibReady=false;

	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO bi;
	char Buffer[MAX_PATH];
	//初始化入口参数bi开始
	bi.hwndOwner = NULL;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = Buffer;//此参数如为NULL则不能显示对话框
	bi.lpszTitle = "选择图像库文件夹";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	//bi.iImage=IDI_ICON2;
	//初始化入口参数bi结束
	LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
	if(pIDList)
	{
		//取得文件夹路径到Buffer里
		SHGetPathFromIDList(pIDList, Buffer);
		uiImageLibPath=Buffer;
		UpdateData(FALSE);

		uiExtractButton.EnableWindow(true);
	}
	else
	{
		uiImageLibPath="";
		UpdateData(FALSE);

		uiExtractButton.EnableWindow(false);
	}
	isImageLibReady=false;

	LPMALLOC lpMalloc;
	if(FAILED(SHGetMalloc(&lpMalloc))) return;
	//释放内存
	lpMalloc->Free(pIDList);
	lpMalloc->Release();
}

void CIRSystemDlg::scanDir(CString dirPath, vector<CString> & imageLibVector)
{
	_finddata64i32_t fileInfo;
	CString filename=dirPath+"\\*.*";

	long handle=_findfirst64i32(filename.GetBuffer(),&fileInfo);
	if(handle==-1)
	{
		return;
	}
	do 
	{
		if(strcmp(fileInfo.name,".")==0||strcmp(fileInfo.name,"..")==0)
		{
			continue;
		}
		CString subDirPath=dirPath+"\\"+fileInfo.name;
		if((fileInfo.attrib&_A_SUBDIR)==_A_SUBDIR)
		{
			scanDir(subDirPath,imageLibVector);
		}
		else
		{
			//检测若能打开图片,则加入imageLibVector，反之忽略
			IplImage* test=cvLoadImage(subDirPath,CV_LOAD_IMAGE_COLOR);
			if(test)
			{
				imageLibVector.push_back(subDirPath);
			}
		}
	}
	while(!(_findnext64i32(handle,&fileInfo)));

	_findclose(handle);
}

void CIRSystemDlg::OnBnClickedButtonFeatureExtraction()
{
	imageLibVector.clear();
	scanDir(uiImageLibPath,imageLibVector);
	CString featuresFileName="..\\已生成的特征文件\\ImageDB_features.txt";
	ofstream featuresLib(featuresFileName);
	//filename<<imageLibVector.size()<<endl;

	Feature feature;
	UpdateData(true);

	float* featuredata=new float[COLOR_FEATURE_DIM + TEXTURE_FEATURE_DIM];
	for(int i=0;i<imageLibVector.size();i++)
	{
		featuresLib<<imageLibVector[i]<<endl;
		feature.m_GetFeature(imageLibVector[i],featuredata);
		for (int j=0; j<COLOR_FEATURE_DIM+TEXTURE_FEATURE_DIM; j++)
		{
			featuresLib<<*(featuredata+j)<<endl;
		}
	}
	//imageLibVector.clear();
	featuresLib.close();

	uiFeatureFilePath=featuresFileName;
	UpdateData(FALSE);

	isImageLibReady=true;
	MessageBox(TEXT("特征提取完成！"));
	if(isImageLibReady==true&&isImageSampleReady==true)
		uiSearchButton.EnableWindow(true);
}


void CIRSystemDlg::OnBnClickedButtonLoadFeatureFile()
{
	CString FilePathName;
	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框
	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName();
		ifstream imageLibFeatures(FilePathName);
		if(!imageLibFeatures)
		{
			MessageBox(TEXT("加载失败，请检查文件格式是否正确！"));
			isImageLibReady=false;
		}
		else
		{
			imageLibVector.clear();
			char temp[MAX_PATH];
			while(!imageLibFeatures.eof())
			{
				imageLibFeatures.getline(temp,MAX_PATH);
				imageLibVector.push_back(temp);
				for(int i=0;i<COLOR_FEATURE_DIM + TEXTURE_FEATURE_DIM;i++)
				{
					imageLibFeatures.getline(temp,MAX_PATH);
				}
				
			}
			uiFeatureFilePath=FilePathName;
			UpdateData(FALSE);
			isImageLibReady=true;

			if(isImageLibReady==true&&isImageSampleReady==true)
				uiSearchButton.EnableWindow(true);
		}
		imageLibFeatures.close();
	}
}

void CIRSystemDlg::OnBnClickedButtonSearch()
{
	Feature feature;
	UpdateData(TRUE);
    
	float* imageFeature = new float[COLOR_FEATURE_DIM + TEXTURE_FEATURE_DIM];
	feature.m_GetFeature(uiImagePath, imageFeature);

	ifstream featuresFile(uiFeatureFilePath);
	//ofstream test("C:\\Users\\zhang yalin\\Desktop\\test.txt");

	char libImagePath[MAX_PATH];
	float* libImageFeature = new float[COLOR_FEATURE_DIM + TEXTURE_FEATURE_DIM];

	float* distances=new float[imageLibVector.size()]();  
	char* temp=new char[MAX_PATH];
	for (int i=0; i<imageLibVector.size(); i++)
	{
		featuresFile.getline(libImagePath,MAX_PATH);
		//test<<libImagePath<<endl;

		//featuresFile.getline((char*)featuresLine,(COLOR_FEATURE_DIM + TEXTURE_FEATURE_DIM)*sizeof(float));
		for(int j=0;j<COLOR_FEATURE_DIM + TEXTURE_FEATURE_DIM;j++)
		{
			featuresFile >> libImageFeature[j];
			//featuresFile.getline(temp,MAX_PATH);
			//memcpy(&libImageFeature[j],featuresLine+j,sizeof(float));
			//featuresFile.getline(libImagePath,MAX_PATH);
			//test<<libImageFeature[j]<<endl;
		}
		featuresFile.getline(libImagePath,MAX_PATH);
		distances[i]=feature.m_ManhattanDis(imageFeature, libImageFeature, COLOR_FEATURE_DIM+TEXTURE_FEATURE_DIM);
	}
	featuresFile.close();

	//对得到的距离进行排序，由小到大
	idx_min2max =new int[imageLibVector.size()]();               
	int* flag = new int[imageLibVector.size()]();                                     
	int minIdx = 0;
	for (int j=0; j<imageLibVector.size(); j++)
	{ 
		for (int i1=0; i1<imageLibVector.size(); i1++)
		{
		    if (flag[i1] == 0)
		    {
				minIdx = i1;
				break;
		    }
		}
		for (int i2=0; i2<imageLibVector.size(); i2++)
		{
			if (0 == flag[i2])
			{
				if (distances[i2]<distances[minIdx])
				{
					minIdx = i2;
				}
			}
			
		}
		flag[minIdx] = 1;
		idx_min2max[j] = minIdx;
	}

	//显示结果
	int outputNum = imageLibVector.size()>9? 9:imageLibVector.size();
  
	currentPage = 1;
	totalImgNum=imageLibVector.size();
	totalPage=totalImgNum/9;
	if ( 0!=(totalImgNum%9) )
	{
		totalPage ++;
	}

	if(imageLibVector.size() > 9)
		uiNextPageButton.EnableWindow(TRUE);

   	CString pageShow;
	pageShow.Format("%d/%d",currentPage,totalPage);
	GetDlgItem(IDC_PageNum)->SetWindowText(pageShow);

   	for (int i=0; i<outputNum; i++)
   	{
	   	// 重绘对话框
		CDialog::UpdateWindow();
		// 读取图片,缓存到一个局部变量 ipl 中
		IplImage* result = cvLoadImage( imageLibVector.at(idx_min2max[i]), CV_LOAD_IMAGE_COLOR );

		// 判断是否成功读取图片	
		if( ! result )			
			return;

		switch (i)
		{
		case 0:
			// 对上一幅显示的图片数据清零
			cvZero(result1);
			// 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
			ResizeImage2( result, result1 );	
			// 调用显示图片函数
			ShowImage( result1, IDC_Result1);
			break;
		case 1:
			cvZero(result2);
			ResizeImage2( result, result2 );	
			ShowImage( result2, IDC_Result2 );
			break;
		case 2:
			cvZero(result3);
			ResizeImage2( result, result3 );	
			ShowImage( result3, IDC_Result3 );
			break;
		case 3:
			cvZero(result4);
			ResizeImage2( result, result4 );	
			ShowImage( result4, IDC_Result4 );
			break;
		case 4:
			cvZero(result5);
			ResizeImage2( result, result5 );	
			ShowImage( result5, IDC_Result5 );
			break;
		case 5:
			cvZero(result6);
			ResizeImage2( result, result6 );	
			ShowImage( result6, IDC_Result6 );
			break;
		case 6:
			cvZero(result7);
			ResizeImage2( result, result7 );	
			ShowImage( result7, IDC_Result7 );
			break;
		case 7:
			cvZero(result8);
			ResizeImage2( result, result8 );	
			ShowImage( result8, IDC_Result8 );
			break;
		case 8:
			cvZero(result9);
			ResizeImage2( result, result9 );	
			ShowImage( result9, IDC_Result9 );
			break;
		}
		// 释放 result 占用的内存	
		cvReleaseImage( & result );
   	}

	delete [] libImageFeature;
	delete [] flag;
	delete [] distances;
	delete [] imageFeature;
	delete [] temp;
}


void CIRSystemDlg::OnBnClickedButtonFrontPage()
{
	// TODO: 在此添加控件通知处理程序代码
	CvSize ImgSize;
	ImgSize.height = uiResultHeight;
	ImgSize.width = uiResultWidth;
	result1 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result2 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result3 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result4 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result5 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result6 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result7 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result8 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result9 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );

	// 更新windows窗口，如果无这步调用，图片显示还会出现问题
	CDialog::UpdateWindow();
	ShowImage( result1, IDC_Result1 );
	ShowImage( result2, IDC_Result2 );
	ShowImage( result3, IDC_Result3 );
	ShowImage( result4, IDC_Result4 );
	ShowImage( result5, IDC_Result5 );
	ShowImage( result6, IDC_Result6 );
	ShowImage( result7, IDC_Result7 );
	ShowImage( result8, IDC_Result8 );
	ShowImage( result9, IDC_Result9 );

	// TODO: 在此添加控件通知处理程序代码
	int start = (currentPage-2)*9;
	int end = start + 9;

	for (int i=start; i<end; i++)
	{
		// 重绘对话框
		CDialog::UpdateWindow();
		// 读取图片,缓存到一个局部变量 ipl 中
		IplImage* result = cvLoadImage( imageLibVector.at(idx_min2max[i]), CV_LOAD_IMAGE_COLOR );

		// 判断是否成功读取图片	
		if( ! result )			
			return;

		switch (i%9)
		{
		case 0:
			// 对上一幅显示的图片数据清零
			if( result1 )	
				cvZero(result1);
			// 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
			ResizeImage2( result, result1 );	
			// 调用显示图片函数
			ShowImage( result1, IDC_Result1);
			break;
		case 1:
			if( result2 )	
				cvZero(result2);
			ResizeImage2( result, result2 );	
			ShowImage( result2, IDC_Result2 );
			break;
		case 2:
			if( result3 )	
				cvZero(result3);
			ResizeImage2( result, result3 );	
			ShowImage( result3, IDC_Result3 );
			break;
		case 3:
			if( result4 )	
				cvZero(result4);
			ResizeImage2( result, result4 );	
			ShowImage( result4, IDC_Result4 );
			break;
		case 4:
			if( result5 )	
				cvZero(result5);
			ResizeImage2( result, result5 );	
			ShowImage( result5, IDC_Result5 );
			break;
		case 5:
			if( result6 )	
				cvZero(result6);
			ResizeImage2( result, result6 );	
			ShowImage( result6, IDC_Result6 );
			break;
		case 6:
			if( result7 )	
				cvZero(result7);
			ResizeImage2( result, result7 );	
			ShowImage( result7, IDC_Result7 );
			break;
		case 7:
			if( result8 )	
				cvZero(result8);
			ResizeImage2( result, result8 );	
			ShowImage( result8, IDC_Result8 );
			break;
		case 8:
			if( result9 )	
				cvZero(result9);
			ResizeImage2( result, result9 );	
			ShowImage( result9, IDC_Result9 );
			break;
		}
		// 释放 result 占用的内存	
		cvReleaseImage( & result );
	}
	// 更新windows窗口，如果无这步调用，图片显示还会出现问题
	CDialog::UpdateWindow();
	UpdateData(false);

	currentPage -= 1;
	CString pageShow;
	pageShow.Format("%d/%d",currentPage,totalPage);
	GetDlgItem(IDC_PageNum)->SetWindowText(pageShow);
	if (1 == currentPage)
	{
		uiFrontPageButton.EnableWindow(FALSE);
	}

	if (currentPage < totalPage)
	{
		uiNextPageButton.EnableWindow(TRUE);
	}
}


void CIRSystemDlg::OnBnClickedButtonNextPage()
{
	// TODO: 在此添加控件通知处理程序代码
	CvSize ImgSize;
	ImgSize.height = uiResultHeight;
	ImgSize.width = uiResultWidth;
	result1 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result2 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result3 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result4 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result5 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result6 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result7 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result8 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );
	result9 = cvCreateImage(ImgSize, IPL_DEPTH_8U, IMAGE_CHANNEL_NUM );

	// 更新windows窗口，如果无这步调用，图片显示还会出现问题
	CDialog::UpdateWindow();
	ShowImage( result1, IDC_Result1 );
	ShowImage( result2, IDC_Result2 );
	ShowImage( result3, IDC_Result3 );
	ShowImage( result4, IDC_Result4 );
	ShowImage( result5, IDC_Result5 );
	ShowImage( result6, IDC_Result6 );
	ShowImage( result7, IDC_Result7 );
	ShowImage( result8, IDC_Result8 );
	ShowImage( result9, IDC_Result9 );

	// TODO: 在此添加控件通知处理程序代码
	int start = currentPage*9;
	int end = start + 9;
	if (end > totalImgNum)
	{
		end = totalImgNum;
	}
	for (int i=start; i<end; i++)
	{
		// 重绘对话框
		CDialog::UpdateWindow();
		// 读取图片,缓存到一个局部变量 ipl 中
		IplImage* result = cvLoadImage( imageLibVector.at(idx_min2max[i]), CV_LOAD_IMAGE_COLOR );

		// 判断是否成功读取图片	
		if( ! result )			
			return;

		switch (i%9)
		{
		case 0:
			// 对上一幅显示的图片数据清零
			if( result1 )	
				cvZero(result1);
			// 对读入的图片进行缩放，使其宽或高最大值者刚好等于 256，再复制到 TheImage 中
			ResizeImage2( result, result1 );	
			// 调用显示图片函数
			ShowImage( result1, IDC_Result1);
			break;
		case 1:
			if( result2 )	
				cvZero(result2);
			ResizeImage2( result, result2 );	
			ShowImage( result2, IDC_Result2 );
			break;
		case 2:
			if( result3 )	
				cvZero(result3);
			ResizeImage2( result, result3 );	
			ShowImage( result3, IDC_Result3 );
			break;
		case 3:
			if( result4 )	
				cvZero(result4);
			ResizeImage2( result, result4 );	
			ShowImage( result4, IDC_Result4 );
			break;
		case 4:
			if( result5 )	
				cvZero(result5);
			ResizeImage2( result, result5 );	
			ShowImage( result5, IDC_Result5 );
			break;
		case 5:
			if( result6 )	
				cvZero(result6);
			ResizeImage2( result, result6 );	
			ShowImage( result6, IDC_Result6 );
			break;
		case 6:
			if( result7 )	
				cvZero(result7);
			ResizeImage2( result, result7 );	
			ShowImage( result7, IDC_Result7 );
			break;
		case 7:
			if( result8 )	
				cvZero(result8);
			ResizeImage2( result, result8 );	
			ShowImage( result8, IDC_Result8 );
			break;
		case 8:
			if( result9 )	
				cvZero(result9);
			ResizeImage2( result, result9 );	
			ShowImage( result9, IDC_Result9 );
			break;
		}
		// 释放 result 占用的内存	
		cvReleaseImage( & result );
	}
	// 更新windows窗口，如果无这步调用，图片显示还会出现问题
	CDialog::UpdateWindow();
	UpdateData(false);

	currentPage += 1;

	CString pageShow;
	pageShow.Format("%d/%d",currentPage,totalPage);
	GetDlgItem(IDC_PageNum)->SetWindowText(pageShow);

	if (1 != currentPage)
	{
		uiFrontPageButton.EnableWindow(TRUE);
	}
	if (currentPage >= totalPage)
	{
		uiNextPageButton.EnableWindow(FALSE);
	}
}

//void CIRSystemDlg::OnBnClickedRadioColor()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if(MB_OK==MessageBox(TEXT("更改检索参数需对图像库重新进行特征提取！是否继续？"),"提示!",MB_OKCANCEL))
//	{
//		isImageLibReady=false;
//		uiSearchButton.EnableWindow(FALSE);
//		#define FEATURE_TYPE COLOR
//	}
//}
//
//
//void CIRSystemDlg::OnBnClickedRadioTexture()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if(MB_OK==MessageBox(TEXT("更改检索参数需对图像库重新进行特征提取！是否继续？"),"提示!",MB_OKCANCEL))
//	{
//		isImageLibReady=false;
//		uiSearchButton.EnableWindow(FALSE);
//		#define FEATURE_TYPE TEXTURE
//	}
//}
//
//
//void CIRSystemDlg::OnBnClickedRadioGradient()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if(MB_OK==MessageBox(TEXT("更改检索参数需对图像库重新进行特征提取！是否继续？"),"提示!",MB_OKCANCEL))
//	{
//		isImageLibReady=false;
//		uiSearchButton.EnableWindow(FALSE);
//		#define FEATURE_TYPE GRADIENT
//	}
//}
//
//
//void CIRSystemDlg::OnBnClickedRadioColortexture()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	if(MB_OK==MessageBox(TEXT("更改检索参数需对图像库重新进行特征提取！是否继续？"),"提示!",MB_OKCANCEL))
//	{
//		isImageLibReady=false;
//		uiSearchButton.EnableWindow(FALSE);
//		#define FEATURE_TYPE COLOR_TEXTURE
//	}
//}
