#include "FrequencyDisplay.h"
#include "ui_FrequencyDisplay.h"

#include "src/information/CFrequencyface.h"

#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QSplitter>

FrequencyDisplay::FrequencyDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FrequencyDisplay)
{
    ui->setupUi(this);

    connect(ui->frequencyFace, SIGNAL(startSig()), this, SIGNAL(startSig()));
    connect(ui->frequencyFace, SIGNAL(stopSig()), this, SIGNAL(stopSig()));
    connect(ui->frequencyFace, SIGNAL(pauseSig()), this, SIGNAL(pauseSig()));
    connect(ui->frequencyFace, SIGNAL(updateFrequencySig()), this, SIGNAL(updateFrequencySig()));
    connect(ui->frequencyFace, SIGNAL(endUpdateFrequencySig()), this, SIGNAL(endUpdateFrequencySig()));

    m_pChartWidget = new CChartWidget(ui->scrollArea, this);
    ui->scrollArea->setWidget(m_pChartWidget);

    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setMargin(0);
    pLayout->setSpacing(0);
    this->setLayout(pLayout);
    QSplitter *pSplitter = new QSplitter(Qt::Horizontal, this);
    pSplitter->addWidget(ui->frequencyFace);
    pSplitter->addWidget(ui->chartWidget);
    pSplitter->setStretchFactor(0, 1);
    pSplitter->setStretchFactor(1, 5);
    pLayout->addWidget(pSplitter);


//    QList<QPointF> listValue;
//    for(int i = 0; i < 121; i++)
//    {
//        listValue.append(QPointF(i, 19000 + i*15));
//    }
//    m_pChartWidget->setData(listValue);
    connect(ui->frequencyFace, SIGNAL(stopSig()), m_pChartWidget, SLOT(slotClear()));
}

FrequencyDisplay::~FrequencyDisplay()
{
    delete m_pChartWidget;
    delete ui;
}

void FrequencyDisplay::setSerialNum(const QString serialNum)
{
    ui->frequencyFace->setSerialNum(serialNum);
}

void FrequencyDisplay::setIntermittentvibrationInfo(const int onceTime, const int intervalTime, const double inputAmplitude)
{
    ui->frequencyFace->setIntermittentvibrationInfo(onceTime, intervalTime, inputAmplitude);
}

void FrequencyDisplay::setTheActualParameters(const double amplitude, const double frequency, const int totalTime)
{
    ui->frequencyFace->setTheActualParameters(amplitude, frequency, totalTime);
}

void FrequencyDisplay::setOfflineAndWorkTime(const int offl, const int wtt)
{
    ui->frequencyFace->setOfflineAndWorkTime(offl, wtt);
}

void FrequencyDisplay::setTwoParameter(const double param1, const double param2)
{
    ui->frequencyFace->setTwoParameter(param1, param2);
}

void FrequencyDisplay::setRecordSavePath(const QString savePath)
{
    ui->frequencyFace->setRecordSavePath(savePath);
}

void FrequencyDisplay::setPgSpectrumPath(const QString path)
{
    ui->frequencyFace->setPgSpectrumPath(path);
}

void FrequencyDisplay::setResonanceFrequency(double resonanceFrequency)
{
    ui->frequencyFace->setResonanceFrequency(resonanceFrequency);
}

void FrequencyDisplay::setEditValid(bool valid)
{
    ui->frequencyFace->setEditvaild(valid);
}

void FrequencyDisplay::chooseCvn()
{
    ui->frequencyFace->chooseCvn();
}

void FrequencyDisplay::chooseInv()
{
    ui->frequencyFace->chooseInv();
}

void FrequencyDisplay::choosePvn()
{
    ui->frequencyFace->choosePvn();
}

void FrequencyDisplay::chooseFol(bool checked)
{
    ui->frequencyFace->chooseFol(checked);
}

void FrequencyDisplay::chooseWtm(bool checked)
{
    ui->frequencyFace->chooseWtm(checked);
}

void FrequencyDisplay::getSerialNumAndSavePath(QString &serialNum, QString &savePath)
{
    ui->frequencyFace->getSerialNumAndSavePath(serialNum, savePath);
}

void FrequencyDisplay::getIntermittentvibrationInfo(int &onceTime, int &intervalTime, double &inputAmplitude)
{
    ui->frequencyFace->getIntermittentvibrationInfo(onceTime, intervalTime, inputAmplitude);
}

void FrequencyDisplay::getPgSpectrumPath(QString &vsPath)
{
    ui->frequencyFace->getPgSpectrumPath(vsPath);
}

void FrequencyDisplay::getOffline(int &offline)
{
    ui->frequencyFace->getOffline(offline);
}

void FrequencyDisplay::getWorkTotalTime(int &workTotalTime)
{
    ui->frequencyFace->getWorkTotalTime(workTotalTime);
}

void FrequencyDisplay::getParameterInfo(double &paramOne, double &paramTwo)
{
    ui->frequencyFace->getParameterInfo(paramOne, paramTwo);
}

void FrequencyDisplay::abnormalPause()
{
    ui->frequencyFace->abnormalPause();
}

void FrequencyDisplay::resetBtState()
{
    ui->frequencyFace->resetBtState();
}

bool FrequencyDisplay::ContinuousVibrationState()
{
    return ui->frequencyFace->ContinuousVibrationState();
}

bool FrequencyDisplay::IntermittentVibrationState()
{
    return  ui->frequencyFace->IntermittentVibrationState();
}

bool FrequencyDisplay::programVibrationState()
{
    return ui->frequencyFace->programVibrationState();
}

bool FrequencyDisplay::FrequencyOlVibrationState()
{
    return ui->frequencyFace->FrequencyOlVibrationState();
}

bool FrequencyDisplay::workTimeVibrationState()
{
    return ui->frequencyFace->workTimeVibrationState();
}

bool FrequencyDisplay::setXMinValue(double minX)
{
    return m_pChartWidget->setXMinValue(minX);
}

bool FrequencyDisplay::setXMaxValue(double maxX)
{
    return m_pChartWidget->setXMaxValue(maxX);
}

bool FrequencyDisplay::setXAxisValue(double minX, double maxX)
{
    return m_pChartWidget->setXAxisValue(minX, maxX);
}

void FrequencyDisplay::setXScalePixel(int xScalePixel)
{
    m_pChartWidget->setXScalePixel(xScalePixel);
}

void FrequencyDisplay::setXScaleValue(double xScaleValue)
{
    m_pChartWidget->setXScaleValue(xScaleValue);
}

bool FrequencyDisplay::setYMinValue(double minY)
{
    return m_pChartWidget->setYMinValue(minY);
}

bool FrequencyDisplay::seYtMaxValue(double maxY)
{
    return m_pChartWidget->seYtMaxValue(maxY);
}

bool FrequencyDisplay::setYAxisValue(double minY, double maxY)
{
    return m_pChartWidget->setYAxisValue(minY, maxY);
}

void FrequencyDisplay::setYScalePixel(int yPixelValue)
{
    m_pChartWidget->setYScalePixel(yPixelValue);
}

void FrequencyDisplay::setYScaleValue(double yScaleValue)
{
    m_pChartWidget->setYScaleValue(yScaleValue);
}

void FrequencyDisplay::setData(const QList<QPointF> &listData)
{
    m_pChartWidget->setData(listData);
}

void FrequencyDisplay::setResetScale(bool resetScale)
{
    m_pChartWidget->setResetScale(resetScale);
}

void FrequencyDisplay::addData(const QPointF &pointF)
{
    m_pChartWidget->addData(pointF);
}

void FrequencyDisplay::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
