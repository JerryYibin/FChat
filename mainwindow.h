#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimerEvent>

#include "src/FrequencyDisplay.h"
#include "src/components/CustomXYSeries.h"
#include "src/components/CustomValueAxis.h"
#include "src/backend/opcuamachinebackend.h"
#include "src/backend/processcontrol.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void timerEvent(QTimerEvent *event);

private:
    int m_SecondCounter;
    Ui::MainWindow *ui;
    int m_iTime;
    FrequencyDisplay *m_pFrequencyDisplay;
    OpcUaMachineBackend* m_pOpcUaClient;
    ProcessControl* m_pProcessControl;
    bool isPause;
    CustomValueAxis *m_pTimeXAxis;
    CustomValueAxis *m_pAmpLeftYAxis;
    CustomValueAxis *m_pFreqRightYAxis;
    CustomXYSeries *m_pFreqXYSeries;
    CustomXYSeries *m_pAmpXYSeries;
    QList<QPointF> m_FreqPoints;
    QList<QPointF> m_AmpPoints;
    static QString constHistoryPath;
    static QString constHistoryFileName;

public slots:
    void slotFrequencyUpdate(double tmpFrequency);
    void slotInputAmplitudeChanged(double inputAmplitude);
    void slotStart();
    void slotStop();
    void slotPause();
    void slotResonanceFrequencyPressed();
    void slotResonanceFrequencyReleased();
    void slotCloseClicked();
    void slotMinimizeClicked();
    void slotAlarmStateUpdate(bool value);
    void slotAmplitudeCompensation();
    void slotAlarmReset();

private:
    void updateProcessModeAndControlLimit();
    void initializeGraphSetting();
    static void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
public:
    void stopSaveRecord();
};

#endif // MAINWINDOW_H
