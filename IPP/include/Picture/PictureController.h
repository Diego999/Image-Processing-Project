#ifndef PICTURECONTROLLER_H
#define PICTURECONTROLLER_H

#include <vector>
#include <string>

#include <QImage>

class PictureController
{
public:
    static std::vector<std::vector<double>> loadPictures(const std::vector<std::string>& filepaths, bool otsu = false);
    static QImage create(const std::vector<double>& values, int width, bool otsu = false);

private:
    static void otsuSegmentation(std::vector<double> &picture);
    static double variance(double x, double m) {return (x-m)*(x-m);}
};

#endif // PICTURECONTROLLER_H
