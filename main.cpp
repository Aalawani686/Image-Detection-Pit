#include "opencv2/opencv.hpp"

using namespace cv;

int threshold(Mat image){

     Mat img_hsv;
     cvtColor(image, img_hsv, CV_BGR2HSV);
     std::vector<cv::Mat> channels;
     split(img_hsv, channels);

     Mat hueOrig = channels.at(0).clone();
     Mat threshLower, threshUpper;
     Mat result;

     threshold(hueOrig, threshLower, 60, 255, CV_THRESH_BINARY);
     threshold(hueOrig, threshUpper, 180, 255, CV_THRESH_BINARY_INV);

     result = threshLower & threshUpper;

     Mat edges;
     Canny(result, edges, 100, 200);
     //does this work
     int Width = image.cols * 2;
     int Height = image.rows * 2;
     Mat dst = Mat(Height, Width, CV_8UC3, Scalar(0,0,0));
     Rect roi(Rect(0,0,Width * 2, Height * 2));
     Mat targetROI = dst(roi);

     std::vector<std::vector<Point> > contours;


     cv::findContours(edges, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

     std::vector<std::Point> output;

     namedWindow("allFour", 1);
     for(int i = 0; i< contours.size(); i++) {
       approxPolyDP(contours[i], output, cv::arcLength(cv::Mat(contours.at(i)), true) * 0.02
       , bool closed);
       if(output.size()==4){
         image.copyTo(targetROI);
         targetROI = dst(cv::Rect(Width,0,image1.cols, image1.rows));
         result.copyTo(targetROI);
         targetROI = dst(cv::Rect(0,Height,image1.cols, image1.rows));
         edges.copyTo(targetROI);
         //drawContours(output, contours, i, Scalar(255,0,0));
         //TODO find a way to make drawContours a mat and put into big Mat
         imshow("allFour", dst);
         break;
       }

     }

     waitKey(0);



}

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    //Mat edges;
    //namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame;
        threshold(frame);// get a new frame from camera
        /*cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        threshold(frame);*/

        if(waitKey(30) >= 0) break;
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
