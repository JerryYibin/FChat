#include "CChartWidget.h"

#include <QPainter>
#include <QLine>
#include <QScrollBar>
#include <QDebug>
#include <QPointF>

CChartWidget::CChartWidget(QScrollArea *pControlScroll, QWidget *parent)
    : QWidget(parent)
    , m_pControlScroll(pControlScroll)
    , m_dScaled(1.0)
    , m_dMinX(0)
    , m_dMaxX(120)
    , m_iXScalePixel(70)
    , m_dXScaleValue(5)
    , m_iBottomInterval(50)
    , m_iXTextDown(15)
    , m_iXTextLeft(5)
    , m_dMinY(19000)
    , m_dMaxY(21000)
    , m_iYScalePixel(10)
    , m_dYScaleValue(100)
    , m_iRightInterval(50)
    , m_iYTextRight(5)
    , m_dRadius(2)
    , m_iTopInterval(10)
    , m_iLeftInterval(30)
    , m_bReteScale(false)
    , m_bAdaptiveInterface(true)
    , m_strXUnit("秒")
    , m_strYUnit("")
{
    m_pControlScroll->setAlignment(Qt::AlignRight|Qt::AlignBottom);
    updateCalculateWidth();

    connect(m_pControlScroll->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(update()));
    connect(m_pControlScroll->horizontalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(update()));
}

bool CChartWidget::setXMinValue(double minX)
{
    if(minX > m_dMaxX)
    {
        return false;
    }
    m_dMinX = minX;
    updateCalculateWidth();
    return true;
}

bool CChartWidget::setXMaxValue(double maxX)
{
    if(maxX < m_dMinX)
    {
        return false;
    }
    m_dMaxX = maxX;
    updateCalculateWidth();
    return true;
}

bool CChartWidget::setXAxisValue(double minX, double maxX)
{
    if(minX > maxX)
    {
        return false;
    }
    m_dMinX = minX;
    m_dMaxX = maxX;
    updateCalculateWidth();
    return true;
}

void CChartWidget::setXScalePixel(int xScalePixel)
{
    m_iXScalePixel = xScalePixel;
    updateCalculateWidth();
}

void CChartWidget::setXScaleValue(double xScaleValue)
{
    m_dXScaleValue = xScaleValue;
    updateCalculateWidth();
}

bool CChartWidget::setYMinValue(double minY)
{
    if(minY > m_dMaxY)
    {
        return false;
    }
    m_dMinY = minY;
    updateCalculateWidth();
    return true;
}

bool CChartWidget::seYtMaxValue(double maxY)
{
    if(maxY < m_dMinY)
    {
        return false;
    }
    m_dMaxY = maxY;
    updateCalculateWidth();
    return false;
}

bool CChartWidget::setYAxisValue(double minY, double maxY)
{
    if(minY > maxY)
    {
        return false;
    }
    m_dMinY = minY;
    m_dMaxY = maxY;
    updateCalculateWidth();
    return true;
}

void CChartWidget::setYScalePixel(int yScalePixel)
{
    m_iYScalePixel = yScalePixel;
    updateCalculateWidth();
}

void CChartWidget::setYScaleValue(double yScaleValue)
{
    m_dYScaleValue = yScaleValue;
    updateCalculateWidth();
}

void CChartWidget::setData(const QList<QPointF> &listData)
{
    m_listValue= listData;
    if(m_bReteScale && m_listValue.count() > 0)
    {
        m_dMinX = m_listValue[0].x();
        m_dMaxX = m_listValue[0].x();
        m_dMinY = m_listValue[0].y();
        m_dMaxY = m_listValue[0].y();
        QList<QPointF>::Iterator itr = m_listValue.begin();
        while(itr != m_listValue.end())
        {
            if(m_dMinX > (*itr).x())
            {
                m_dMinX = (*itr).x();
            }
            if(m_dMaxX < (*itr).x())
            {
                m_dMaxX = (*itr).x();
            }
            if(m_dMinY > (*itr).y())
            {
                m_dMinY = (*itr).y();
            }
            if(m_dMaxY < (*itr).y())
            {
                m_dMaxY = (*itr).y();
            }
            ++itr;
        }
        int xMulriple = static_cast<int>((m_dMaxX - m_dMinX)/m_dXScaleValue + 1);
        m_dMaxX = m_dMinX + xMulriple*m_dXScaleValue;
        int yMulriple = static_cast<int>((m_dMaxY - m_dMinY)/m_dYScaleValue + 1);
        m_dMaxY = m_dMinY + yMulriple*m_dYScaleValue;
        updateCalculateWidth();
    }
    else
    {
        update();
    }
}

void CChartWidget::setResetScale(bool resetScale)
{
    m_bReteScale = resetScale;
}

void CChartWidget::setXUnit(const QString &unit)
{
    m_strXUnit = unit;
    this->update();
}

void CChartWidget::setYUnit(const QString &unit)
{
    m_strYUnit = unit;
    this->update();
}

void CChartWidget::addData(const QPointF &pointF)
{
    m_listValue.append(pointF);
    if(m_listValue.count() < 121)
    {
        m_dMinX = m_listValue.first().x();
        m_dMaxX = m_dMinX + 120;
    }
    else if(m_listValue.count() == 121)
    {
        m_dMinX = m_listValue.first().x();
        m_dMaxX = m_listValue.last().x();
    }
    else
    {
        m_listValue.removeFirst();
        m_dMinX = m_listValue.first().x();
        m_dMaxX = m_listValue.last().x();
    }
    this->update();
}

void CChartWidget::slotClear()
{
    m_dMinX = 0;
    m_dMaxX = 120;
    m_listValue.clear();
    this->update();
}

void CChartWidget::paintEvent(QPaintEvent *event)
{
    if(m_bAdaptiveInterface)
    {
        if(this->size() != m_pControlScroll->size())
        {
            this->setFixedSize(m_pControlScroll->size());
            return;
        }
    }

    QWidget::paintEvent(event);

    //获取本界面在滑动界面右下角的坐标
    QPoint rightBottomPoint = this->mapFromGlobal(m_pControlScroll->mapToGlobal(QPoint(m_pControlScroll->width(), m_pControlScroll->height())));
    if(rightBottomPoint.x() > this->width())
    {
        rightBottomPoint.setX(this->width());
    }
    if(rightBottomPoint.y() > this->height())
    {
        rightBottomPoint.setY(this->height());
    }
    QPoint leftTopPoint = this->mapFromGlobal(m_pControlScroll->mapToGlobal(QPoint(0, 0)));
    if(leftTopPoint.x() < 0)
    {
        leftTopPoint.setX(0);
    }
    if(leftTopPoint.y() < 0)
    {
        leftTopPoint.setY(0);
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //绘制X轴刻度
    double xScalePixel = m_iXScalePixel;
    if(m_bAdaptiveInterface)
    {
        xScalePixel = m_dXScaleValue*((this->width() - m_iLeftInterval - m_iRightInterval)/m_dScaled)/(m_dMaxX - m_dMinX);
    }
    //X轴两数据间间隔像素
    double xAxisIntervalPixel = xScalePixel*m_dScaled;
    //x轴1像素代表的值
    double xAxis1Value = m_dXScaleValue/xAxisIntervalPixel;
    //计算x轴起点值
    double xBeginValue = m_dMinX + leftTopPoint.x()*xAxis1Value;
    //计算X轴终点的值
    double xEndValue = m_dMinX + (rightBottomPoint.x() - m_iRightInterval - m_iLeftInterval)*xAxis1Value;
    //绘制X轴坐标刻度
    int xAxisIndex = 0;
    for(double value = m_dMinX; value <= xEndValue; value += m_dXScaleValue)
    {
        if(value >= xBeginValue)
        {
            painter.drawLine(QPointF(xAxisIntervalPixel*xAxisIndex + m_iLeftInterval, leftTopPoint.y() + m_iTopInterval),
                             QPointF(xAxisIntervalPixel*xAxisIndex + m_iLeftInterval, static_cast<double>(rightBottomPoint.y() - m_iBottomInterval)));
            if(xAxisIndex%2 == 0)
            {
                painter.drawText(QPointF(QPointF(xAxisIntervalPixel*xAxisIndex + m_iLeftInterval - m_iXTextLeft,
                                             rightBottomPoint.ry() - m_iBottomInterval + m_iXTextDown)),
                             QString::number(value));
            }
        }
        ++xAxisIndex;
    }
    //绘制X轴单位
    painter.drawText(QRect(rightBottomPoint.x() - m_iRightInterval, rightBottomPoint.y() - m_iBottomInterval + m_iXTextDown,
                           m_iRightInterval*3/4, m_iBottomInterval - m_iXTextDown), m_strXUnit, QTextOption(Qt::AlignRight|Qt::AlignTop));

    //绘制Y轴刻度
    double yScalePixel = m_iYScalePixel;
    if(m_bAdaptiveInterface)
    {
        yScalePixel = m_dYScaleValue*((this->height() - m_iTopInterval - m_iBottomInterval)/m_dScaled)/(m_dMaxY - m_dMinY);
    }
    //X轴两数据间间隔像素
    double yAxisIntervalPixel = yScalePixel*m_dScaled;
    //x轴1像素代表的值
    double yAxis1Value = m_dYScaleValue/yAxisIntervalPixel;
    //计算x轴起点值
    double yBeginValue = m_dMinY + (this->height() - rightBottomPoint.y() - m_iBottomInterval)*yAxis1Value;
    //计算X轴终点的值
    double yEndValue = m_dMinY + (this->height() - leftTopPoint.y() - m_iBottomInterval)*yAxis1Value;
    //绘制X轴坐标刻度
    int yAxisIndex = 0;
    for(double value = m_dMinY; value < yEndValue; value += m_dYScaleValue)
    {
        if(value >= yBeginValue)
        {
            painter.drawLine(QPointF(leftTopPoint.x() + m_iLeftInterval, this->height() - yAxisIntervalPixel*yAxisIndex - m_iBottomInterval),
                             QPointF(rightBottomPoint.x() - m_iRightInterval, this->height() - yAxisIntervalPixel*yAxisIndex - m_iBottomInterval));
            painter.drawText(QPointF(rightBottomPoint.x() - m_iRightInterval + m_iYTextRight,
                                     this->height() - yAxisIntervalPixel*yAxisIndex - m_iBottomInterval),
                             QString::number(value));
        }
        ++yAxisIndex;
    }

    //绘制数据
    QPen pen = painter.pen();
    pen.setColor(Qt::blue);
    painter.setPen(pen);
    QBrush brush(Qt::blue);
    painter.setBrush(brush);
    //X轴1数值需要的像素
    double xValuePixel = xAxisIntervalPixel/m_dXScaleValue;
    //Y轴1数值需要的像素
    double yValuePixel = yAxisIntervalPixel/m_dYScaleValue;
    QList<QPointF>::Iterator itr = m_listValue.begin();
    while(itr != m_listValue.end())
    {
        //根据数据计算像素位置
        double xValue = m_iLeftInterval + ((*itr).x() - m_dMinX)*xValuePixel;
        double yValue = this->height() - m_iBottomInterval - ((*itr).y() - m_dMinY)*yValuePixel;
        painter.drawEllipse(QPointF(xValue, yValue), m_dRadius, m_dRadius);
        ++itr;
    }
}

void CChartWidget::wheelEvent(QWheelEvent *event)
{
    if(m_bAdaptiveInterface)
    {
        return;
    }
    if(event->delta() > 0)
    {
        m_dScaled *= 1.1;
    }
    else
    {
        m_dScaled *= 0.9;
    }
    updateCalculateWidth();
}

void CChartWidget::updateCalculateWidth()
{
    if(m_bAdaptiveInterface)
    {
        return;
    }

    m_iCalculateWidth = static_cast<int>(m_pControlScroll->width()*m_dScaled + m_iRightInterval + m_iLeftInterval + 0.5);
    m_iCalculateHeight = static_cast<int>(m_pControlScroll->height()*m_dScaled + m_iBottomInterval + m_iTopInterval + 0.5);

    this->setFixedSize(m_iCalculateWidth, m_iCalculateHeight);
}
