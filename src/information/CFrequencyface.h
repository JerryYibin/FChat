#ifndef CFREQUENCYFACE_H
#define CFREQUENCYFACE_H

#include <QWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QDebug>
#include <QPaintEvent>

namespace Ui {
class CFrequencyFace;
}

class CFrequencyFace : public QWidget
{
    Q_OBJECT

public:
    explicit CFrequencyFace(QWidget *parent = nullptr);
    ~CFrequencyFace();

    /*set Serial number*/
    void setSerialNum(const QString serialNum);
    /*set Intermittent vibration info*/
    void setIntermittentvibrationInfo(const int onceTime,const int intervalTime,const double inputAmplitude);

    /*The actual parameters*/
    void setTheActualParameters(const double amplitude,const double frequency,const int totalTime);
    /*set the offline and work total time*/
    void setOfflineAndWorkTime(const int offl,const int wtt);
    /*set two parameter*/
    void setTwoParameter(const double param1,const double param2);
    /*set the save path*/
    void setRecordSavePath(const QString savePath);
    /*set the vibration spectrum path of program vibration*/
    void setPgSpectrumPath(const QString path);
    /*设置共振频率*/
    void setResonanceFrequency(double resonanceFrequency);
    /*设置界面是否可编辑*/
    void setEditvaild(bool valid);
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

    /*get serial number and save path*/
    void getSerialNumAndSavePath(QString &serialNum,QString &savePath);
    /*get Intermittent vibration info*/
    void getIntermittentvibrationInfo(int &onceTime,int &intervalTime,double &inputAmplitude);
    /*get vibration spectrum path of program vibration */
    void getPgSpectrumPath(QString &vsPath);
    /*get offline and work total time*/
    void getOffline(int &offline);
    void getWorkTotalTime(int &workTotalTime);
    /*get one two parameter*/
    void getParameterInfo(double &paramOne,double &paramTwo);

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


    /*未知原因异常暂停接口(调用此接口后，只能点击窗口的结束按钮),
     * 用此接口时应当自己结束数据的刷新，并且调用resetBtState()重置按键状态*/
    void abnormalPause();
    /*重置开始暂停结束按键显示状态*/
    void resetBtState();

    /*设置状态显示框的颜色*/
    void setStateColor(const QString &color);


protected:
    void paintEvent(QPaintEvent *event);

private:
    /*显示清零*/
    void clearShowData();
    /*设置按钮状态*/
    void setBtnEnabled(QPushButton *pBtn, bool enabled);

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

private slots:
    /*保存路径选择*/
    void slotSavePathChooese();
    /*震动谱路径选择*/
    void slotVsPathChooese();
    /*开始*/
    void slotStart();
    /*暂停*/
    void slotPause();
    /*结束*/
    void slotStop();
    /*参数1参数2编辑完成*/
    void slotParamEditFinished();

private slots:
    //开始扫频
    void on_updateFrequencyBt_pressed();
    //停止扫频
    void on_updateFrequencyBt_released();

    void on_lineEdit_onceTime_editingFinished();

    void on_lineEdit_interval_editingFinished();

    void on_spinBox_inputAmplitude_editingFinished();

private:
    Ui::CFrequencyFace *ui;
};

#endif // CFREQUENCYFACE_H
