#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimerEvent>

#include "src/FrequencyDisplay.h"
#include "src/backend/opcuamachinebackend.h"
#include "src/backend/processcontrol.h"
#include "xlsxdocument.h"

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
    QXlsx::Document *m_pDocument;
    int             m_iDocumentCurrentRow;
    bool isPause;

public slots:
    void slotFrequencyUpdate(double tmpFrequency);
    void slotStart();
    void slotStop();
    void slotPause();
    void slotResonanceFrequencyPressed();
    void slotResonanceFrequencyReleased();
    void slotCloseClicked();
    void slotAlarmStateUpdate(bool value);

private:
    void updateProcessModeAndControlLimit();
public:
    void stopSaveRecord();
};

#endif // MAINWINDOW_H
