#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPointF>
#include <QMessageBox>
#include <QIcon>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_iTime(0),
    m_pDocument(nullptr)
{
    ui->setupUi(this);
    this->setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("超高频超声波振动系统");
    this->setWindowIcon(QIcon(":/image/components/frequency.png"));
    ui->headTitle->setTitle("超高频超声波振动系统  1.0.1");
    ui->headTitle->setImage(QImage(":/image/components/frequency.png"));
    connect(ui->headTitle, SIGNAL(signalBtnClicked()), this, SLOT(slotCloseClicked()));

    m_pFrequencyDisplay = new FrequencyDisplay(this);
    ui->verticalLayout->addWidget(m_pFrequencyDisplay);
    m_pFrequencyDisplay->chooseCvn();
    connect(m_pFrequencyDisplay, SIGNAL(startSig()), this, SLOT(slotStart()));
    connect(m_pFrequencyDisplay, SIGNAL(stopSig()), this, SLOT(slotStop()));
    connect(m_pFrequencyDisplay, SIGNAL(pauseSig()), this, SLOT(slotPause()));
    connect(m_pFrequencyDisplay, SIGNAL(updateFrequencySig()), this, SLOT(slotResonanceFrequencyPressed()));
    connect(m_pFrequencyDisplay, SIGNAL(endUpdateFrequencySig()), this, SLOT(slotResonanceFrequencyReleased()));
    m_pOpcUaClient = OpcUaMachineBackend::Instance(this);
    if (m_pOpcUaClient->connected())
        m_pOpcUaClient->disconnectFromEndpoint();
    else
    {
//        m_pOpcUaClient->connectToEndpoint("opc.tcp://127.0.0.1:49320", 0);
        m_pOpcUaClient->connectToEndpoint("opc.tcp://192.168.0.10:2019", 0);
    }

    if(!m_pOpcUaClient->connected())
    {
        QMessageBox messageBox;
        messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
        messageBox.setWindowTitle("警告");
        messageBox.setText("请确保本机IP地址在192.168.0.x网段内，\n子网掩码255.255.255.0。");
        messageBox.exec();
    }

    m_pProcessControl = ProcessControl::Instance(this);
    double tmpAmplitude = m_pProcessControl->m_RealTimeUpdate.m_amplitude;
    double tmpFrequency = m_pProcessControl->m_RealTimeUpdate.m_frequency;
    double tmpTotalTime = m_pProcessControl->m_RealTimeUpdate.m_totalTime;
    m_pFrequencyDisplay->setTheActualParameters(tmpAmplitude, tmpFrequency, static_cast<int>(tmpTotalTime));
    m_pFrequencyDisplay->setResonanceFrequency(0.0);
    connect(m_pProcessControl, SIGNAL(signalResonanceFrequency(double)), this, SLOT(slotFrequencyUpdate(double)));
    connect(m_pOpcUaClient, SIGNAL(alarmOnlyReadChanged(bool)), this, SLOT(slotAlarmStateUpdate(bool)));
    m_SecondCounter = 0;
    if(m_pOpcUaClient->alarmState() == true)
    {
        QMessageBox messageBox;
        messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
        messageBox.setWindowTitle("警告");
        messageBox.setText("请点击确认按钮复位设备报警。");
        if(messageBox.exec())
            m_pOpcUaClient->machineWriteReset(true);
    }
    isPause = false;
}

MainWindow::~MainWindow()
{
    m_pOpcUaClient->machineWriteRun(false);
    if (m_pOpcUaClient->connected())
    {
        m_pOpcUaClient->machineWriteRun(false);
        m_pOpcUaClient->disconnectFromEndpoint();
    }
    delete m_pOpcUaClient;
    delete m_pFrequencyDisplay;
    delete ui;

    if(m_iTime != 0)
    {
        this->killTimer(m_iTime);
    }
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    double tmpAmplitude = m_pProcessControl->m_RealTimeUpdate.m_amplitude;
    double tmpFrequency = m_pProcessControl->m_RealTimeUpdate.m_frequency;
    double tmpTotalTime = m_pProcessControl->m_RealTimeUpdate.m_totalTime;
    if(event->timerId() == m_iTime)
    {
        m_SecondCounter++;
        tmpAmplitude = m_pProcessControl->m_RealTimeUpdate.m_amplitude;
        tmpFrequency = m_pProcessControl->m_RealTimeUpdate.m_frequency;
        tmpTotalTime = m_pProcessControl->m_RealTimeUpdate.m_totalTime;
        m_pFrequencyDisplay->setTheActualParameters(tmpAmplitude, tmpFrequency, static_cast<int>(tmpTotalTime));

        if(m_pProcessControl->CheckStrategyProcess() == false)
        {
            m_pFrequencyDisplay->setEditValid(true);
            stopSaveRecord();
            return;
        }

        if((m_SecondCounter % 100) == 0) //1sec
        {
            m_SecondCounter = 0;
            m_pFrequencyDisplay->addData(QPointF(m_pProcessControl->m_RealTimeUpdate.m_totalTime,
                                                 m_pProcessControl->m_RealTimeUpdate.m_frequency));
            m_pProcessControl->m_RealTimeUpdate.m_totalTime++;
            if(m_pDocument)
            {
                m_pDocument->write("A" + QString::number(m_iDocumentCurrentRow),
                                   m_pProcessControl->m_RealTimeUpdate.m_amplitude);
                m_pDocument->write("B" + QString::number(m_iDocumentCurrentRow),
                                   m_pProcessControl->m_RealTimeUpdate.m_frequency);
                m_pDocument->write("C" + QString::number(m_iDocumentCurrentRow),
                                   m_pProcessControl->m_RealTimeUpdate.m_totalTime);
                m_pDocument->write("D" + QString::number(m_iDocumentCurrentRow),
                                   QDateTime::currentDateTime().toString("yyyy/MM/d hh:mm:ss:zzz"));
                ++m_iDocumentCurrentRow;

                if(m_SecondCounter % 1000 == 0) //10sec
                {
                    m_pDocument->write("D1", "文件保存时间:\n"
                                       + QDateTime::currentDateTime().toString("yyyy/MM/d hh:mm:ss:zzz"));
                    m_pDocument->save();
                }
            }
        }
    }
}

void MainWindow::slotFrequencyUpdate(double tmpFrequency)
{
    m_pFrequencyDisplay->setResonanceFrequency(tmpFrequency);
}

void MainWindow::slotStart()
{
    QDir objPath;
    if(!m_pOpcUaClient->connected())
    {
        QMessageBox messageBox;
        messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
        messageBox.setWindowTitle("警告");
        messageBox.setText("请确保本机IP地址在192.168.0.x网段内，\n子网掩码255.255.255.0。");
        messageBox.exec();
        return;
    }
    if(isPause == true)
    {
        if(m_pProcessControl->processData.processMode == ProcessControl::ContinueMode)
            m_pOpcUaClient->machineWriteRun(true);
        return;
    }
    updateProcessModeAndControlLimit();
    m_pFrequencyDisplay->getIntermittentvibrationInfo(
                m_pProcessControl->processData.durationTimer,
                m_pProcessControl->processData.stopTimer,
                m_pProcessControl->processData.amplitudeSetting);
    m_pProcessControl->processData.durationTimer /= 10;
    m_pProcessControl->processData.stopTimer /= 10;
    m_pProcessControl->processData.durationTimerBakup =
            m_pProcessControl->processData.durationTimer;
    m_pProcessControl->processData.stopTimerBakup =
            m_pProcessControl->processData.stopTimer;
    m_pFrequencyDisplay->getOffline(
                m_pProcessControl->processData.lowerFrequencyLimit);
    m_pFrequencyDisplay->getWorkTotalTime(
                m_pProcessControl->processData.upperTimeLimit);
    m_pFrequencyDisplay->getParameterInfo(
                m_pProcessControl->processData.param1,
                m_pProcessControl->processData.param2);
    m_pFrequencyDisplay->getSerialNumAndSavePath(m_pProcessControl->processData.serialNum,
                                                 m_pProcessControl->processData.RecordSavingPath);
    m_pFrequencyDisplay->getPgSpectrumPath(m_pProcessControl->processData.ProgramAmplitudePath);
    m_pProcessControl->isScannFrequency = false;

    QDir recordDir;
    if(recordDir.exists(m_pProcessControl->processData.RecordSavingPath) == false)
    {
        QMessageBox messageBox;
        messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
        messageBox.setWindowTitle("警告");
        messageBox.setText("请确保保存文件路径是有效的。");
        messageBox.exec();
        return;
    }

    if(m_pProcessControl->processData.processMode == ProcessControl::ProgramMode)
    {
        if(recordDir.exists(m_pProcessControl->processData.ProgramAmplitudePath) == false)
        {
            QMessageBox messageBox;
            messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
            messageBox.setWindowTitle("警告");
            messageBox.setText("请确保导入文件路径是有效的。");
            messageBox.exec();
            return;
        }
        if(m_pProcessControl->processData.ProgramAmplitudePath.contains(".xlsx") == false)
        {
            QMessageBox messageBox;
            messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
            messageBox.setWindowTitle("警告");
            messageBox.setText("请确保导入文件是.xlsx后缀的Excel文件。");
            messageBox.exec();
            return;
        }
        if(objPath.exists(m_pProcessControl->processData.ProgramAmplitudePath) == false)
        {
            QMessageBox messageBox;
            messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
            messageBox.setWindowTitle("警告");
            messageBox.setText("请确保所要导入的文件在正确的目录下。");
            messageBox.exec();
            return;
        }
    }
    m_pProcessControl->LoadProgramedAmplitude2List();
    if(m_pProcessControl->processData.processMode == ProcessControl::ContinueMode)
        m_pOpcUaClient->machineWriteRun(true);
//    if(m_pProcessControl->processData.processMode == ProcessControl::IntervalMode)


    if(m_pDocument)
    {
        delete m_pDocument;
    }
    m_pDocument = new QXlsx::Document(m_pProcessControl->processData.RecordSavingPath
                                      + "\\"
                                      + QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz")
                                      + ".xlsx");
    m_pDocument->write("A1", "试件\n名称:");
    m_pDocument->write("B1", m_pProcessControl->processData.serialNum);
    m_pDocument->write("C1", "文件建立时间:\n"
                       + QDateTime::currentDateTime().toString("yyyy/MM/d hh:mm:ss:zzz"));
    m_pDocument->write("D1", "文件保存时间:\n"
                       + QDateTime::currentDateTime().toString("yyyy/MM/d hh:mm:ss:zzz"));
    m_pDocument->write("A3", "实际振幅（um）");
    m_pDocument->write("B3", "实际频率（Hz）");
    m_pDocument->write("C3", "累计时长（s）");
    m_pDocument->write("D3", "采集时间");
    m_pDocument->save();
    m_iDocumentCurrentRow = 4;

    if(m_iTime != 0)
    {
        this->killTimer(m_iTime);
    }
    m_pFrequencyDisplay->setEditValid(false);
    m_SecondCounter = 0;
    m_iTime = this->startTimer(10);
}

void MainWindow::slotStop()
{
    m_pFrequencyDisplay->setEditValid(true);
    if(!m_pOpcUaClient->connected())
        return;
    stopSaveRecord();
    for(int i = 0; i < 5; i++)
        m_pOpcUaClient->machineWriteRun(false);
}

void MainWindow::slotPause()
{
    if(!m_pOpcUaClient->connected())
        return;
    if(m_iTime != 0)
    {
        this->killTimer(m_iTime);
        m_iTime = 0;
        isPause = true;
    }
    m_pFrequencyDisplay->setEditValid(true);
    for(int i = 0; i < 5; i++)
        m_pOpcUaClient->machineWriteRun(false);
}

void MainWindow::slotResonanceFrequencyPressed()
{
    m_pProcessControl->isScannFrequency = true;
    m_pOpcUaClient->machineWriteSeek(true);
}

void MainWindow::slotResonanceFrequencyReleased()
{
    m_pProcessControl->isScannFrequency = false;
    m_pOpcUaClient->machineWriteSeek(false);
}

void MainWindow::slotCloseClicked()
{
    m_pOpcUaClient->machineWriteRun(false);
    this->close();
}

void MainWindow::slotAlarmStateUpdate(bool value)
{
    if(value == true)
    {
        QMessageBox messageBox;
        messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
        messageBox.setWindowTitle("警告");
        messageBox.setText("请点击确认按钮复位设备报警。");
        if(messageBox.exec())
            m_pOpcUaClient->machineWriteReset(true);
    }
}

/************************************************private****************************/
void MainWindow::updateProcessModeAndControlLimit()
{
    if(m_pFrequencyDisplay->ContinuousVibrationState() == true)
        m_pProcessControl->processData.processMode = ProcessControl::ContinueMode;
    else if(m_pFrequencyDisplay->IntermittentVibrationState() == true)
        m_pProcessControl->processData.processMode = ProcessControl::IntervalMode;
    else if(m_pFrequencyDisplay->programVibrationState() == true)
        m_pProcessControl->processData.processMode = ProcessControl::ProgramMode;
    else
        m_pProcessControl->processData.processMode = ProcessControl::ContinueMode;

    if(m_pFrequencyDisplay->FrequencyOlVibrationState() == true)
        m_pProcessControl->processData.controlLimit = ProcessControl::FrequencyLimit;
    else if(m_pFrequencyDisplay->workTimeVibrationState() == true)
        m_pProcessControl->processData.controlLimit = ProcessControl::TimeLimit;
    else if((m_pFrequencyDisplay->FrequencyOlVibrationState() == true) &&
            (m_pFrequencyDisplay->workTimeVibrationState() == true))
        m_pProcessControl->processData.controlLimit = ProcessControl::BothLimit;
    else
        m_pProcessControl->processData.controlLimit = ProcessControl::Undefine;
}

void MainWindow::stopSaveRecord()
{
    if(m_iTime != 0)
    {
        this->killTimer(m_iTime);
        m_iTime = 0;
        m_pProcessControl->m_RealTimeUpdate.m_totalTime = 0;
        m_pProcessControl->m_RealTimeUpdate.m_frequency = 0.0;
        m_pProcessControl->m_RealTimeUpdate.m_amplitude = 0;
    }

    if(m_pDocument)
    {
        m_pDocument->write("D1", "文件保存时间:\n"
                           + QDateTime::currentDateTime().toString("yyyy/MM/d hh:mm:ss:zzz"));
        m_pDocument->save();
        delete m_pDocument;
        m_pDocument = nullptr;
    }
    m_iDocumentCurrentRow = 4;
}

