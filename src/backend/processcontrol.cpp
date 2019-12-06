#include "processcontrol.h"
#include "opcuamachinebackend.h"
#include "xlsxdocument.h"
#include "xlsxcell.h"
#include <QDir>
#include <QPair>
ProcessControl* ProcessControl::_instance = nullptr;
ProcessControl* ProcessControl::Instance(QObject *parent)
{
    if(_instance == nullptr){
        _instance = new ProcessControl(parent);
    }
    return _instance;
}

ProcessControl::ProcessControl(QObject *parent):QObject(parent)
{
    isScannFrequency = false;
    m_AmplitudeDeviation = 1.0;
    m_RealTimeUpdate.m_amplitude = 0.0;
    m_RealTimeUpdate.m_frequency = 0.0;
    m_RealTimeUpdate.m_totalTime = 0;

    OpcUaMachineBackend* _pOpcUaClientObj = OpcUaMachineBackend::Instance(this);
    connect(_pOpcUaClientObj, SIGNAL(frequencyOnlyReadChanged(qint16)), this, SLOT(slotFrequencyUpdate(qint16)));
    connect(_pOpcUaClientObj, SIGNAL(amplitudeOnlyReadChanged(qint16)), this, SLOT(slotAmplitudeUpdate(qint16)));
}

void ProcessControl::slotFrequencyUpdate(qint16 tmpFrequency)
{
    m_RealTimeUpdate.m_frequency = HEX2FREQUENCY(tmpFrequency);
    if(isScannFrequency == true)
        emit signalResonanceFrequency(m_RealTimeUpdate.m_frequency);
}

void ProcessControl::slotAmplitudeUpdate(qint16 tmpAmplitude)
{
    m_RealTimeUpdate.m_amplitude = HEX2AMPLITUDE(tmpAmplitude) * m_AmplitudeDeviation* processData.param1 * processData.param2;
}

void ProcessControl::LoadProgramedAmplitude2List()
{
    OpcUaMachineBackend* _pOpcUaClientObj = OpcUaMachineBackend::Instance(this);
    qint16 tmpAmplitude;
    int iRow = 1;
    QXlsx::Document objExcel(processData.ProgramAmplitudePath);
    objExcel.selectSheet("Sheet1");
    QXlsx::Cell* _objCell = objExcel.cellAt(1, 1);
    QString strColumn1;
    QPair<int, int> tmpPair;
    bool isAmplitudeData = false;
    processData.programList.clear();
    switch(processData.processMode)
    {
    case ProcessControl::ContinueMode:
    case ProcessControl::IntervalMode:
        tmpAmplitude = AMPLITUDE2HEX(processData.amplitudeSetting);
        _pOpcUaClientObj->machineWriteAmplitude(tmpAmplitude);
        break;
    case ProcessControl::ProgramMode:
        processData.durationTimer = 0;
        strColumn1 = _objCell->value().toString();
        while (strColumn1.contains("停止") == false)
        {
            if(strColumn1.contains("延时启动"))
            {
                _objCell = objExcel.cellAt(iRow, 2);
                processData.DelayStartTimer = _objCell->value().toFloat() * 100;
            }
            else if(strColumn1.contains("振幅"))
            {
                isAmplitudeData = true;
            }
            else if(isAmplitudeData == true)
            {
                _objCell = objExcel.cellAt(iRow, 1);
                tmpPair.first = _objCell->value().toInt();
                _objCell = objExcel.cellAt(iRow, 2);
                tmpPair.second = _objCell->value().toDouble() * 100;
                processData.programList.append(tmpPair);
            }
            iRow++;
            _objCell = objExcel.cellAt(iRow, 1);
            strColumn1 = _objCell->value().toString();
        };
        break;
    default:
        tmpAmplitude = AMPLITUDE2HEX(processData.amplitudeSetting);
        _pOpcUaClientObj->machineWriteAmplitude(tmpAmplitude);
        break;
    }
}

bool ProcessControl::CheckStrategyProcess()
{
    bool bResult = true;
    OpcUaMachineBackend* _pOpcUaClientObj = OpcUaMachineBackend::Instance(this);
    qint16 tmpAmplitude;
    /**************************1 step ***********************************/
    switch(processData.controlLimit)
    {
    case ProcessControl::BothLimit:
        if(m_RealTimeUpdate.m_totalTime > 5)
        {
            if(m_RealTimeUpdate.m_frequency < processData.lowerFrequencyLimit)
            bResult = false;
        }
        if(m_RealTimeUpdate.m_totalTime > processData.upperTimeLimit)
            bResult = false;
        break;
    case ProcessControl::FrequencyLimit:
        if(m_RealTimeUpdate.m_totalTime > 5)
        {
            if(m_RealTimeUpdate.m_frequency < processData.lowerFrequencyLimit)
            bResult = false;
        }
        break;
    case ProcessControl::TimeLimit:
        if(m_RealTimeUpdate.m_totalTime > processData.upperTimeLimit)
            bResult = false;
        break;
    default:
        break;
    }
    if(bResult == false)
    {
        _pOpcUaClientObj->machineWriteRun(false);
        return bResult;
    }

    switch(processData.processMode)
    {
    case ProcessControl::ContinueMode:
//        if(_pOpcUaClientObj->machineState() == false)
//            _pOpcUaClientObj->machineWriteRun(true);
        break;
    case ProcessControl::IntervalMode:
        if(_pOpcUaClientObj->machineState() == false) //ready signal
        {
            processData.durationTimer--;
            if(processData.durationTimer == 0)
            {
                _pOpcUaClientObj->machineWriteRun(false);
                processData.durationTimer = processData.durationTimerBakup;
                processData.isSonicsOn = false;
            }
        }
        else{
            processData.stopTimer--;
            if(processData.stopTimer == 0)
            {
                _pOpcUaClientObj->machineWriteRun(true);
                processData.stopTimer = processData.stopTimerBakup;
                processData.isSonicsOn = true;
            }
        }
        break;
    case ProcessControl::ProgramMode:
        if(processData.DelayStartTimer != 0)
            processData.DelayStartTimer--;
        else
        {
            if(_pOpcUaClientObj->machineState() == true) //Ready signal
                _pOpcUaClientObj->machineWriteRun(true);
        }
        if(processData.durationTimer == 0)
        {
            if(processData.programList.size() > 0)
            {
                qDebug()<<"amplitude:"<<processData.programList[0].first;
                tmpAmplitude = AMPLITUDE2HEX(processData.programList[0].first);
                _pOpcUaClientObj->machineWriteAmplitude(tmpAmplitude);
                processData.durationTimer = processData.programList[0].second;
                processData.programList.removeFirst();
            }
            else
            {
                _pOpcUaClientObj->machineWriteRun(false);
            }
        }
        else
        {
            if(_pOpcUaClientObj->machineState() == true) //Ready Signal
                _pOpcUaClientObj->machineWriteRun(true);
            processData.durationTimer--;
        }
        break;
    default:
        break;
    }
    return bResult;
}

void ProcessControl::SetNewUpdateAmplitude()
{
    OpcUaMachineBackend* _pOpcUaClientObj = OpcUaMachineBackend::Instance(this);
    qint16 tmpAmplitude = AMPLITUDE2HEX(static_cast<qint16>(processData.amplitudeSetting));
    _pOpcUaClientObj->machineWriteAmplitude(tmpAmplitude);
}

void ProcessControl::CalibrationAmplitude()
{
    m_AmplitudeDeviation = processData.amplitudeSetting/m_RealTimeUpdate.m_amplitude;
}
