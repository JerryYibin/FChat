#include "CustomValueAxis.h"

CustomValueAxis::CustomValueAxis(QObject *parent) : QObject(parent)
{
    this->setProperty("custom", QVariant("CustomValueAxis"));

    m_color = "#ffffff";
    m_gridVisible = true;
    m_labelsVisible = true;
    m_labelsColor = "#ffffff";
    m_lineVisible = true;
    m_min = 0;
    m_max = 1;
    m_tickCount = 2;
    m_labelsFont.setPixelSize(12);
}

bool CustomValueAxis::operator!=(const CustomValueAxis &other)
{
    if(m_color == other.color() && m_gridVisible == other.gridVisible() && m_labelsVisible == other.labelsVisible()
            && m_labelsFont == other.labelsFont() && m_labelsColor == other.labelsColor() && m_lineVisible == other.lineVisible()
            && m_titleText == other.titleText() && m_titleVisible == other.titleVisible()
            && m_decimals == other.decimals()
            && !(m_min < other.min() || m_min > other.min())
            && !(m_max < other.max() || m_max > other.max())
            && m_tickCount == other.tickCount())
    {
        return true;
    }
    return false;
}

void CustomValueAxis::operator=(const CustomValueAxis &other)
{
    this->setColor(other.color());
    this->setGridVisible(other.gridVisible());
    this->setLabelsVisible(other.labelsVisible());
    this->setLabelsFont(other.labelsFont());
    this->setLabelsColor(other.labelsColor());
    this->setLineVisible(other.lineVisible());
    this->setTitleText(other.titleText());
    this->setTitleVisible(other.titleVisible());
    this->setDecimals(other.decimals());
    this->setMin(other.min());
    this->setMax(other.max());
    this->setTickCount(other.tickCount());
}

TagAxisInfo* CustomValueAxis::getAxisInfo()
{
    return &m_axisInfo;
}

QColor CustomValueAxis::color() const
{
    return m_color;
}

void CustomValueAxis::setColor(const QColor &color)
{
    if(m_color != color)
    {
        m_color = color;
        emit colorChanged();
    }
}

bool CustomValueAxis::gridVisible() const
{
    return m_gridVisible;
}

void CustomValueAxis::setGridVisible(bool gridVisible)
{
    if(m_gridVisible != gridVisible)
    {
        m_gridVisible = gridVisible;
        emit gridVisibleChanged();
    }
}

bool CustomValueAxis::labelsVisible() const
{
    return m_labelsVisible;
}

void CustomValueAxis::setLabelsVisible(bool labelsVisible)
{
    if(m_labelsVisible != labelsVisible)
    {
        m_labelsVisible = labelsVisible;
        emit labelsVisibleChanged();
    }
}

QFont CustomValueAxis::labelsFont() const
{
    return m_labelsFont;
}

void CustomValueAxis::setLabelsFont(const QFont &labelsFont)
{
    if(m_labelsFont != labelsFont)
    {
        m_labelsFont = labelsFont;
        emit labelsFontChanged();
    }
}

QColor CustomValueAxis::labelsColor() const
{
    return m_labelsColor;
}

void CustomValueAxis::setLabelsColor(const QColor &labelsColor)
{
    if(m_labelsColor != labelsColor)
    {
        m_labelsColor = labelsColor;
        emit labelsColorChanged();
    }
}

bool CustomValueAxis::lineVisible() const
{
    return m_lineVisible;
}

void CustomValueAxis::setLineVisible(bool lineVisible)
{
    if(m_lineVisible != lineVisible)
    {
        m_lineVisible = lineVisible;
        emit lineVisibleChanged();
    }
}

QString CustomValueAxis::titleText() const
{
    return m_titleText;
}

void CustomValueAxis::setTitleText(const QString &titleText)
{
    if(m_titleText != titleText)
    {
        m_titleText = titleText;
        emit titleTextChanged();
    }
}

bool CustomValueAxis::titleVisible() const
{
    return m_titleVisible;
}

void CustomValueAxis::setTitleVisible(bool titleVisible)
{
    if(m_titleVisible != titleVisible)
    {
        m_titleVisible = titleVisible;
        emit titleVisibleChanged();
    }
}

int CustomValueAxis::decimals() const
{
    return m_decimals;
}

void CustomValueAxis::setDecimals(int decimals)
{
    if(m_decimals != decimals)
    {
        m_decimals = decimals;
        emit decimalsChanged();
    }
}

double CustomValueAxis::min() const
{
    return m_min;
}

void CustomValueAxis::setMin(double min)
{
    if(m_min < min || m_min > min)
    {
        m_min = min;
        emit minChanged();
    }
}

double CustomValueAxis::max() const
{
    return m_max;
}

void CustomValueAxis::setMax(double max)
{
    if(m_max < max || m_max > max)
    {
        m_max = max;
        emit maxChanged();
    }
}

int CustomValueAxis::tickCount() const
{
    return m_tickCount;
}

void CustomValueAxis::setTickCount(int tickCount)
{
    if(tickCount < 2)
    {
        tickCount = 2;
    }
    if(m_tickCount != tickCount)
    {
        m_tickCount = tickCount;
        emit tickCountChanged();
    }
}
