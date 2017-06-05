// SCR_OpenCV.cpp : Defines the exported functions for the DLL application.
#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#define _CRT_SECURE_NO_DEPRECATE
#include <cv.h>
#define _CRT_SECURE_NO_DEPRECATE
#include <cxcore.h>
#define _CRT_SECURE_NO_DEPRECATE
#include <highgui.h>
#define _CRT_SECURE_NO_DEPRECATE
#include "iostream"

using namespace std;
using namespace cv;

extern "C"
{
    __declspec(dllexport) int Demo(LPCWSTR);
    __declspec(dllexport) bool Process(LPCWSTR, LPCWSTR);
}


extern "C" int __cdecl Demo(LPCWSTR filename)
{
    IplImage* frame = cvLoadImage((char*)filename, 1);

    cvSmooth(frame, frame, CV_BLUR, 3, 0, 2.0, 2.0);

    if (frame == NULL){ 
        printf("File does not exist!");
        exit(0);
    }

    cvNamedWindow("Hi", CV_WINDOW_AUTOSIZE);
    cvShowImage("My Window", frame);
    cvSaveImage("image.jpg", frame);

    cvWaitKey();
    cvDestroyWindow("Hi");

    return 0;
}
/*
extern "C" int __cdecl Process(LPCWSTR filename_tr, LPCWSTR filename_ex)
{
    //Image to recognize

    //cvThreshold
    IplImage *src = 0, *dst = 0, *dst2 = 0;
    src = cvLoadImage((char*)filename_tr, 0);
    assert(src != 0);

    dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
    cvThreshold(src, dst, 125, 250, CV_THRESH_BINARY);

    //cvFindContours
    CvSeq* contours_tr = 0;
    CvMemStorage* storage = cvCreateMemStorage(0);

    cvFindContours(dst, storage, &contours_tr, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

    //Image with vocabulary

    //cvThreshold
    src = cvLoadImage((char*)filename_ex, 0);
    assert(src != 0);

    dst = cvCreateImage(cvSize(src->width, src->height), IPL_DEPTH_8U, 1);
    cvThreshold(src, dst, 125, 250, CV_THRESH_BINARY);

    //cvFindContours
    CvSeq* contours_ex = 0;
    storage = cvCreateMemStorage(0);

    cvFindContours(dst, storage, &contours_ex, sizeof(CvContour), CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

    //Compare
    //for each (CvSeq var in &contours_tr)
    //{

    //}


    /* Hmm
    CvSeq* contoursI = 0;
    int contoursCont = cvFindContours(dst, storage, &contoursI, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

    //! matches two contours using one of the available algorithms
    //CV_EXPORTS_W double matchShapes(InputArray contour1, InputArray contour2, int method, double parameter);

    // обходим контуры изображения 
    CvSeq* seqM = 0;
    double matchM = 0.9;
    int counter = 0;
    
    //if (contoursI != 0){
    //    
        // поиск лучшего совпадения контуров по их моментам 

    for (CvSeq* seqTR = contours_tr; seqTR != 0; seqTR = seqTR->h_next)
        for (CvSeq* seqEX = contours_ex; seqEX != 0; seqEX = seqEX->h_next)
        {        
            printf("%d %d %d \n", seqTR, seqEX);
            double match0 = cvMatchShapes(seqTR, seqEX, CV_CONTOURS_MATCH_I3);
            
            if (match0 >= matchM) 
                printf("[i] %d match: %.2f\n", ++counter, match0);
            else
                printf("[i] %d NO match: %.2f\n", ++counter, match0);
            
        }
    //}
    

    //делаем CV…TREE т.к. нам нужно получить внешние и внутренние контуры

    IplImage* cnt_img = cvCreateImage(cvSize(src->width, src->height), 8, 3);
    CvSeq* _contours = contours_tr;
    _contours->h_next = 0;
    CvSeq* h_next;
    CvSeq* contours1 = _contours->v_next;

    CvMoments moments;

    char buf[256];

    int i = 0;
    do{
        cvZero(cnt_img);
        h_next = contours1->h_next;
        contours1->h_next = 0;
       
        //считаем моменты здесь
        //cvCvtColor(cnt_img, image1, CV_BGR2GRAY);

        //Можно было обойтись без преобразования цвета, пименив binary
        cvMoments(image1, &moments);

        //moments.m00, moments.m01, moments.m02, moments.m03, moments.m10, moments.m11, moments.m12, moments.m20, moments.m21, moments.m30

        contours1 = h_next;
        i++;
        if (h_next == NULL) break;
    } while (true);






    cvNamedWindow("cvProcess", 1);
    cvShowImage("cvProcess", dst);

    cvNamedWindow("cvProcess2", 1);
    cvShowImage("cvProcess2", src);

    cvWaitKey(0);
    cvDestroyAllWindows();

    
    return 0;
}*/

extern "C" bool __cdecl Process(LPCWSTR filename_tr, LPCWSTR filename_ex)
{
    bool found = false;

    IplImage *original = 0, *templ = 0;
    original = cvLoadImage((char*)filename_tr, 0);
    templ = cvLoadImage((char*)filename_ex, 0);

    assert(original != 0);
    assert(templ != 0);

    IplImage *src = 0, *dst = 0;

    src = cvCloneImage(original);

    IplImage* binI = cvCreateImage(cvGetSize(src), 8, 1);
    IplImage* binT = cvCreateImage(cvGetSize(templ), 8, 1);

    IplImage* rgb = cvCreateImage(cvGetSize(original), 8, 3);
    cvConvertImage(src, rgb, CV_GRAY2BGR);
    IplImage* rgbT = cvCreateImage(cvGetSize(templ), 8, 3);
    cvConvertImage(templ, rgbT, CV_GRAY2BGR);

    cvCanny(src, binI, 50, 200);
    cvCanny(templ, binT, 50, 200);

    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contoursI = 0, *contoursT = 0;

    int contoursCont = cvFindContours(binI, storage, &contoursI, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

    CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0);
    char buf[128];
    int counter = 0;

    if (contoursI != 0){
        for (CvSeq* seq0 = contoursI; seq0 != 0; seq0 = seq0->h_next){
            cvDrawContours(rgb, seq0, CV_RGB(255, 216, 0), CV_RGB(0, 0, 250), 0, 1, 8);
        }
    }

    cvConvertImage(src, rgb, CV_GRAY2BGR);

    cvFindContours(binT, storage, &contoursT, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0, 0));

    CvSeq* seqT = 0;
    double perimT = 0;

    if (contoursT != 0){
        for (CvSeq* seq0 = contoursT; seq0 != 0; seq0 = seq0->h_next){
            double perim = cvContourPerimeter(seq0);
            if (perim>perimT){
                perimT = perim;
                seqT = seq0;
            }
            cvDrawContours(rgbT, seq0, CV_RGB(255, 216, 0), CV_RGB(0, 0, 250), 0, 1, 8); // рисуем контур
        }
    }
    cvDrawContours(rgbT, seqT, CV_RGB(52, 201, 36), CV_RGB(36, 201, 197), 0, 2, 8); // рисуем контур

    CvSeq* seqM = 0;
    double matchM = 0.5;

    counter = 0;
    if (contoursI != 0){
        for (CvSeq* seq0 = contoursI; seq0 != 0; seq0 = seq0->h_next){
            double match0 = cvMatchShapes(seq0, seqT, CV_CONTOURS_MATCH_I3);
            if (match0<matchM){
                found = true;
            }
        }
    }

    cvReleaseMemStorage(&storage);

    cvReleaseImage(&src);
    cvReleaseImage(&dst);
    cvReleaseImage(&rgb);
    cvReleaseImage(&rgbT);
    cvReleaseImage(&binI);
    cvReleaseImage(&binT);

    return found;
}
