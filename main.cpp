#include "opencv2/opencv.hpp"

using namespace cv;

int threshold(Mat image){


     Mat img_hsv;
     imshow("image", image);
     cvtColor(image, img_hsv, CV_BGR2HSV);
     std::vector<cv::Mat> channels;
     split(img_hsv, channels);

     Mat hueOrig = channels.at(0).clone();
     Mat threshLower, threshUpper;
     Mat result;


     threshold(hueOrig, threshLower, 60, 255, CV_THRESH_BINARY);
     threshold(hueOrig, threshUpper, 180, 255, CV_THRESH_BINARY_INV);

     result = threshLower & threshUpper;

     imshow("threshold", result);
     Mat edges;
     Canny(result, edges, 100, 200);
     //does this work
     imshow("Canny", edges);
     std::vector<std::vector<Point> > contours;


     cv::findContours(edges, contours, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

     std::vector<Point> output;
     Mat drawing = Mat::zeros(edges.size(), CV_8UC3);

     for(int i = 0; i< contours.size(); i++) {
       approxPolyDP(contours[i], output, cv::arcLength(cv::Mat(contours.at(i)), true) * 0.02, true);
       if(output.size()==4){

         drawContours(drawing, contours, i, Scalar(255,0,0));
         imshow("contours", drawing);
         break;
       }

     }


}

int main()
{

    VideoCapture cap(2); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat frame;
    //Mat edges;
    //namedWindow("edges",1);
    while(cv::waitKey(30) != 27)
    {

        cap >> frame;
        threshold(frame);// get a new frame from camera
        /*cvtColor(frame, edges, COLOR_BGR2GRAY);
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        Canny(edges, edges, 0, 30, 3);
        imshow("edges", edges);
        threshold(frame);*/

    }

    waitKey(0);
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;


}
