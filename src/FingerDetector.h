#ifndef FINGER_DETECTOR_H
#define FINGER_DETECEOR_H
#include <opencv2/opencv.hpp>
class FingerDetector{
    public:
        FingerDetector();
        ~FingerDetector();
        cv::Point detectFingertip(cv::Mat& frame);
    private:
        cv::Scalar lowerRed_;
        cv::Scalar upperRed_;
        int movementThreshold_;
};

#endif