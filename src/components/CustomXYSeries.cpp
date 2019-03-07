#include "CustomXYSeries.h"

#include <QVector>

CustomXYSeries::CustomXYSeries(QObject *parent) : QObject(parent)
  , m_width(1)
  , m_color("#ffffff")
  , m_axisX(nullptr)
  , m_axisY(nullptr)
  , m_axisYRight(nullptr)
  , m_visible(true)
  , m_pointsVisible(true)
  , m_iTime(0)
{
    this->setProperty("custom", "CustomXYSeries");
}

void CustomXYSeries::replace(QList<QPointF> points)
{
    this->setListData(points);
}

void CustomXYSeries::replace(QVector<QPointF> points)
{
    this->setListData(points.toList());
}

void CustomXYSeries::clear()
{

}

const QList<QPointF> &CustomXYSeries::constListData() const
{
    return m_listData;
}

TagSeriesTitle* CustomXYSeries::getTitleInfo()
{
    return &m_titleInfo;
}

TagSeriesPoint* CustomXYSeries::getDrawPointInfo()
{
    return &m_drawPointInfo;
}

QList<QPointF> CustomXYSeries::listData() const
{
    return m_listData;
}

void CustomXYSeries::setListData(const QList<QPointF> &listData)
{
    if(m_listData != listData)
    {
        m_listData = listData;
        emit listDataChanged();
    }
}

QString CustomXYSeries::name() const
{
    return m_name;
}

void CustomXYSeries::setName(const QString &name)
{
    if(m_name != name)
    {
        m_name = name;
        emit nameChanged();
    }
}

double CustomXYSeries::width() const
{
    return m_width;
}

void CustomXYSeries::setWidth(double width)
{
    if(m_width < width || m_width > width)
    {
        m_width = width;
        emit widthChanged();
    }
}

QColor CustomXYSeries::color() const
{
    return m_color;
}

void CustomXYSeries::setColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        emit colorChanged();
    }
}

CustomValueAxis *CustomXYSeries::axisX() const
{
    return m_axisX;
}

void CustomXYSeries::setAxisX(CustomValueAxis *axisX)
{
    if(m_axisX != axisX)
    {
        m_axisX = axisX;
        emit axisXChanged();
    }
}

CustomValueAxis *CustomXYSeries::axisY() const
{
    return m_axisY;
}

void CustomXYSeries::setAxisY(CustomValueAxis *axisY)
{
    if(m_axisY != axisY || !m_axisYIsLeft)
    {
        m_axisY = axisY;
        this->setAxisYIsLeft(true);
        emit axisYChanged();
    }
}

CustomValueAxis *CustomXYSeries::axisYRight() const
{
    return m_axisYRight;
}

void CustomXYSeries::setAxisYRight(CustomValueAxis *axisYRight)
{
    if(m_axisYRight != axisYRight || m_axisYIsLeft)
    {
        m_axisYRight = axisYRight;
        this->setAxisYIsLeft(false);
        emit axisYRightChanged();
    }
}

bool CustomXYSeries::visible() const
{
    return m_visible;
}

void CustomXYSeries::setVisible(bool visible)
{
    if(m_visible != visible)
    {
        m_visible = visible;
        emit visibleChanged();
    }
}

bool CustomXYSeries::pointsVisible() const
{
    return m_pointsVisible;
}

void CustomXYSeries::setPointsVisible(bool pointsVisible)
{
    if(m_pointsVisible != pointsVisible)
    {
        m_pointsVisible = pointsVisible;
        emit pointsVisibleChanged();
    }
}

bool CustomXYSeries::axisYIsLeft() const
{
    return m_axisYIsLeft;
}

void CustomXYSeries::setAxisYIsLeft(bool axisYIsLeft)
{
    if(m_axisYIsLeft != axisYIsLeft)
    {
        m_axisYIsLeft = axisYIsLeft;
        emit axisYIsLeftChanged();
    }
}
