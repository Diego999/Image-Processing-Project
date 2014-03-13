#include "include/Picture/PictureController.h"

#include <map>

#include <QString>
#include <QImage>
#include <QColor>

std::vector<std::vector<double>> PictureController::loadPictures(const std::vector<std::string>& filepaths, bool otsu)
{
    std::vector<std::vector<double>> output;

    for(auto& filepath : filepaths)
    {
        QImage picture(QString::fromStdString(filepath));

        std::vector<double> data;
        for(int i = 0; i < picture.height(); ++i)
            for(int j = 0; j < picture.width(); ++j)
                data.push_back(QColor(picture.pixel(j, i)).red());
        output.push_back(otsu ? otsuSegmentation(data) : data);
    }

    return output;
}

QImage PictureController::create(const std::vector<double>& values, int width, bool otsu)
{
    QImage picture(width, values.size()/width, QImage::Format_RGB888);

    double ratio = 255.0/(*std::max_element(values.cbegin(), values.cend()) - *std::min_element(values.cbegin(), values.cend()));
    for(size_t i = 0; i < values.size(); ++i)
        picture.setPixel(i%picture.width(), i/picture.width(), qRgb(ratio*values[i], ratio*values[i], ratio*values[i]));

    return otsu ? otsuSegmentation(picture) : picture;
}

void PictureController::otsuSegmentation(std::vector<double> &picture)
{
    int min = static_cast<int>(*std::min_element(picture.cbegin(), picture.cend()));
    int max = static_cast<int>(*std::max_element(picture.cbegin(), picture.cend()));

    std::map<int, double> histogram;
    for(int i = min; i <= max; ++i)
        histogram[i] = 0.0;

    for(auto& v : picture)
        ++histogram[static_cast<int>(v)];
    for(std::pair<int, double> v : histogram)
        histogram[v.first] /= picture.size();

    std::vector<std::pair<int, double>> ow;
    for(int t = min+1; t < max; ++t)
    {
        double p1, p2, m1, m2;
        p1 = p2 = m1 = m2 = 0;
        int i = min;
        for(auto& v : histogram)
            if(i++ < t)
            {
                p1 += v.second;
                m1 += v.first*v.second;
            }
            else
            {
                p2 += v.second;
                m2 += v.first*v.second;
            }
        m1 /= p1; m2 /= p2;

        double s1 = 0, s2 = 0;
        i = min;
        for(auto& v : histogram)
            if(i++ < t)
                s1 += v.second*variance(i-1, m1);
            else
                s2 += v.second*variance(i-1, m2);
        s1 /= p1; s2 /= p2;

        ow.push_back(std::pair<int, double>(t, p1*s1+p2*s2));
    }

    std::sort(ow.begin(), ow.end(),
              [](std::pair<int,double> const & a, std::pair<int,double> const & b)
            {
                 return a.second != b.second?  a.second < b.second : a.first < b.first;
            });

    for(auto& v : picture)
        v = v <= ow[0].first ? 0 : 1;
}
