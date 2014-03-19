#include "include/GUI/anngraphics.h"

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
        curve->attach(this);

        this->curves.push_back(std::pair<std::shared_ptr<QwtPlotCurve>, QPolygonF>(curve, points));
    }

    this->setCanvasBackground(Qt::white);
    this->setAxisScale(QwtPlot::yLeft, 0, 0);
    this->insertLegend(new QwtLegend(), QwtPlot::BottomLegend);
    this->enableAxis(QwtPlot::xBottom);
    this->enableAxis(QwtPlot::yLeft);
    this->setAxisAutoScale(QwtPlot::xBottom, true);
    this->setAxisAutoScale(QwtPlot::yLeft, true);
    this->setAxisTitle(QwtPlot::xBottom, QString::fromStdString(titleX));
    this->setAxisTitle(QwtPlot::yLeft, QString::fromStdString(titleY));
    grid.attach(this);
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
        this->replot();
    }
}
