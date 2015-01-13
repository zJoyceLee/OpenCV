//get frame from video
#include "opencv2/opencv.hpp"
//check people and draw rect
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/gpu/gpu.hpp>

using namespace cv;

int main(int argc, char** argv)
{
    VideoCapture cap("9.avi"); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
        return -1;

    Mat CheckStandingPeople;
    namedWindow("CheckStandingPeople", CV_WINDOW_AUTOSIZE);

    vector<Rect> found;
    for(int frameNum = 0;; ++frameNum)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        if(frame.empty())
            break;
        HOGDescriptor defaultHog;
        defaultHog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

        //check
        if(frameNum % 2 == 0)
            defaultHog.detectMultiScale(frame, found);

        //draw tect
        for(std::size_t i = 0; i < found.size(); i++){
            Rect r = found[i];
            rectangle(frame, r.tl(), r.br(), Scalar(0, 255, 0), 3);
        }

        imshow("CheckStandingPeople", frame);
        if(waitKey(1) >= 0) break;

    }
    return 0;
}
