#ifndef PICTURECONTROLLER_H
#define PICTURECONTROLLER_H

#include <vector>
#include <string>

#include <QImage>

class PictureController
{
public:
    static std::vector<std::vector<double>> loadPictures(const std::vector<std::string>& filepaths);
    static QImage create(const std::vector<double>& values, int width);
    static QImage createThresholded(const std::vector<double>& values, int width);

private:
    static std::vector<double> otsuSegmentation(const std::vector<double>& picture);
    static double variance(double x, double m) {return (x-m)*(x-m);}
};

#endif // PICTURECONTROLLER_H
