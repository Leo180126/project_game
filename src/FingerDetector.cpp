#include "FingerDetector.h"

FingerDetector::FingerDetector()
    :   lowerRed_(1,150,150),
        upperRed_(10,225,225)
    {
    }


FingerDetector::~FingerDetector()=default;


cv::Point FingerDetector::detectFingertip(cv::Mat& frame){
    cv::Mat hsvFrame, mask, filteredMask;
    cv::flip(frame, frame, 1);
    cv::cvtColor(frame, hsvFrame, cv::COLOR_BGR2HSV);

    cv::inRange(hsvFrame, lowerRed_, upperRed_, mask);
    cv::GaussianBlur(mask, mask, cv::Size(9, 9), 2, 2);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::erode(mask, filteredMask, kernel);
    cv::dilate(filteredMask, filteredMask, kernel);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(filteredMask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    cv::Point fingertip(-1, -1);
    if (!contours.empty()) {
        std::vector<cv::Point> largestContour = contours[0];
        for (auto &contour : contours) {
            if (cv::contourArea(contour) > cv::contourArea(largestContour)) {
                largestContour = contour;
            }
        }
        cv::Moments m = cv::moments(largestContour);
        fingertip = cv::Point(m.m10 / m.m00, m.m01 / m.m00);
        cv::circle(frame, fingertip, 10, cv::Scalar(255, 0, 0), -1);
    }

    return fingertip;
}