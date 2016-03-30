#include "picture.hpp"

using namespace IMT;

Pixel Picture::GetInterPixel(CoordF pt) const
{
    const cv::Mat& img = m_pictMat;
    assert(!img.empty());
    assert(img.channels() == 3);

    int x = (int)pt.x;
    int y = (int)pt.y;

    int x0 = cv::borderInterpolate(x,   img.cols, cv::BORDER_REFLECT_101);
    int x1 = cv::borderInterpolate(x+1, img.cols, cv::BORDER_REFLECT_101);
    int y0 = cv::borderInterpolate(y,   img.rows, cv::BORDER_REFLECT_101);
    int y1 = cv::borderInterpolate(y+1, img.rows, cv::BORDER_REFLECT_101);

    float a = pt.x - (float)x;
    float c = pt.y - (float)y;

    uchar b = (uchar)cvRound((img.at<Pixel>(y0, x0)[0] * (1.f - a) + img.at<Pixel>(y0, x1)[0] * a) * (1.f - c)
                           + (img.at<Pixel>(y1, x0)[0] * (1.f - a) + img.at<Pixel>(y1, x1)[0] * a) * c);
    uchar g = (uchar)cvRound((img.at<Pixel>(y0, x0)[1] * (1.f - a) + img.at<Pixel>(y0, x1)[1] * a) * (1.f - c)
                           + (img.at<Pixel>(y1, x0)[1] * (1.f - a) + img.at<Pixel>(y1, x1)[1] * a) * c);
    uchar r = (uchar)cvRound((img.at<Pixel>(y0, x0)[2] * (1.f - a) + img.at<Pixel>(y0, x1)[2] * a) * (1.f - c)
                           + (img.at<Pixel>(y1, x0)[2] * (1.f - a) + img.at<Pixel>(y1, x1)[2] * a) * c);

    return Pixel(b, g, r);
}