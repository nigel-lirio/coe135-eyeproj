#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp> 

void detectEyes(cv::Mat &frame, cv::CascadeClassifier &faceCascade, cv::CascadeClassifier &eyeCascade)
{
  cv::Mat grayscale;
  cv::cvtColor(frame, grayscale, CV_BGR2GRAY); // convert image to grayscale
  cv::equalizeHist(grayscale, grayscale); // enhance image contrast 
  std::vector<cv::Rect> faces;
  faceCascade.detectMultiScale(grayscale, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(150, 150));
  if (faces.size() == 0) return; // none face was detected
  cv::Mat face = frame(faces[0]); // crop the face
  std::vector<cv::Rect> eyes;
  eyeCascade.detectMultiScale(face, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(150, 150)); // same thing as above  
  rectangle(frame, faces[0].tl(), faces[0].br(), cv::Scalar(255, 0, 0), 2);
  if (eyes.size() != 2) return; // both eyes were not detected
  for (cv::Rect &eye : eyes)
  {
      rectangle(frame, faces[0].tl() + eye.tl(), faces[0].tl() + eye.br(), cv::Scalar(0, 255, 0), 2);
  }
}

int main()
{
  cv::CascadeClassifier faceCascade;
  cv::CascadeClassifier eyeCascade;
  if (!faceCascade.load("./haarcascade_frontalface_alt.xml"))
  {
      std::cerr << "Could not load face detector." << std::endl;
      return -1;
  }
  if (!eyeCascade.load("./haarcascade_eye.xml"))
  {
      std::cerr << "Could not load eye detector." << std::endl;
      return -1;
  }
  cv::VideoCapture cap(0); // the fist webcam connected to your PC
  if (!cap.isOpened())
  {
      std::cerr << "Webcam not detected." << std::endl;
      return -1;
  }
  cv::Mat frame;
  while (1)
  {
      cap >> frame; // outputs the webcam image to a Mat
      detectEyes(frame, faceCascade, eyeCascade);
      cv::imshow("Webcam", frame); // displays the Mat
      cvWaitKey(30);
  }
  return 0;
}