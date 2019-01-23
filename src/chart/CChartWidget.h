#ifndef CCHARTWIDGET_H
#define CCHARTWIDGET_H

#include <QScrollArea>
#include <QWheelEvent>

class CChartWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CChartWidget(QScrollArea *pControlScroll, QWidget *parent = nullptr);

    //设置X轴最小值
    bool setXMinValue(double minX);
    //设置X轴最大值
    bool setXMaxValue(double maxX);
    //设置X轴最大最小值
    bool setXAxisValue(double minX, double maxX);
    //设置X轴两数据之间间隔的像素
    void setXScalePixel(int xScalePixel);
    //设置X轴两个刻度之间的数据间隔
    void setXScaleValue(double xScaleValue);

    //设置Y轴最小值
    bool setYMinValue(double minY);
    //设置Y轴最大值
    bool seYtMaxValue(double maxY);
    //设置Y轴最大最小值
    bool setYAxisValue(double minY, double maxY);
    //设置Y轴两数据之间间隔的像素
    void setYScalePixel(int yPixelValue);
    //设置Y轴两个刻度之间的数据间隔
    void setYScaleValue(double yScaleValue);

    //设置显示数据
    void setData(const QList<QPointF> &listData);
    //设置是否根据数据重置XY的最大最小值，默认为true
    void setResetScale(bool resetScale);

    //设置X轴单位
    void setXUnit(const QString &unit);
    //设置Y轴单位
    void setYUnit(const QString &unit);

    //增加数据
    void addData(const QPointF &pointF);

signals:

public slots:
    void slotClear();

protected:
    void paintEvent(QPaintEvent *event);
    void wheelEvent(QWheelEvent *event);

private:
    void updateCalculateWidth();

private:
    QScrollArea     *m_pControlScroll;  //控制本界面的滑动区域
    double          m_dScaled;          //当前放大缩小比例

    double          m_dMinX;            //一页的x最小值
    double          m_dMaxX;            //一页的x最大值
    int             m_iXScalePixel;     //X轴两数据之间间隔的像素
    double          m_dXScaleValue;     //X轴刻度间间隔的值
    int             m_iBottomInterval;  //X轴距下边界的距离
    int             m_iXTextDown;       //X轴文本下移像素
    int             m_iXTextLeft;       //X轴文本左移像素

    double          m_dMinY;            //一页的y最小值
    double          m_dMaxY;            //一页的y最大值
    int             m_iYScalePixel;     //Y轴两数据之间间隔的像素
    double          m_dYScaleValue;     //Y轴刻度间间隔的值
    int             m_iRightInterval;   //Y轴距右边界的距离
    int             m_iYTextRight;      //Y轴文本右移像素

    double          m_dRadius;          //数据点的半径

    int             m_iTopInterval;      //上部间隔像素
    int             m_iLeftInterval;     //左部间隔像素

    QList<QPointF>  m_listValue;        //数据
    bool            m_bReteScale;     //是否根据数据重置刻度
    bool            m_bAdaptiveInterface;   //自适应界面

    int             m_iCalculateWidth;  //根据数据计算出的宽度
    int             m_iCalculateHeight; //根据数据计算出的高度

    QString         m_strXUnit;         //X轴单位
    QString         m_strYUnit;         //Y轴单位
};

#endif // CCHARTWIDGET_H
