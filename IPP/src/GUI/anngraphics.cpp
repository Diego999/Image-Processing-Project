#include "include/GUI/anngraphics.h"

#include <qwt_legend.h>
#include <tuple>

ANNGraphics::ANNGraphics(const std::string& titleX, const std::string& titleY)
{
    initBasic(titleX, titleY);
}

void ANNGraphics::initLoop(std::shared_ptr<QwtPlotCurve> curve)
{
    QPolygonF points;
    curve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    curve->setSamples(points);
    curve->attach(this);
    this->curves.push_back(std::pair<std::shared_ptr<QwtPlotCurve>, QPolygonF>(curve, points));
}


void ANNGraphics::initBasic(const std::string& titleX, const std::string& titleY)
{
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

void ANNGraphics::addCurve(const std::tuple<std::string, QPen, QwtSymbol*>& curveData)
{
    std::shared_ptr<QwtPlotCurve> curve(new QwtPlotCurve(QString::fromStdString(std::get<0>(curveData))));
    curve->setPen(std::get<1>(curveData));
    curve->setSymbol(std::get<2>(curveData));
    initLoop(curve);
}

void ANNGraphics::addCurve(const std::tuple<std::string, QPen>& curveData)
{
    std::shared_ptr<QwtPlotCurve> curve(new QwtPlotCurve(QString::fromStdString(std::get<0>(curveData))));
    curve->setPen(std::get<1>(curveData));
    initLoop(curve);
}
