#include "include/Picture/PictureController.h"

#include <QString>
#include <QImage>
#include <QColor>
#include <iostream>
#include <QDebug>

std::vector<std::vector<double>> PictureController::loadPictures(const std::vector<std::string>& filepaths)
{
    std::vector<std::vector<double>> output;

    for(auto& filepath : filepaths)
    {
        QImage picture(QString::fromStdString(filepath));

        std::vector<double> data;
        for(int i = 0; i < picture.height(); ++i)
            for(int j = 0; j < picture.width(); ++j)
                data.push_back(QColor(picture.pixel(j, i)).red());
        output.push_back(data);
    }

    return output;
}

QImage PictureController::create(const std::vector<double>& values, int width)
{
    QImage picture(width, values.size()/width, QImage::Format_RGB888);

    double ratio = 255.0/(*std::max_element(values.cbegin(), values.cend()) - *std::min_element(values.cbegin(), values.cend()));
    for(size_t i = 0; i < values.size(); ++i)
        picture.setPixel(i%picture.width(), i/picture.width(), qRgb(ratio*values[i], ratio*values[i], ratio*values[i]));

    return picture;
}

std::vector<double> PictureController::otsuSegmentation(const std::vector<double>& picture)
{

}
