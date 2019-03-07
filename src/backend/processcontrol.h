#ifndef PROCESSCONTROL_H
#define PROCESSCONTROL_H
#include <QObject>
#include <QList>
#include <QPair>
#define HEX2AMPLITUDE(x) (x * 100.00/0x6C00)
#define HEX2FREQUENCY(x) (x * 1000/0x6C00 + 19450)
#define AMPLITUDE2HEX(x) (x * 0x6C00/100)
class ProcessControl : public QObject
{
    Q_OBJECT
public:
    enum MODECONTROL
    {
        ContinueMode,
        IntervalMode,
        ProgramMode,
    };

    enum LIMITDCONTROL
    {
        Undefine = -1,
        FrequencyLimit,
        TimeLimit,
        BothLimit,
    };

    struct DATAUPDATE
    {
        double m_frequency;
        double m_amplitude;
        int m_totalTime;
    };

    struct DATA_PROCESS
    {
        MODECONTROL processMode;
        LIMITDCONTROL controlLimit;
        int durationTimer;
        int durationTimerBakup;
        int stopTimer;
        int stopTimerBakup;
        double amplitudeSetting;
        int DelayStartTimer;
        QList<QPair<int, int>> programList;
        int lowerFrequencyLimit;
        int upperTimeLimit;
        double param1;
        double param2;
        QString serialNum;
        QString RecordSavingPath;
        QString ProgramAmplitudePath;
    };
public:
    DATA_PROCESS processData;
    DATAUPDATE m_RealTimeUpdate;
    bool isScannFrequency;
private:
    double m_AmplitudeDeviation;
public:
    void LoadProgramedAmplitude2List();
    bool CheckStrategyProcess();
    void CalibrationAmplitude();
    void SetNewUpdateAmplitude();
signals:
    void signalResonanceFrequency(double value);
public slots:
    void slotFrequencyUpdate(qint16 tmpFrequency);
    void slotAmplitudeUpdate(qint16 tmpAmplitude);
public:
    static ProcessControl* Instance(QObject *parent = nullptr);
protected:
    ProcessControl(QObject *parent = nullptr);
private:
    static ProcessControl* _instance;
public:
    ~ProcessControl(){} //do-nothing destructor
};

#endif // PROCESSCONTROL_H
