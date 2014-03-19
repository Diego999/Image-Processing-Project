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

class ANNGraphics
{
public:
    ANNGraphics(const std::vector<std::tuple<std::string, QwtSymbol*, QPen>>& curves, const std::string& titleX, const std::string& titleY);

    void addPoints(const std::vector<std::vector<QPointF>>& points);
    void addPoint(const std::vector<QPointF>& point);

    void resize(const QSize& size);

private:
    QwtPlot plot;
    QwtPlotGrid grid;

    std::vector<std::pair<std::shared_ptr<QwtPlotCurve>, QPolygonF>> curves;
};

#endif // ANNGRAPHICS_H