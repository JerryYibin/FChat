#include "CFrequencyface.h"
#include "ui_CFrequencyface.h"

#include <QStyle>
#include <QStyleOption>
#include <QPainter>
#include <QDoubleValidator>
#include <QIntValidator>

CFrequencyFace::CFrequencyFace(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFrequencyFace)
{
    ui->setupUi(this);
    setTheActualParameters(15.8,19980,43620);

    connect(ui->lineEdit_paramOne, SIGNAL(editingFinished()), this, SLOT(slotParamEditFinished()));
    connect(ui->lineEdit_paramTwo, SIGNAL(editingFinished()), this, SLOT(slotParamEditFinished()));
    ui->lineEdit_offline->setValidator(new QDoubleValidator(ui->lineEdit_offline));
    ui->lineEdit_worktotaltime->setValidator(new QIntValidator(0, 9999999, ui->lineEdit_worktotaltime));
    ui->lineEdit_onceTime->setValidator(new QIntValidator(ui->lineEdit_onceTime));
    ui->lineEdit_interval->setValidator(new QIntValidator(0, 9999999, ui->lineEdit_interval));

    /*zhendong file chooese*/
    connect(ui->zhendongBt,&QPushButton::clicked,this,&CFrequencyFace::slotVsPathChooese);
    /*save path*/
    connect(ui->savePathBt,&QPushButton::clicked,this,&CFrequencyFace::slotSavePathChooese);
    /*start*/
    connect(ui->startBt,&QPushButton::clicked,this,&CFrequencyFace::slotStart);
    /*pause*/
    connect(ui->pauseBt,&QPushButton::clicked,this,&CFrequencyFace::slotPause);
    /*stop*/
    connect(ui->stopBt,&QPushButton::clicked,this,&CFrequencyFace::slotStop);
    /*amplitudeCompensation*/
    connect(ui->amplitudeCompensation, &QPushButton::clicked, this, &CFrequencyFace::amplitudeCompensation);
    /*alarm reset*/
    connect(ui->alarmReset, &QPushButton::clicked, this, &CFrequencyFace::alarmReset);
}

CFrequencyFace::~CFrequencyFace()
{
    delete ui;
}

void CFrequencyFace::setSerialNum(const QString serialNum)
{
    ui->lineEdit_serialNum->setText(serialNum);
}

void CFrequencyFace::setIntermittentvibrationInfo(const int onceTime, const int intervalTime, const double inputAmplitude)
{
    ui->lineEdit_onceTime->setText(QString::number(onceTime));
    ui->lineEdit_interval->setText(QString::number(intervalTime));
    ui->spinBox_inputAmplitude->setValue(inputAmplitude);
}

void CFrequencyFace::setTheActualParameters(const double amplitude,const double frequency, const int totalTime)
{
    ui->label_amplitude->setText(QString::number(amplitude));
//    ui->label_power->setText(QString::number(power));
    ui->label_frequency->setText(QString::number(frequency));
    ui->label_totalTime->setText(QString::number(totalTime));
}

void CFrequencyFace::setOfflineAndWorkTime(const int offl, const int wtt)
{
    ui->lineEdit_offline->setText(QString::number(offl));
    ui->lineEdit_worktotaltime->setText(QString::number(wtt));
}

void CFrequencyFace::setTwoParameter(const double param1, const double param2)
{
    ui->lineEdit_paramOne->setValue(param1);
    ui->lineEdit_paramTwo->setValue(param2);
}

void CFrequencyFace::setRecordSavePath(const QString savePath)
{
    ui->lineEdit_savePath->setText(savePath);
}

void CFrequencyFace::setPgSpectrumPath(const QString path)
{
    ui->lineEdit_zhenDong->setText(path);
}

void CFrequencyFace::setResonanceFrequency(double resonanceFrequency)
{
    ui->lineEdit_resonanceFrequency->setText(QString::number(resonanceFrequency));
}

void CFrequencyFace::setEditvaild(bool valid)
{
    ui->lineEdit_serialNum->setReadOnly(!valid);
    ui->lineEdit_savePath->setReadOnly(!valid);
    this->setBtnEnabled(ui->savePathBt, valid);
    this->setBtnEnabled(ui->amplitudeCompensation, valid);
    this->setBtnEnabled(ui->alarmReset, valid);
    ui->cvnButton->setEnabled(valid);
    ui->pvnButton->setEnabled(valid);

    ui->ivnButton->setEnabled(valid);
    ui->changeProperty->setEnabled(valid);
    if((ui->ivnButton->isChecked() || ui->cvnButton->isChecked()) && valid == false)
    {
        if(!ui->changeProperty->isChecked())
        {
            ui->lineEdit_onceTime->setReadOnly(!valid);
            ui->lineEdit_interval->setReadOnly(!valid);
            ui->spinBox_inputAmplitude->setReadOnly(!valid);
        }
    }
    else {
        ui->lineEdit_onceTime->setReadOnly(!valid);
        ui->lineEdit_interval->setReadOnly(!valid);
        ui->spinBox_inputAmplitude->setReadOnly(!valid);
    }

    ui->lineEdit_zhenDong->setReadOnly(!valid);
    this->setBtnEnabled(ui->zhendongBt, valid);
    ui->folBox->setEnabled(valid);
    ui->lineEdit_offline->setReadOnly(!valid);
    ui->wtmBox_2->setEnabled(valid);
    ui->lineEdit_worktotaltime->setReadOnly(!valid);
    ui->lineEdit_paramOne->setReadOnly(!valid);
    ui->lineEdit_paramTwo->setReadOnly(!valid);
    this->setBtnEnabled(ui->startBt, valid);
    this->setBtnEnabled(ui->updateFrequencyBt, valid);
}

void CFrequencyFace::chooseCvn()
{
    ui->cvnButton->setChecked(true);
}

void CFrequencyFace::chooseInv()
{
    ui->ivnButton->setChecked(true);
}

void CFrequencyFace::choosePvn()
{
    ui->pvnButton->setChecked(true);
}

void CFrequencyFace::chooseFol(bool checked)
{
    ui->folBox->setChecked(checked);
}

void CFrequencyFace::chooseWtm(bool checked)
{
    ui->wtmBox_2->setChecked(checked);
}

void CFrequencyFace::getSerialNumAndSavePath(QString &serialNum, QString &savePath)
{
    serialNum = ui->lineEdit_serialNum->text();
    savePath = ui->lineEdit_savePath->text();
}

void CFrequencyFace::getIntermittentvibrationInfo(int &onceTime, int &intervalTime, double &inputAmplitude)
{
    onceTime = ui->lineEdit_onceTime->text().toInt();
    intervalTime = ui->lineEdit_interval->text().toInt();
    inputAmplitude = ui->spinBox_inputAmplitude->value();

}

void CFrequencyFace::getPgSpectrumPath(QString &vsPath)
{
    vsPath = ui->lineEdit_zhenDong->text();
}

void CFrequencyFace::getOffline(int &offline)
{
    offline = ui->lineEdit_offline->text().toInt();
}

void CFrequencyFace::getWorkTotalTime(int &workTotalTime)
{
    workTotalTime = ui->lineEdit_worktotaltime->text().toInt();
}

void CFrequencyFace::getParameterInfo(double &paramOne, double &paramTwo)
{
    paramOne = ui->lineEdit_paramOne->value();
    paramTwo = ui->lineEdit_paramTwo->value();
}

bool CFrequencyFace::ContinuousVibrationState()
{
    return ui->cvnButton->isChecked();
}

bool CFrequencyFace::IntermittentVibrationState()
{
    return ui->ivnButton->isChecked();
}

bool CFrequencyFace::programVibrationState()
{
    return ui->pvnButton->isChecked();
}

bool CFrequencyFace::FrequencyOlVibrationState()
{
    return ui->folBox->isChecked();
}

bool CFrequencyFace::workTimeVibrationState()
{
    return ui->wtmBox_2->isChecked();
}

void CFrequencyFace::abnormalPause()
{
    ui->startBt->setDisabled(false);
    ui->startBt->setStyleSheet("background-color: rgb(212, 212, 212);");

    ui->pauseBt->setDisabled(false);
    ui->startBt->setStyleSheet("background-color: rgb(212, 212, 212);");
}

void CFrequencyFace::resetBtState()
{
    ui->startBt->setDisabled(true);
    ui->startBt->setStyleSheet("background-color: rgb(0, 121, 193);");

    ui->pauseBt->setDisabled(true);
    ui->startBt->setStyleSheet("background-color: rgb(0, 121, 193);");

    ui->stopBt->setDisabled(true);
}

void CFrequencyFace::setStateColor(const QString &color)
{
    ui->stateWidget->setStyleSheet(QString("QWidget{border-radius: %1px; background-color: %2;}").arg(ui->stateWidget->width()/2).arg(color));
}

void CFrequencyFace::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
    QStyleOption opt;
    opt.init(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

void CFrequencyFace::clearShowData()
{
    ui->label_amplitude->setText(QString(""));
//    ui->label_power->setText(QString(""));
    ui->label_frequency->setText(QString(""));
    ui->label_totalTime->setText(QString(""));
//    ui->lineEdit_paramOne->setText(QString(""));
//    ui->lineEdit_paramTwo->setText(QString(""));
//    ui->lineEdit_serialNum->setText(QString(""));

}

void CFrequencyFace::setBtnEnabled(QPushButton *pBtn, bool enabled)
{
    pBtn->setEnabled(enabled);
    if(enabled)
    {
        pBtn->setStyleSheet("background-color: rgb(0, 121, 193);");
    }
    else
    {
        pBtn->setStyleSheet("background-color: rgb(212, 212, 212);");
    }
}

void CFrequencyFace::slotSavePathChooese()
{
    QString cFilePath = QFileDialog::getExistingDirectory(this,
                                                          tr("选择保存路径"),
                                                          ".",
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    qDebug()<<cFilePath<<endl;
    ui->lineEdit_savePath->setText(cFilePath);
}

void CFrequencyFace::slotVsPathChooese()
{
    QString cFilePath = QFileDialog::getOpenFileName(nullptr,tr("选择震动谱路径"),".","");
    qDebug()<<cFilePath<<endl;
    ui->lineEdit_zhenDong->setText(cFilePath);
}

void CFrequencyFace::slotStart()
{
    slotParamEditFinished();
    emit startSig();
}

void CFrequencyFace::slotPause()
{
    emit pauseSig();
}

void CFrequencyFace::slotStop()
{
    /*清零数据*/
    clearShowData();
    emit stopSig();
}

void CFrequencyFace::slotParamEditFinished()
{
    if(ui->lineEdit_paramOne->value() == 0.0)
    {
        ui->lineEdit_paramOne->setValue(1);
    }
    if(ui->lineEdit_paramTwo->value() == 0.0)
    {
        ui->lineEdit_paramTwo->setValue(1);
    }
}

void CFrequencyFace::on_updateFrequencyBt_pressed()
{
    emit updateFrequencySig();
}

void CFrequencyFace::on_updateFrequencyBt_released()
{
    emit endUpdateFrequencySig();
}

void CFrequencyFace::on_lineEdit_onceTime_editingFinished()
{
    if(!ui->changeProperty->isEnabled() && ui->changeProperty->isChecked())
    {
        emit onceTimeChanged(ui->lineEdit_onceTime->text().toInt());
    }
}

void CFrequencyFace::on_lineEdit_interval_editingFinished()
{
    if(!ui->changeProperty->isEnabled() && ui->changeProperty->isChecked())
    {
        emit intervalChanged(ui->lineEdit_interval->text().toInt());
    }
}

void CFrequencyFace::on_spinBox_inputAmplitude_editingFinished()
{
    if(!ui->changeProperty->isEnabled() && ui->changeProperty->isChecked())
    {
        emit inputAmplitudeChanged(ui->spinBox_inputAmplitude->value());
    }
}
