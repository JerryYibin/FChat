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
    connect(ui->frequencyFace, SIGNAL(onceTimeChanged(int)), this, SIGNAL(onceTimeChanged(int)));
    connect(ui->frequencyFace, SIGNAL(intervalChanged(int)), this, SIGNAL(intervalChanged(int)));
    connect(ui->frequencyFace, SIGNAL(inputAmplitudeChanged(double)), this, SIGNAL(inputAmplitudeChanged(double)));
    connect(ui->frequencyFace, SIGNAL(amplitudeCompensation()), this, SIGNAL(amplitudeCompensation()));
    connect(ui->frequencyFace, SIGNAL(alarmReset()), this, SIGNAL(alarmReset()));

    m_pChartWidget = new CustomChartView(ui->scrollArea);

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

//    connect(ui->frequencyFace, SIGNAL(stopSig()), m_pChartWidget, SLOT(slotClear()));
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

void FrequencyDisplay::setStartBtnEditValid()
{
    ui->frequencyFace->setStartEditValid();
}

void FrequencyDisplay::setPauseBtnEditValue()
{
    ui->frequencyFace->setPauseEditValid();
}

void FrequencyDisplay::setStopBtnEditValue()
{
    ui->frequencyFace->setStopEditValid();
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

void FrequencyDisplay::setStateColor(const QString &color)
{
    ui->frequencyFace->setStateColor(color);
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

CustomChartView *FrequencyDisplay::chartView()
{
    return m_pChartWidget;
}

void FrequencyDisplay::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
