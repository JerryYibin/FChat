#include "opcuamachinebackend.h"
OpcUaMachineBackend* OpcUaMachineBackend::_instance = 0;

OpcUaMachineBackend* OpcUaMachineBackend::Instance(QObject *parent)
{
    if(_instance == 0){
        _instance = new OpcUaMachineBackend(parent);
    }
    return _instance;
}

OpcUaMachineBackend::OpcUaMachineBackend(QObject *parent)
    : QObject(parent),
    m_frequencyOnlyRead(0),
    m_amplitudeOnlyRead(0),
    m_weldRunningStatus(false),
    m_machineState(false),
    m_connected(false),
    m_message("Ready to connect"),
    m_successfullyCreated(false)
{
    QOpcUaProvider provider;
    setBackends(provider.availableBackends());
}

OpcUaMachineBackend::~OpcUaMachineBackend()
{
    if (m_client)
        m_client->disconnectFromEndpoint();
}

double OpcUaMachineBackend::frequencyReading() const
{
    return m_frequencyOnlyRead;
}

double OpcUaMachineBackend::amplitudeReading() const
{
    return m_amplitudeOnlyRead;
}

bool OpcUaMachineBackend::weldingStatus() const
{
    return m_weldRunningStatus;
}

bool OpcUaMachineBackend::machineState() const
{
    return m_machineState;
}

bool OpcUaMachineBackend::alarmState() const
{
    return m_alarmState;
}

bool OpcUaMachineBackend::connected() const
{
    return m_connected;
}

QString OpcUaMachineBackend::machineDesignation() const
{
    return m_machineDesignation;
}

QString OpcUaMachineBackend::message() const
{
    return m_message;
}

bool OpcUaMachineBackend::successfullyCreated() const
{
    return m_successfullyCreated;
}

void OpcUaMachineBackend::setMessage(const QString &message)
{
    if (message != m_message) {
        m_message = message;
        emit messageChanged(m_message);
    }
}

QStringList OpcUaMachineBackend::backends() const
{
    return m_backends;
}

void OpcUaMachineBackend::connectToEndpoint(const QString &url, qint32 index)
{
    if (m_connected)
        return;

    QOpcUaProvider provider;

    if (index < 0 || index >= m_backends.size())
        return; // Invalid index

    if (!m_client || (m_client && m_client->backend() != m_backends.at(index))) {
        m_client.reset(provider.createClient(m_backends.at(index)));
        if (m_client)
            QObject::connect(m_client.data(), &QOpcUaClient::stateChanged, this, &OpcUaMachineBackend::clientStateHandler);
    }

    if (!m_client) {
        qWarning() << "Could not create client";
        m_successfullyCreated = false;
        return;
    }

    m_successfullyCreated = true;
    m_client->connectToEndpoint(QUrl(url));
}

void OpcUaMachineBackend::disconnectFromEndpoint()
{
    if (m_connected)
        m_client->disconnectFromEndpoint();
}

void OpcUaMachineBackend::machineWriteAmplitude(qint16 value)
{
    if (m_amplitudeOnlyWriteNode)
    {
        m_amplitudeOnlyWriteNode->writeAttribute(QOpcUa::NodeAttribute::Value, value, QOpcUa::Types::UInt16);
    }
}

void OpcUaMachineBackend::machineWriteReset(bool value)
{
    if (m_resetFromPLConlyWriteNode)
        m_resetFromPLConlyWriteNode->writeAttribute(QOpcUa::NodeAttribute::Value, value);
}

void OpcUaMachineBackend::machineWriteRun(bool value)
{
    if (m_runFromPLConlyWriteNode)
        m_runFromPLConlyWriteNode->writeAttribute(QOpcUa::NodeAttribute::Value, value);
}

void OpcUaMachineBackend::machineWriteSeek(bool value)
{
    if (m_seekFromPLConlyWriteNode)
        m_seekFromPLConlyWriteNode->writeAttribute(QOpcUa::NodeAttribute::Value, value);
}

void OpcUaMachineBackend::startMachine()
{
    QString strNodeId = "ns=3;s=\042startFromPLConlyWrite\042";
    m_machineNode->callMethod(strNodeId);
}

void OpcUaMachineBackend::stopMachine()
{
    if (m_machineNode)
        m_machineNode->callMethod("ns=2;s=Machine.Stop");
}

void OpcUaMachineBackend::resetMachine()
{
    if (m_machineNode)
        m_machineNode->callMethod("ns=2;s=Machine.Reset");
}

/* slot */
void OpcUaMachineBackend::clientStateHandler(QOpcUaClient::ClientState state)
{
    m_connected = (state == QOpcUaClient::ClientState::Connected);

    emit connectedChanged(m_connected);

    if (state == QOpcUaClient::ClientState::Connected)
    {
        setMessage("Connected");
        // Create node objects for reading, writing and subscriptions
        m_machineNode.reset(m_client->node("ns=3;s=PLC"));

        QString strNodeId = "ns=3;s=\042alarmToPLConlyRead\042";
        m_machineAlarmNode.reset(m_client->node(strNodeId));

        strNodeId.clear();
        strNodeId= "ns=3;s=\042stateToPLConlyRead\042";
        m_machineStateNode.reset(m_client->node(strNodeId));

        strNodeId.clear();
        strNodeId = "ns=3;s=\042frequencyToPLConlyRead\042";
        m_frequencyOnlyReadNode.reset(m_client->node(strNodeId));

        strNodeId.clear();
        strNodeId = "ns=3;s=\042amplitudeToPLConlyRead\042";
        m_amplitudeOnlyReadNode.reset(m_client->node(strNodeId));

        strNodeId.clear();
        strNodeId = "ns=3;s=\042runFromPLConlyWrite\042";
        m_runFromPLConlyWriteNode.reset(m_client->node(strNodeId));

        strNodeId.clear();
        strNodeId = "ns=3;s=\042resetFromPLConlyWrite\042";
        m_resetFromPLConlyWriteNode.reset(m_client->node(strNodeId));
        strNodeId.clear();
        strNodeId = "ns=3;s=\042seekFromPLConlyWrite\042";
        m_seekFromPLConlyWriteNode.reset(m_client->node(strNodeId));
        strNodeId.clear();
        strNodeId = "ns=3;s=\042amplitudeFromPLConlyWrite\042";
        m_amplitudeOnlyWriteNode.reset(m_client->node(strNodeId));

        // Connect signal handlers for subscribed values
        QObject::connect(m_machineStateNode.data(), &QOpcUaNode::attributeUpdated, this, &OpcUaMachineBackend::machineStateUpdated);
        QObject::connect(m_frequencyOnlyReadNode.data(), &QOpcUaNode::attributeUpdated, this, &OpcUaMachineBackend::frequencyOnlyReadUpdated);
        QObject::connect(m_amplitudeOnlyReadNode.data(), &QOpcUaNode::attributeUpdated, this, &OpcUaMachineBackend::amplitudeOnlyReadUpdated);
        QObject::connect(m_machineAlarmNode.data(), &QOpcUaNode::attributeUpdated, this, &OpcUaMachineBackend::alarmOnlyReadUpdated);
        // Subscribe to data changes
        m_machineStateNode->enableMonitoring(QOpcUa::NodeAttribute::Value, QOpcUaMonitoringParameters(10));
        m_frequencyOnlyReadNode->enableMonitoring(QOpcUa::NodeAttribute::Value, QOpcUaMonitoringParameters(10));
        m_amplitudeOnlyReadNode->enableMonitoring(QOpcUa::NodeAttribute::Value, QOpcUaMonitoringParameters(10));
        m_machineAlarmNode->enableMonitoring(QOpcUa::NodeAttribute::Value, QOpcUaMonitoringParameters(10));
        // Connect the handler for async reading
//        QObject::connect(m_machineDesignationNode.data(), &QOpcUaNode::attributeRead, this, &OpcUaMachineBackend::machineDesignationRead);
        // Request the value attribute of the machine designation node
//        m_machineDesignationNode->readAttributes(QOpcUa::NodeAttribute::Value);

        // Add handlers for write and call results
        QObject::connect(m_amplitudeOnlyWriteNode.data(), &QOpcUaNode::attributeWritten, this, &OpcUaMachineBackend::setpointWritten);
        QObject::connect(m_runFromPLConlyWriteNode.data(), &QOpcUaNode::attributeWritten, this, &OpcUaMachineBackend::setpointWritten);
        QObject::connect(m_resetFromPLConlyWriteNode.data(), &QOpcUaNode::attributeWritten, this, &OpcUaMachineBackend::setpointWritten);
        QObject::connect(m_resetFromPLConlyWriteNode.data(), &QOpcUaNode::attributeWritten, this, &OpcUaMachineBackend::setpointWritten);
//        QObject::connect(m_machineNode.data(), &QOpcUaNode::methodCallFinished, this, &OpcUaMachineBackend::handleMethodResult);

        // Add handlers for enableMonitoring results
        QObject::connect(m_machineStateNode.data(), &QOpcUaNode::enableMonitoringFinished, this, &OpcUaMachineBackend::enableMonitoringFinished);
        QObject::connect(m_frequencyOnlyReadNode.data(), &QOpcUaNode::enableMonitoringFinished, this, &OpcUaMachineBackend::enableMonitoringFinished);
        QObject::connect(m_amplitudeOnlyReadNode.data(), &QOpcUaNode::enableMonitoringFinished, this, &OpcUaMachineBackend::enableMonitoringFinished);
        QObject::connect(m_machineAlarmNode.data(), &QOpcUaNode::enableMonitoringFinished, this, &OpcUaMachineBackend::enableMonitoringFinished);

    }

    if (state == QOpcUaClient::ClientState::Connecting)
        setMessage(QStringLiteral("Connecting"));

    if (state == QOpcUaClient::ClientState::Disconnected)
        setMessage(QString("Disconnected: %1").arg(QMetaEnum::fromType<QOpcUaClient::ClientError>().valueToKey(static_cast<int>(m_client->error()))));
}

void OpcUaMachineBackend::machineStateUpdated(QOpcUa::NodeAttribute attr, const QVariant &value)
{
    Q_UNUSED(attr);
//    bool newState = static_cast<MachineState>(value.toBool());
    bool newState = value.toBool();
    if (newState != m_machineState) {
        m_machineState = newState;
        emit machineStateChanged(m_machineState);
    }
}

void OpcUaMachineBackend::machineDesignationRead(QOpcUa::NodeAttributes attr)
{
    if (attr & QOpcUa::NodeAttribute::Value) { // Make sure the value attribute has been read
        if (m_machineDesignationNode->attributeError(QOpcUa::NodeAttribute::Value) == QOpcUa::UaStatusCode::Good) { // Make sure there was no error
            m_machineDesignation = m_machineDesignationNode->attribute(QOpcUa::NodeAttribute::Value).toString(); // Get the attribute from the cache
            emit machineDesignationChanged(m_machineDesignation);
        }
    }
}

void OpcUaMachineBackend::frequencyOnlyReadUpdated(QOpcUa::NodeAttribute attr, const QVariant &value)
{
    Q_UNUSED(attr);
    m_frequencyOnlyRead = value.toInt();
    emit frequencyOnlyReadChanged(m_frequencyOnlyRead);
}

void OpcUaMachineBackend::amplitudeOnlyReadUpdated(QOpcUa::NodeAttribute attr, const QVariant &value)
{
    Q_UNUSED(attr);
    m_amplitudeOnlyRead = value.toInt();
    emit amplitudeOnlyReadChanged(m_amplitudeOnlyRead);
}

void OpcUaMachineBackend::alarmOnlyReadUpdated(QOpcUa::NodeAttribute attr, const QVariant &value)
{
    Q_UNUSED(attr);
    m_alarmState = value.toBool();
    emit alarmOnlyReadChanged(m_alarmState);
}

void OpcUaMachineBackend::setpointWritten(QOpcUa::NodeAttribute attr, QOpcUa::UaStatusCode status)
{
    if (attr == QOpcUa::NodeAttribute::Value && status == QOpcUa::UaStatusCode::Good)
        setMessage("Setpoint successfully set");
    else if (attr == QOpcUa::NodeAttribute::Value && status != QOpcUa::UaStatusCode::Good)
        setMessage("Failed to set setpoint");
}

//void OpcUaMachineBackend::handleMethodResult(QString methodNodeId, const QVariant &result, QOpcUa::UaStatusCode statusCode)
//{
//    Q_UNUSED(result);

//    if (methodNodeId == "ns=3;s=\042startFromPLConlyWrite\042") {
//        if (statusCode == QOpcUa::UaStatusCode::Good)
//            setMessage("Pump successfully started");
//        else
//            setMessage("Unable to start pump");
//    } else if (methodNodeId == "ns=2;s=Machine.Stop") {
//        if (statusCode == QOpcUa::UaStatusCode::Good)
//            setMessage("Pump successfully stopped");
//        else
//            setMessage("Unable to stop pump");
//    } else if (methodNodeId == "ns=2;s=Machine.FlushTank2") {
//        if (statusCode == QOpcUa::UaStatusCode::Good)
//            setMessage("Flushing tank 2 successfully started");
//        else
//            setMessage("Unable to flush tank 2");
//    } else if (methodNodeId == "ns=2;s=Machine.Reset") {
//        if (statusCode == QOpcUa::UaStatusCode::Good)
//            setMessage("Simulation successfully reset");
//        else
//            setMessage("Unable to reset simulation");
//    }
//}

void OpcUaMachineBackend::enableMonitoringFinished(QOpcUa::NodeAttribute attr, QOpcUa::UaStatusCode status)
{
    Q_UNUSED(attr);
    if (!sender())
        return;
    if (status == QOpcUa::UaStatusCode::Good)
        qDebug() << "Monitoring successfully enabled for" << qobject_cast<QOpcUaNode *>(sender())->nodeId();
    else {
        qDebug() << "Failed to enable monitoring for" << qobject_cast<QOpcUaNode *>(sender())->nodeId() << ":" << status;
        setMessage("Failed to enable monitoring");
    }
}

/* private functions */
void OpcUaMachineBackend::setBackends(const QStringList &backends)
{
    if (m_backends != backends) {
        m_backends = backends;
        emit backendsChanged(m_backends);
    }
}
