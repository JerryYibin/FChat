#ifndef FREQUENCYDISPLAY_H
#define FREQUENCYDISPLAY_H

#include <QWidget>
#include <QPaintEvent>

#include "src/chart/CChartWidget.h"

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
    //增加数据
    void addData(const QPointF &pointF);

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

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::FrequencyDisplay *ui;
    CChartWidget *m_pChartWidget;
};

#endif // FREQUENCYDISPLAY_H
