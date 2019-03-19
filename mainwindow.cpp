#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPointF>
#include <QMessageBox>
#include <QIcon>
QString MainWindow::constHistoryPath = "";
QString MainWindow::constHistoryFileName = "";
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_iTime(0)
{
    ui->setupUi(this);
//    this->setWindowFlag(Qt::WindowMinMaxButtonsHint, false);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("超高频超声波振动系统");
    this->setWindowIcon(QIcon(":/image/components/frequency.png"));
    ui->headTitle->setTitle("超高频超声波振动系统  1.0.3");
    ui->headTitle->setImage(QImage(":/image/components/frequency.png"));
    connect(ui->headTitle, SIGNAL(signalBtnClicked()), this, SLOT(slotCloseClicked()));
    connect(ui->headTitle, SIGNAL(signalMinimizeClicked()), this, SLOT(slotMinimizeClicked()));

    m_pFrequencyDisplay = new FrequencyDisplay(this);
    ui->verticalLayout->addWidget(m_pFrequencyDisplay);
    m_pFrequencyDisplay->chooseCvn();
    connect(m_pFrequencyDisplay, SIGNAL(startSig()), this, SLOT(slotStart()));
    connect(m_pFrequencyDisplay, SIGNAL(stopSig()), this, SLOT(slotStop()));
    connect(m_pFrequencyDisplay, SIGNAL(pauseSig()), this, SLOT(slotPause()));
    connect(m_pFrequencyDisplay, SIGNAL(updateFrequencySig()), this, SLOT(slotResonanceFrequencyPressed()));
    connect(m_pFrequencyDisplay, SIGNAL(endUpdateFrequencySig()), this, SLOT(slotResonanceFrequencyReleased()));
    connect(m_pFrequencyDisplay, SIGNAL(amplitudeCompensation()), this, SLOT(slotAmplitudeCompensation()));
    connect(m_pFrequencyDisplay, SIGNAL(alarmReset()), this, SLOT(slotAlarmReset()));
    connect(m_pFrequencyDisplay, SIGNAL(inputAmplitudeChanged(double)), this, SLOT(slotInputAmplitudeChanged(double)));
    m_pFrequencyDisplay->setEditValid(true);
    initializeGraphSetting();

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
    m_pFrequencyDisplay->setStateColor("blue");
    if(m_pOpcUaClient->alarmState() == true)
    {
        QMessageBox messageBox;
        m_pFrequencyDisplay->setStateColor("red");
        messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
        messageBox.setWindowTitle("警告");
        messageBox.setText("请点击复位按钮复位设备报警。");
        messageBox.exec();
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
    delete m_pFreqXYSeries;
    delete m_pAmpXYSeries;
    delete m_pAmpLeftYAxis;
    delete m_pFreqRightYAxis;
    delete m_pTimeXAxis;
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
            m_pFrequencyDisplay->setStateColor("blue");
            return;
        }

        if((m_SecondCounter % 100) == 0) //1sec
        {
            m_SecondCounter = 0;
            if(m_FreqPoints.size() > 120)
            {
                m_FreqPoints.removeFirst();
                m_AmpPoints.removeFirst();
                m_pTimeXAxis->setMax(m_pProcessControl->m_RealTimeUpdate.m_totalTime);
                m_pTimeXAxis->setMin(m_pProcessControl->m_RealTimeUpdate.m_totalTime - 120);

            }
            m_FreqPoints.append(QPointF(m_pProcessControl->m_RealTimeUpdate.m_totalTime, m_pProcessControl->m_RealTimeUpdate.m_frequency));
            m_AmpPoints.append(QPointF(m_pProcessControl->m_RealTimeUpdate.m_totalTime, m_pProcessControl->m_RealTimeUpdate.m_amplitude));
            m_pFreqXYSeries->replace(m_FreqPoints);
            m_pAmpXYSeries->replace(m_AmpPoints);
            m_pProcessControl->m_RealTimeUpdate.m_totalTime++;
            qInfo()<<m_pProcessControl->m_RealTimeUpdate.m_amplitude<<" "
                  <<m_pProcessControl->m_RealTimeUpdate.m_frequency<<" "
                 <<m_pProcessControl->m_RealTimeUpdate.m_totalTime;
        }
    }
}

void MainWindow::slotFrequencyUpdate(double tmpFrequency)
{
    m_pFrequencyDisplay->setResonanceFrequency(tmpFrequency);
}

void MainWindow::slotInputAmplitudeChanged(double inputAmplitude)
{
    m_pProcessControl->processData.amplitudeSetting = inputAmplitude;
    m_pProcessControl->SetNewUpdateAmplitude();
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
        isPause = false;
        if(m_pProcessControl->processData.processMode == ProcessControl::ContinueMode)
            m_pOpcUaClient->machineWriteRun(true);
        m_pFrequencyDisplay->setEditValid(false);
        m_iTime = this->startTimer(10);
        m_pFrequencyDisplay->setStateColor("green");
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

    if(m_iTime != 0)
    {
        this->killTimer(m_iTime);
    }
    m_pFrequencyDisplay->setEditValid(false);
    m_SecondCounter = 0;
    m_FreqPoints.clear();
    m_AmpPoints.clear();

    m_iTime = this->startTimer(10);
    m_pFrequencyDisplay->setStateColor("green");

    constHistoryPath = m_pProcessControl->processData.RecordSavingPath + "/log";
    QDir dir(constHistoryPath);
    if(!dir.exists())
    {
        dir.mkdir(dir.absolutePath());
    }
    constHistoryFileName = constHistoryPath + "/" + QDateTime::currentDateTime().toString("yyyyMMddhhmmss") + ".log";
    QString strModuleName = QString("Module Name: %1").arg(m_pProcessControl->processData.serialNum);
    QString strHeader = QString("%1 %2 %3 %4").arg("Actual Amplitude(%)").arg("Actual Frequency(Hz)").arg("Total Work Time(s)").arg("Sampled Time");

    // 输出信息至文件中（读写、追加形式）
    QFile file(constHistoryFileName);
    file.open(QIODevice::ReadWrite | QIODevice::Append);
    QTextStream stream(&file);
    stream << strModuleName << "\r\n";
    stream << strHeader << "\r\n";
    file.flush();
    file.close();
    qInstallMessageHandler(MainWindow::myMessageOutput);
}

void MainWindow::slotStop()
{
    m_pFrequencyDisplay->setEditValid(true);
    if(!m_pOpcUaClient->connected())
        return;
    stopSaveRecord();
    m_pProcessControl->m_RealTimeUpdate.m_totalTime = 0;
    for(int i = 0; i < 5; i++)
        m_pOpcUaClient->machineWriteRun(false);
    m_pFrequencyDisplay->setStateColor("blue");
    m_pTimeXAxis->setMax(120);
    m_pTimeXAxis->setMin(0);
    isPause = false;
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
    m_pFrequencyDisplay->setPauseBtnEditValue();
    for(int i = 0; i < 5; i++)
        m_pOpcUaClient->machineWriteRun(false);
    m_pFrequencyDisplay->setStateColor("blue");
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

void MainWindow::slotMinimizeClicked()
{
    this->lower();
}

void MainWindow::slotAlarmStateUpdate(bool value)
{
    if(value == true)
    {
        m_pFrequencyDisplay->setStateColor("red");
        QMessageBox messageBox;
        messageBox.setWindowIcon(QIcon(":/image/components/warning.png"));
        messageBox.setWindowTitle("警告");
        messageBox.setText("请点击复位按钮复位设备报警。");
        messageBox.exec();
//        if(messageBox.exec())
//            m_pOpcUaClient->machineWriteReset(true);
    }
    else
    {
        m_pFrequencyDisplay->setStateColor("blue");
    }
}

void MainWindow::slotAmplitudeCompensation()
{
    qDebug()<<"calibration Amplitude";
    m_pProcessControl->CalibrationAmplitude();
}

void MainWindow::slotAlarmReset()
{
    qDebug()<<"alarmReset";
    m_pOpcUaClient->machineWriteReset(true);
    m_pFrequencyDisplay->setStateColor("blue");
}

/***********************************************private****************************/
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

    if((m_pFrequencyDisplay->FrequencyOlVibrationState() == true) &&
            (m_pFrequencyDisplay->workTimeVibrationState() == true))
        m_pProcessControl->processData.controlLimit = ProcessControl::BothLimit;
    else if(m_pFrequencyDisplay->FrequencyOlVibrationState() == true)
        m_pProcessControl->processData.controlLimit = ProcessControl::FrequencyLimit;
    else if(m_pFrequencyDisplay->workTimeVibrationState() == true)
        m_pProcessControl->processData.controlLimit = ProcessControl::TimeLimit;
    else
        m_pProcessControl->processData.controlLimit = ProcessControl::Undefine;
}

void MainWindow::initializeGraphSetting()
{
    m_pTimeXAxis = new CustomValueAxis(m_pFrequencyDisplay);
    m_pTimeXAxis->setMax(120);
    m_pTimeXAxis->setMin(0);
    m_pTimeXAxis->setTickCount(13);
    m_pTimeXAxis->setDecimals(0);
    m_pTimeXAxis->setColor("black");
    m_pTimeXAxis->setTitleText("Time(s)");
    m_pTimeXAxis->setTitleVisible(true);
    m_pAmpLeftYAxis = new CustomValueAxis(m_pFrequencyDisplay);
    m_pAmpLeftYAxis->setMin(0);
    m_pAmpLeftYAxis->setMax(100);
    m_pAmpLeftYAxis->setTickCount(11);
    m_pAmpLeftYAxis->setDecimals(2);
    m_pAmpLeftYAxis->setColor("#f03e3e");
    m_pAmpLeftYAxis->setTitleText("Amplitude(%)");
    m_pAmpLeftYAxis->setTitleVisible(true);
    m_pFreqRightYAxis = new CustomValueAxis(m_pFrequencyDisplay);
    m_pFreqRightYAxis->setMin(19000);
    m_pFreqRightYAxis->setMax(21000);
    m_pFreqRightYAxis->setTickCount(21);
    m_pFreqRightYAxis->setDecimals(0);
    m_pFreqRightYAxis->setColor("#4285f4");
    m_pFreqRightYAxis->setTitleText("Frequency(Hz)");
    m_pFreqRightYAxis->setTitleVisible(true);
    m_pAmpXYSeries = new CustomXYSeries(m_pFrequencyDisplay);
    m_pAmpXYSeries->setAxisX(m_pTimeXAxis);
    m_pAmpXYSeries->setAxisY(m_pAmpLeftYAxis);
    m_pAmpXYSeries->setColor("#f03e3e");
    m_pAmpXYSeries->setName("Amplitude");
    m_pFreqXYSeries = new CustomXYSeries(m_pFrequencyDisplay);
    m_pFreqXYSeries->setAxisX(m_pTimeXAxis);
    m_pFreqXYSeries->setAxisYRight(m_pFreqRightYAxis);
    m_pFreqXYSeries->setColor("#4285f4");
    m_pFreqXYSeries->setName("Frequency");
    m_pFrequencyDisplay->chartView()->addXYSeries(m_pFreqXYSeries);
    m_pFrequencyDisplay->chartView()->addXYSeries(m_pAmpXYSeries);
    m_FreqPoints.clear();
    m_AmpPoints.clear();
    m_pFreqXYSeries->replace(m_FreqPoints);
    m_pAmpXYSeries->replace(m_AmpPoints);
}

void MainWindow::myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)
    // 加锁
    static QMutex mutex;
    mutex.lock();

    QByteArray localMsg = msg.toLocal8Bit();

    QString strMsg("");
    int tmptype = type;
    bool isNeed2Record = false;
    switch(tmptype)
    {
    case QtDebugMsg:
        strMsg = QString("Debug:");
        break;
    case QtWarningMsg:
        strMsg = QString("Warning:");
        break;
    case QtCriticalMsg:
        strMsg = QString("Critical:");
        break;
    case QtFatalMsg:
        strMsg = QString("Fatal:");
        break;
    case QtInfoMsg:
        strMsg = QString("Record:");
        isNeed2Record = true;
        break;
    }
    if(isNeed2Record == true)
    {

        // 设置输出信息格式
        QString strDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz");
        QString strMessage = QString("%1 %2").arg(localMsg.constData()).arg(strDateTime);

        // 输出信息至文件中（读写、追加形式）
        QFile file(constHistoryFileName);
        file.open(QIODevice::ReadWrite | QIODevice::Append);
        QTextStream stream(&file);
        stream << strMessage << "\r\n";
        file.flush();
        file.close();
    }

    // 解锁
    mutex.unlock();
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

//    if(m_pDocument)
//    {
//        m_pDocument->write("D1", "文件保存时间:\n"
//                           + QDateTime::currentDateTime().toString("yyyy/MM/d hh:mm:ss:zzz"));
//        m_pDocument->save();
//        delete m_pDocument;
//        m_pDocument = nullptr;
//    }
//    m_iDocumentCurrentRow = 4;
}

