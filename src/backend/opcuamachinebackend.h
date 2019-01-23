#ifndef OPCUAMACHINEBACKEND_H
#define OPCUAMACHINEBACKEND_H
#include <QtOpcUa>

class OpcUaMachineBackend : public QObject
{
    Q_OBJECT
public:
    double frequencyReading() const;
    double amplitudeReading() const;
    bool weldingStatus() const;
    bool machineState() const;
    bool alarmState() const;
    bool connected() const;
    QString machineDesignation() const;
    QString message() const;
    bool successfullyCreated() const;
    void setMessage(const QString &message);
    QStringList backends() const;

    void connectToEndpoint(const QString &url, qint32 index);
    void disconnectFromEndpoint();
    void machineWriteAmplitude(qint16 value);
    void machineWriteRun(bool value);
    void machineWriteReset(bool value);
    void machineWriteSeek(bool value);
    void startMachine();
    void stopMachine();
    void resetMachine();

signals:
    void connectedChanged(bool connected);
    void messageChanged(QString message);
    void backendsChanged(QStringList backends);
    void machineStateChanged(bool state);
    void machineDesignationChanged(QString designation);
    void frequencyOnlyReadChanged(qint16 value);
    void amplitudeOnlyReadChanged(qint16 value);
    void alarmOnlyReadChanged(bool value);
public slots:
    void clientStateHandler(QOpcUaClient::ClientState state);
    void machineStateUpdated(QOpcUa::NodeAttribute attr, const QVariant &value);
    void machineDesignationRead(QOpcUa::NodeAttributes attr);
    void frequencyOnlyReadUpdated(QOpcUa::NodeAttribute attr, const QVariant &value);
    void amplitudeOnlyReadUpdated(QOpcUa::NodeAttribute attr, const QVariant &value);
    void alarmOnlyReadUpdated(QOpcUa::NodeAttribute attr, const QVariant &value);
    void setpointWritten(QOpcUa::NodeAttribute attr, QOpcUa::UaStatusCode status);
//    void handleMethodResult(QString methodNodeId, const QVariant &result, QOpcUa::UaStatusCode statusCode);
    void enableMonitoringFinished(QOpcUa::NodeAttribute attr, QOpcUa::UaStatusCode status);
private:
    void setBackends(const QStringList &backends);

    QScopedPointer<QOpcUaClient> m_client;
    QScopedPointer<QOpcUaNode> m_machineStateNode;
    QScopedPointer<QOpcUaNode> m_machineAlarmNode;
    QScopedPointer<QOpcUaNode> m_machineNode;
    QScopedPointer<QOpcUaNode> m_machineDesignationNode;
    QScopedPointer<QOpcUaNode> m_frequencyOnlyReadNode;
    QScopedPointer<QOpcUaNode> m_amplitudeOnlyReadNode;
    QScopedPointer<QOpcUaNode> m_amplitudeOnlyWriteNode;
    QScopedPointer<QOpcUaNode> m_runFromPLConlyWriteNode;
    QScopedPointer<QOpcUaNode> m_resetFromPLConlyWriteNode;
    QScopedPointer<QOpcUaNode> m_seekFromPLConlyWriteNode;

    qint16 m_frequencyOnlyRead;
    qint16 m_amplitudeOnlyRead;
    qint16 m_amplitudeOnlyWrite;
    bool m_weldStart;
    bool m_weldStop;
    bool m_weldRunningStatus;
    bool m_machineState;
    bool m_alarmState;
    QString m_machineDesignation;

    bool m_connected;
    QString m_message;

    bool m_successfullyCreated;
    QStringList m_backends;
public:
    static OpcUaMachineBackend* Instance(QObject *parent = nullptr);
protected:
    OpcUaMachineBackend(QObject *parent = nullptr);
private:
    static OpcUaMachineBackend* _instance;
public:
    ~OpcUaMachineBackend(); //do-nothing destructor
};

#endif // OPCUAMACHINEBACKEND_H
