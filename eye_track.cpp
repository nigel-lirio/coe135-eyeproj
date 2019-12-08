#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
using namespace std;
cv::Rect getLeft(std::vector<cv::Rect> &eye_reg){
    //selects left eye
    int left_h = 999999999;
    int left_m = -1;
    for (int i = 0; i < eye_reg.size(); i++)
    {
        //if current left eye coordinate is greater (to the right) then overwrite
        if (left_h > eye_reg[i].tl().x)
        {
            left_h = eye_reg[i].tl().x;
            left_m = i;
        }
        
    }
    return eye_reg[left_m];
    
}
cv::Vec3f getIris(cv::Mat eye, vector<cv::Vec3f> circle_objs){
    //Assumption: IRIS -> Circle with the most black/dark pixels
    vector<int> sum_list(circle_objs.size(), 0);
    //Get Sum of each circles
    for (int i = 0; i < eye.row; i++)
    {
        uchar *point = eye.ptr<uchar>(i);
        for (int k = 0; k < eye.col; k++)
        {
            int val = static_cast<int>(*point);
            for (int j = 0; j < circle_objs.size(); j++)
            {
                cv::Point ctr((int)round(circle_objs[j][0]), (int)round(circle_objs[j][1]));
                int r = (int)round(circle_objs[j][2]);
                if (pow(k - ctr.x,2) + pow(i - ctr.y,2) < pow(r,2))
                {
                    sum_list[j] += val;
                }
                
            }
            ++point;
        }
    }
    //Smallest sum -> Circle with most black/dark pixels
    int min_sum = 99999999999;
    int min_i = -1;
    for (int q = 0; q < circle_objs.size(); q++)
    {
        if (min_sum > sum_list[q])
        {
            min_sum = sum_list[q];
            min_i = q;
        }
        
    }
    return circle_objs[min_i];
}
void detect_frame(cv::Mat &img, cv::CascadeClassifier &f_Cascade, cv::CascadeClassifier &e_Cascade){

    //CONVERTS FRAME TO BLACK AND WHITE THEN DETECTS FACE FROM FRAME
    cv::Mat bw;
    cv::cvtColor(img, bw, CV_BGR2GRAY); // convert image to grayscale
    cv::equalizeHist(bw, bw); // enhance image contrast 
    vector<cv::Rect> face_region;
    f_Cascade.detectMultiScale(bw, face_region, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(150, 150));
    if (face_region.size() == 0) return;
    std::cout << "FACE";
    cv::Mat face = img(face_region[0]);
    //DETECT EYES FROM FACE REGION
    vector<cv::Rect> eye_region;
    e_Cascade.detectMultiScale(face, eye_region, 2, 1, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(50, 50)); 
    rectangle(img, face_region[0].tl(), face_region[0].br(), cv::Scalar(255, 0, 0), 2);
    if (eye_region.size() != 2) return; // both eyes were not detected
    for (cv::Rect &eye : eye_region)
    {
        rectangle(img, face_region[0].tl() + eye.tl(), face_region[0].tl() + eye.br(), cv::Scalar(0, 255, 0), 2);
    }
    cv::Rect eye_square = getLeft(eye_region);
    cv::Mat eye = face(eye_square);
    cv::equalizeHist(eye, eye);
    vector<cv::Vec3f> cir_objs;
    cv::HoughCircles(eye, cir_objs, CV_HOUGH_GRADIENT, 1, eye.cols / 8, 250, 15, eye.rows / 8, eye.rows / 3);
    cv::Vec3f iris = getIris(eye, cir_objs);
    

}

int main(){
    cv::CascadeClassifier faceCascade;
    if (!faceCascade.load("./haarcascade_frontalface_default.xml"))
    {
        cout << "Face Cascade not detected." << std::endl;
        return -1;
    }
    cv::CascadeClassifier eyeCascade;
    if (!eyeCascade.load("./haarcascade_eye.xml"))
    {
        cout << "Eye Cascade not detected." << std::endl;
        return -1;
    }
    cv::VideoCapture cap(0); // the fist webcam connected to your PC
    if (!cap.isOpened())
    {
        cout << "Webcam not detected." << std::endl;
        return -1;
    }
    cv::Mat frame;
    while (1)
    {
        cap >> frame; // outputs the webcam image to a Mat
        detect_frame(frame, faceCascade, eyeCascade);
        cv::imshow("Webcam", frame); // displays the Mat
        cvWaitKey(30);
    }
    return 0;
        
    
}