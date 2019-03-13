#ifndef FREQUENCYDISPLAY_H
#define FREQUENCYDISPLAY_H

#include <QWidget>
#include <QPaintEvent>

//#include "src/chart/CChartWidget.h"
#include "src/components/CustomChartView.h"

namespace Ui {
class FrequencyDisplay;
}

class FrequencyDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit FrequencyDisplay(QWidget *parent = nullptr);
    ~FrequencyDisplay();

    /*设置试件编号*/
    void setSerialNum(const QString serialNum);
    /*设置间歇振动参数:单次时长、间歇、振幅*/
    void setIntermittentvibrationInfo(const int onceTime,const int intervalTime,const double inputAmplitude);
    /*实际数据显示:实际振幅、实际频率、累计时长*/
    void setTheActualParameters(const double amplitude,const double frequency,const int totalTime);
    /*设置频率下限和工作时长*/
    void setOfflineAndWorkTime(const int offl,const int wtt);
    /*设置参数1和参数2*/
    void setTwoParameter(const double param1,const double param2);
    /*设置记录保存地址*/
    void setRecordSavePath(const QString savePath);
    /*设置导入振动谱路径*/
    void setPgSpectrumPath(const QString path);
    /*设置共振频率(扫频后)*/
    void setResonanceFrequency(double resonanceFrequency);
    /*设置界面可编辑*/
    void setEditValid(bool valid);
    /* Set only for Start button */
    void setStartBtnEditValid();
    /* Set only for Pause button */
    void setPauseBtnEditValue();
    /* Set only for Stop button */
    void setStopBtnEditValue();
    //选中连续振动
    void chooseCvn();
    //选中间歇性振动
    void chooseInv();
    //选中程序振动
    void choosePvn();
    //选中频率下线
    void chooseFol(bool checked);
    //选中工作时长
    void chooseWtm(bool checked);

    /*获取试件编号和记录保存地址*/
    void getSerialNumAndSavePath(QString &serialNum,QString &savePath);
    /*获取间歇振动参数:单次时长、间歇、振幅*/
    void getIntermittentvibrationInfo(int &onceTime,int &intervalTime,double &inputAmplitude);
    /*获取记录保存地址*/
    void getPgSpectrumPath(QString &vsPath);
    /*获取频率下限*/
    void getOffline(int &offline);
    /*获取工作时长*/
    void getWorkTotalTime(int &workTotalTime);
    /*获取参数1和参数2*/
    void getParameterInfo(double &paramOne,double &paramTwo);
    /*未知原因异常暂停接口(调用此接口后，只能点击窗口的结束按钮),
     * 用此接口时应当自己结束数据的刷新，并且调用resetBtState()重置按键状态*/
    void abnormalPause();
    /*重置开始暂停结束按键显示状态*/
    void resetBtState();

    void setStateColor(const QString &color);


    /*连续震动选中状态*/
    bool ContinuousVibrationState();
    /*间歇性震动*/
    bool IntermittentVibrationState();
    /*程序震动*/
    bool programVibrationState();
    /*频率下限*/
    bool FrequencyOlVibrationState();
    /*工作时长*/
    bool workTimeVibrationState();

    /*获取绘图界面*/
    CustomChartView* chartView();

signals:
    /*开始信号*/
    void startSig();
    /*停止信号*/
    void stopSig();
    /*暂停信号*/
    void pauseSig();
    /*扫频信号*/
    void updateFrequencySig();
    /*停止扫频*/
    void endUpdateFrequencySig();

    /*间歇性震动时单次时长改变*/
    void onceTimeChanged(int onceTime);
    /*间歇性震动时间歇改变*/
    void intervalChanged(int intervalTime);
    /*间歇性震动时输入振幅改变*/
    void inputAmplitudeChanged(double inputAmplitude);
    /*振幅补偿*/
    void amplitudeCompensation();
    /*Alarm Reset*/
    void alarmReset();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::FrequencyDisplay *ui;
    CustomChartView *m_pChartWidget;
};

#endif // FREQUENCYDISPLAY_H
