#include <iostream>
#include <vector>
#include <cmath>
#include <opencv2/opencv.hpp>

float sqr(float num) {
    return num * num;
}

float getDiagDistance(const cv::Point & tl, const cv::Point & br) {
    return std::sqrt(sqr(tl.x - br.x) + sqr(tl.y - br.y));
}

const float c_diagDistThreshold = 40;

void trackRedCars(const cv::Mat & image) {
    // Convert Original Image to HSV Image
    cv::Mat imageHSV;
    cv::cvtColor(image, imageHSV, cv::COLOR_BGR2HSV);
    // Mask out red part in HSV image
    cv::Mat redMask;
    cv::inRange(imageHSV, cv::Scalar(170, 0, 0), cv::Scalar(180, 255, 255), redMask);

    //cv::imshow("Red Mask", redMask);

    // get bound contours of the red masks in HSV image
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(redMask, contours, hierarchy,
        CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE,
        cv::Point(0, 0));

    std::vector<std::vector<cv::Point>> contours_poly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    for(std::size_t i = 0; i < contours.size(); i++) {
        cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
        boundRect[i] = cv::boundingRect(cv::Mat(contours_poly[i]));
    }

    boundRect.erase(std::remove_if(boundRect.begin(), boundRect.end(),
        [&](const cv::Rect & rect) -> bool {
            return getDiagDistance(rect.tl(), rect.br()) < c_diagDistThreshold;
        }), boundRect.end());

    // draw bounds of HSV image
    //cv::Mat boundImage = cv::Mat::zeros(redMask.size(), CV_8UC3);
    cv::Mat boundImage = image;
    for(std::size_t i = 0; i < boundRect.size(); ++i) {
        cv::rectangle(boundImage, boundRect[i].tl(), boundRect[i].br(),
            cv::Scalar(0, 255, 0), 2, 8, 0 );
    }
    cv::imshow("bounds", boundImage);
}

int main(int argc, char * argv[])
{
    cv::VideoCapture cap("video2.mp4");
    if(!cap.isOpened())
        return 1;

    for(int frameNum = 0;; ++frameNum)
    {
        cv::Mat frame;
        cap >> frame; // get a new frame from camera
        if(frame.empty())
            break;
        trackRedCars(frame);

        if(cv::waitKey(1) > 0)
            break;
    }

    return 0;
}
