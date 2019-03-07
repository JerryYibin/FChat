#ifndef CUSTOMVALUEAXIS_H
#define CUSTOMVALUEAXIS_H

#include <QObject>
#include <QColor>
#include <QFont>
#include <QVariant>

struct TagAxisInfo
{
    enum E_AXIS_TYPE
    {
        EAT_X_AXIS,
        EAT_Y_LEFT_AXIS,
        EAT_Y_RIGHT_AXIS,
    };

    E_AXIS_TYPE     axisType;
    QColor          color;
    QColor          labelColor;
    bool            visible;
    bool            labelsVisible;
    bool            gridVisible;
    double          startValue;
    double          endValue;
    int             tickCount;
    double          scaleValue;
    int             decimals;
    QString         titleText;
    bool            titleVisible;
    QList<QString>  listScaleValue;
    double          textMaxWidth;
    double          textHeight;
    QFont           font;
    double          widthOrHeight;  //EST_X_AXIS->height  other->width
    double          pixelValue;
    QList<double>   listScalePos;   //EST_X_AXIS->x  other->y

    TagAxisInfo()
    {
        tickCount = 2;
        widthOrHeight = 0;
    }
};

class CustomValueAxis : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool gridVisible READ gridVisible WRITE setGridVisible NOTIFY gridVisibleChanged)
    Q_PROPERTY(bool labelsVisible READ labelsVisible WRITE setLabelsVisible NOTIFY labelsVisibleChanged)
    Q_PROPERTY(QFont labelsFont READ labelsFont WRITE setLabelsFont NOTIFY labelsFontChanged)
    Q_PROPERTY(QColor labelsColor READ labelsColor WRITE setLabelsColor NOTIFY labelsColorChanged)
    Q_PROPERTY(bool lineVisible READ lineVisible WRITE setLineVisible NOTIFY lineVisibleChanged)
    Q_PROPERTY(QString titleText READ titleText WRITE setTitleText NOTIFY titleTextChanged)
    Q_PROPERTY(bool titleVisible READ titleVisible WRITE setTitleVisible NOTIFY titleVisibleChanged)
    Q_PROPERTY(int decimals READ decimals WRITE setDecimals NOTIFY decimalsChanged)
    Q_PROPERTY(double min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(double max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(int tickCount READ tickCount WRITE setTickCount NOTIFY tickCountChanged)

public:
    explicit CustomValueAxis(QObject *parent = Q_NULLPTR);

    bool operator!=(const CustomValueAxis &other);
    void operator=(const CustomValueAxis &other);

//chart view transfer
public:
    friend class CustomChartView;
private:
    TagAxisInfo* getAxisInfo();

public:
    QColor color() const;
    void setColor(const QColor &color);

    bool gridVisible() const;
    void setGridVisible(bool gridVisible);

    bool labelsVisible() const;
    void setLabelsVisible(bool labelsVisible);

    QFont labelsFont() const;
    void setLabelsFont(const QFont &labelsFont);

    QColor labelsColor() const;
    void setLabelsColor(const QColor &labelsColor);

    bool lineVisible() const;
    void setLineVisible(bool lineVisible);

    QString titleText() const;
    void setTitleText(const QString &titleText);

    bool titleVisible() const;
    void setTitleVisible(bool titleVisible);

    int decimals() const;
    void setDecimals(int decimals);

    double min() const;
    void setMin(double min);

    double max() const;
    void setMax(double max);

    int tickCount() const;
    void setTickCount(int tickCount);

signals:
    void colorChanged();
    void gridVisibleChanged();
    void labelsVisibleChanged();
    void labelsFontChanged();
    void labelsColorChanged();
    void lineVisibleChanged();
    void titleTextChanged();
    void titleVisibleChanged();
    void decimalsChanged();
    void minChanged();
    void maxChanged();
    void tickCountChanged();

public slots:

private:
    QColor      m_color;
    bool        m_gridVisible;
    bool        m_labelsVisible;
    QFont       m_labelsFont;
    QColor      m_labelsColor;
    bool        m_lineVisible;
    QString     m_titleText;
    bool        m_titleVisible;
    int         m_decimals;
    double      m_min;
    double      m_max;
    int         m_tickCount;

    TagAxisInfo m_axisInfo;
};

#endif // CUSTOMVALUEAXIS_H
