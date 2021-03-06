#include <jni.h>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

vector<Vec3f> circles;
Point center;
int radius;

Mat temp;

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opencv_12_MainActivity_ConvertRGBtoGray(JNIEnv *env, jobject thiz,
                                                         jlong mat_addr_input,
                                                         jlong mat_addr_result) {
    Mat &matInput = *(Mat *)mat_addr_input;
    Mat &matResult = *(Mat *)mat_addr_result;

    cvtColor(matInput, matResult, COLOR_RGBA2GRAY);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_opencv_12_MainActivity_makecircle(JNIEnv *env, jobject thiz, jlong mat_addr_input,
                                                   jlong mat_addr_result, int cnt) {
    Mat &matInput = *(Mat *)mat_addr_input;
    Mat &matResult = *(Mat *)mat_addr_result;

//    vector<Vec3f> circles;
//    Point center;
//    int radius;

    if(cnt == 1){
        radius = -1;

        matResult = matInput;
        int row, col;
        row = matInput.rows;
        col = matInput.cols;
        Mat img_gray;

        cvtColor(matInput, img_gray, COLOR_RGBA2GRAY);
        resize(img_gray, img_gray, Size(240,135), 0, 0, INTER_LINEAR_EXACT);
//    resize(matResult, matResult, Size(240,135), 0, 0, INTER_LINEAR_EXACT);
        medianBlur(img_gray, img_gray, 5);




        HoughCircles(img_gray, circles, HOUGH_GRADIENT, 1,20,50,35,0,0);




        for (size_t i = 0; i < circles.size(); i++) {
            Vec3i c = circles[i];
            if (radius < c[2]) {
                center = Point(c[0], c[1]);
                radius = c[2];
            }
        }
//        if(radius>-1)
//            circle(matResult, center*8, radius*8, Scalar(0, 0, 255), 3);
    }

    if(radius>-1)
        circle(matResult, center*8, radius*8, Scalar(0, 0, 255), 3);

//    resize(matResult, matResult, Size(col,row), 0, 0, INTER_LINEAR_EXACT);

}extern "C"
JNIEXPORT int JNICALL
Java_com_example_opencv_12_MainActivity_findtext(JNIEnv *env, jobject thiz, jlong mat_addr_input,
                                                 jlong mat_addr_result, jint cnt) {
    Mat &matInput = *(Mat *)mat_addr_input;
    Mat &matResult = *(Mat *)mat_addr_result;
    matResult = matInput;

    if(cnt == 1){

        int row, col;
        row = matInput.rows;
        col = matInput.cols;
        Mat img_gray;

        cvtColor(matInput, img_gray, COLOR_RGBA2GRAY);
        resize(img_gray, img_gray, Size(240,135), 0, 0, INTER_LINEAR_EXACT);
//        medianBlur(img_gray, img_gray, 5);
        morphologyEx(img_gray, img_gray, MORPH_GRADIENT, Mat());
//        morphologyEx(img_gray, img_gray, MORPH_CLOSE, Mat());
        Canny(img_gray, img_gray, 100, 200, 3);
//        medianBlur(img_gray, img_gray, 5);

        vector<vector<Point>> contours;
        vector<Vec4i> hierarchy;
        Rect r;

        findContours(img_gray, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE,Point(0,0));

        for (int i = 0; i< contours.size(); i++)
        {
            if(arcLength(contours[i],true)<50)
                continue;

            r = boundingRect(contours[i]);
            rectangle(matResult, Point(r.x * 8, r.y * 8),  Point((r.x + r.width) * 8, (r.y + r.height)*8), Scalar(0, 0, 255), 3);
            //rectangle(img_gray, Point(r.x, r.y),  Point((r.x + r.width), (r.y + r.height)), Scalar(255, 255, 255), 3);
            temp = matResult;
        }
        return 1;
    }
    else{
        matResult = temp;
    }

    return -1;
}