#ifndef ANNGRAPHICS_H
#define ANNGRAPHICS_H

#include <vector>
#include <tuple>
#include <string>
#include <memory>

#include <QColor>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>

class ANNGraphics : public QwtPlot
{
public:
    ANNGraphics(const std::string& titleX, const std::string& titleY);

    void addPoints(const std::vector<std::vector<QPointF>>& points);
    void addPoint(const std::vector<QPointF>& point);

    void addCurve(const std::tuple<std::string, QPen, QwtSymbol*>& curveData, unsigned int k = 0);
    void addCurve(const std::tuple<std::string, QPen>& curveData, unsigned int k = 0);

    void addPointKFoldCrossValidation(const std::vector<std::vector<QPointF>>& points);
    void addKFoldValidationCurves(unsigned int k);

    void reset();

private:
    void initBasic(const std::string &titleX, const std::string &titleY);
    void initLoop(std::shared_ptr<QwtPlotCurve> curve);

    QwtPlotGrid grid;

    std::vector<std::pair<std::shared_ptr<QwtPlotCurve>, QPolygonF>> curves;
    std::vector<std::pair<std::shared_ptr<QwtPlotCurve>, QPolygonF>> kFoldValidationCurves;
};

#endif // ANNGRAPHICS_H
