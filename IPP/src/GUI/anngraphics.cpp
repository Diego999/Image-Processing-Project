#include "anngraphics.h"

#include <qwt_legend.h>

ANNGraphics::ANNGraphics(const std::vector<std::tuple<std::string, QwtSymbol*, QPen> > &curves, const std::string& titleX, const std::string& titleY)
{
    for(auto& tuple : curves)
    {
        std::shared_ptr<QwtPlotCurve> curve(new QwtPlotCurve(QString::fromStdString(std::get<0>(tuple))));
        QPolygonF points;

        curve->setPen(std::get<2>(tuple));
        curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
        curve->setSymbol(std::get<1>(tuple));
        curve->setSamples(points);
        curve->attach(&plot);

        this->curves.push_back(std::pair<std::shared_ptr<QwtPlotCurve>, QPolygonF>(curve, points));
    }

    plot.setCanvasBackground(Qt::white);
    plot.setAxisScale(QwtPlot::yLeft, 0, 0);
    plot.insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
    plot.enableAxis(QwtPlot::xBottom);
    plot.enableAxis(QwtPlot::yLeft);
    plot.setAxisAutoScale(QwtPlot::xBottom, true);
    plot.setAxisAutoScale(QwtPlot::yLeft, true);
    plot.setAxisTitle(QwtPlot::xBottom, QString::fromStdString(titleX));
    plot.setAxisTitle(QwtPlot::yLeft, QString::fromStdString(titleY));
    grid.attach(&plot);
    plot.show();
}

void ANNGraphics::addPoints(const std::vector<std::vector<QPointF>> &points)
{
    for(auto& point : points)
        addPoint(point);
}

void ANNGraphics::addPoint(const std::vector<QPointF>& point)
{
    if(point.size() == curves.size())
    {
        auto curve = curves.begin();
        for(auto& p : point)
        {
            curve->second << p;
            curve->first->setSamples(curve->second);
            ++curve;
        }
        plot.replot();
    }
}

void ANNGraphics::resize(const QSize& size)
{
    plot.resize(size);
}
