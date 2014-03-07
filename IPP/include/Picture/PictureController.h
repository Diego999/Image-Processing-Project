#ifndef PICTURECONTROLLER_H
#define PICTURECONTROLLER_H

#include <vector>
#include <QImage>
#include <string>

class PictureController
{
public:
    static std::vector<std::vector<double>> loadPictures(const std::vector<std::string>& filepaths);
    static QImage create(const std::vector<double>& values, int width);

private:
    static std::vector<double> otsuSegmentation(const std::vector<double>& picture);
};

#endif // PICTURECONTROLLER_H
