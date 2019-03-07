#ifndef CUSTOMCHARTVIEW_H
#define CUSTOMCHARTVIEW_H

#include <QWidget>
#include <QRect>
#include <QPainter>
#include <QList>
#include <QPointF>

class CustomXYSeries;
class CustomValueAxis;
struct TagAxisInfo;
struct TagSeriesTitle;
struct TagSeriesPoint;

class CustomChartView : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QRectF plotArea READ plotArea NOTIFY plotAreaChanged)
    Q_PROPERTY(int leftMargin READ leftMargin WRITE setLeftMargin NOTIFY leftMarginChanged)
    Q_PROPERTY(int topMargin READ topMargin WRITE setTopMargin NOTIFY topMarginChanged)
    Q_PROPERTY(int rightMargin READ rightMargin WRITE setRightMargin NOTIFY rightMarginChanged)
    Q_PROPERTY(int bottomMargin READ bottomMargin WRITE setBottomMargin NOTIFY bottomMarginChanged)

    Q_PROPERTY(int titleRectWidth READ titleRectWidth WRITE setTitleRectWidth NOTIFY titleRectWidthChanged)
    Q_PROPERTY(QFont titleFont READ titleFont WRITE setTitleFont NOTIFY titleFontChanged)
    Q_PROPERTY(int titleSpace READ titleSpace WRITE setTitleSpace NOTIFY titleSpaceChanged)
    Q_PROPERTY(int titleRowSpace READ titleRowSpace WRITE setTitleRowSpace NOTIFY titleRowSpaceChanged)

    Q_PROPERTY(int axisSpace READ axisSpace WRITE setAxisSpace NOTIFY axisSpaceChanged)
    Q_PROPERTY(int scaleWidth READ scaleWidth WRITE setScaleWidth NOTIFY scaleWidthChanged)

public:
    CustomChartView(QWidget *parent = Q_NULLPTR);

    Q_INVOKABLE CustomXYSeries* series(const QString &name);

public:
    QRectF plotArea() const;
    void setPlotArea(const QRectF &plotArea);

    int leftMargin() const;
    void setLeftMargin(int leftMargin);
    int rightMargin() const;
    void setRightMargin(int rightMargin);
    int topMargin() const;
    void setTopMargin(int topMargin);
    int bottomMargin() const;
    void setBottomMargin(int bottomMargin);

    int titleRectWidth();
    void setTitleRectWidth(int titleRectWidth);
    QFont titleFont();
    void setTitleFont(QFont titleFont);
    int titleSpace();
    void setTitleSpace(int titleSpace);
    int titleRowSpace();
    void setTitleRowSpace(int titleRowSpace);

    int axisSpace();
    void setAxisSpace(int axisSpace);
    double scaleWidth();
    void setScaleWidth(double scaleWidth);
    int textScaleSpace();
    void setTextScaleSpace(int textScaleSpace);
    QColor gridColor();
    void setGridColor(QColor color);

    void addXYSeries(CustomXYSeries *customXYSeries);

signals:
    void plotAreaChanged();

    void leftMarginChanged();
    void rightMarginChanged();
    void topMarginChanged();
    void bottomMarginChanged();

    void titleRectWidthChanged();
    void titleFontChanged();
    void titleSpaceChanged();
    void titleRowSpaceChanged();

    void axisSpaceChanged();
    void scaleWidthChanged();
    void textScaleSpaceChanged();
    void gridColorChanged();

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void slotChildItemsChanged();

    void slotSeriesDataChanged();
    void slotSeriesNameChanged();
    void slotSeriesWidthChanged();
    void slotSeriesColorChanged();
    void slotSeriesAxisXChanged();
    void slotSeriesAxisYChanged();
    void slotSeriesAxisYRightChanged();
    void slotSeriesVisibleChanged();
    void slotSeriesPointsVisibleChanged();

    void slotAxisColorChanged();
    void slotAxisGridVisibleChanged();
    void slotAxisLabelsVisibleChanged();
    void slotAxisLabelsFontChanged();
    void slotAxisLabelsColorChanged();
    void slotAxisLineVisibleChanged();
    void slotAxisTitleTextChanged();
    void slotAxisTitleVisibleChanged();
    void slotAxisDecimalsChanged();
    void slotAxisMinChanged();
    void slotAxisMaxChanged();
    void slotAxisTickCountChanged();

private:
    void updatePaintData();

    void updateAxis();
    void updateAxisInfo();

    void axisToAxisInfo(const CustomValueAxis& valueAxis, TagAxisInfo &axisInfo, int axisType);
    void calculatePlotArea();
    void calculateAxisInfo();
    void calculateSerisData();

    void updatePlotAreaLeft(double &plotAreaLeftMargin);
    void updatePlotAreaRight(double &plotAreaRightMargin);
    void updatePlotAreaTop(double &plotAreaTopMargin, double plotAreaWidth);
    void updatePlotAreaBottom(double &plotAreaBottomMargin);

    void updateXAxisInfo();
    void updateYAxisInfo();
    void updateYRightAxisInfo();

    void updateSerisTitleData();
    void serisToSerisTitle(const CustomXYSeries &series, TagSeriesTitle &seriesTitle);
    bool updateSerisData(const CustomXYSeries &series, TagSeriesPoint &seriesPoint);

    void connectBySeries(CustomXYSeries *pXYSeries);
    void connectByAxis(CustomValueAxis *pAxis);
    void disconnectByAxis();

private:
    QRectF                                  m_plotArea;
    QList<CustomXYSeries*>                  m_listXYSeries;
    QList<CustomValueAxis*>                 m_listXAxis;
    QList<TagAxisInfo*>                     m_listXAxisInfo;
    QList<CustomValueAxis*>                 m_listYAxis;
    QList<TagAxisInfo*>                     m_listYAxisInfo;
    QList<CustomValueAxis*>                 m_listYRightAxis;
    QList<TagAxisInfo*>                     m_listYRightAxisInfo;

    QList<TagSeriesTitle*>                  m_listSeriesTitle;
    QList<TagSeriesPoint*>                  m_listSeriesData;

    int                                     m_iLeftMargin;
    int                                     m_iTopMargin;
    int                                     m_iRightMargin;
    int                                     m_iBottomMargin;
    int                                     m_iTitleRectWidth;
    int                                     m_iAxisSpace;
    int                                     m_iTitleSpace;
    int                                     m_iTitleRowSpace;

    QSizeF                                  m_thisSize;
    int                                     m_iTitleRow;
    double                                  m_dTitleWidth;

    QFont                                   m_titleFont;
    double                                  m_scaleWidth;
    int                                     m_textScaleSpace;
    QColor                                  m_gridColor;

    QObjectList                             m_listChildSeries;
};

#endif // CUSTOMCHARTVIEW_H
