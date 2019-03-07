#ifndef CUSTOMXYSERIES_H
#define CUSTOMXYSERIES_H

#include <QObject>
#include <QColor>
#include <QPointF>
#include <QList>

#include "CustomValueAxis.h"

struct TagSeriesTitle
{
    QString text;
    QColor  color;
    int     space;
    double  width;
    double  height;
    int     row;

    TagSeriesTitle()
    {
        space = 4;
    }
};

struct TagSeriesPoint
{
    QList<QPointF> listPoint;
    QColor         color;
    double         width;
    bool           visible;
};

class CustomXYSeries : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QList<QPointF> listData READ listData WRITE setListData NOTIFY listDataChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(CustomValueAxis* axisX READ axisX WRITE setAxisX NOTIFY axisXChanged)
    Q_PROPERTY(CustomValueAxis* axisY READ axisY WRITE setAxisY NOTIFY axisYChanged)
    Q_PROPERTY(CustomValueAxis* axisYRight READ axisYRight WRITE setAxisYRight NOTIFY axisYRightChanged)
    Q_PROPERTY(bool visible READ visible WRITE setVisible NOTIFY visibleChanged)
    Q_PROPERTY(bool pointsVisible READ pointsVisible WRITE setPointsVisible NOTIFY pointsVisibleChanged)
    Q_PROPERTY(bool axisYIsLeft READ axisYIsLeft WRITE setAxisYIsLeft NOTIFY axisYIsLeftChanged)

public:
    explicit CustomXYSeries(QObject *parent = Q_NULLPTR);

    Q_INVOKABLE void replace(QList<QPointF> points);
    Q_INVOKABLE void replace(QVector<QPointF> points);

    Q_INVOKABLE void clear();

    const QList<QPointF>& constListData() const;

//chart view transfer
public:
    friend class CustomChartView;
private:
    TagSeriesTitle* getTitleInfo();
    TagSeriesPoint* getDrawPointInfo();

public:
    QList<QPointF> listData() const;
    void setListData(const QList<QPointF> &listData);

    QString name() const;
    void setName(const QString &name);

    double width() const;
    void setWidth(double width);

    QColor color() const;
    void setColor(const QColor &color);

    CustomValueAxis *axisX() const;
    void setAxisX(CustomValueAxis *axisX);

    CustomValueAxis *axisY() const;
    void setAxisY(CustomValueAxis *axisY);

    CustomValueAxis *axisYRight() const;
    void setAxisYRight(CustomValueAxis *axisYRight);

    bool visible() const;
    void setVisible(bool visible);

    bool pointsVisible() const;
    void setPointsVisible(bool pointsVisible);

    bool axisYIsLeft() const;
    void setAxisYIsLeft(bool axisYIsLeft);

signals:
    void listDataChanged();
    void nameChanged();
    void widthChanged();
    void colorChanged();
    void axisXChanged();
    void axisYChanged();
    void axisYRightChanged();
    void visibleChanged();
    void pointsVisibleChanged();
    void axisYIsLeftChanged();

private:
    QList<QPointF>      m_listData;
    QString             m_name;
    double              m_width;
    QColor              m_color;
    CustomValueAxis*    m_axisX;
    CustomValueAxis*    m_axisY;
    CustomValueAxis*    m_axisYRight;
    bool                m_visible;
    bool                m_pointsVisible;
    bool                m_axisYIsLeft;
    TagSeriesTitle      m_titleInfo;
    TagSeriesPoint      m_drawPointInfo;
    int                 m_iTime;
};

#endif // CUSTOMXYSERIES_H
