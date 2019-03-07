#include "WindowHead.h"
#include "ui_WindowHead.h"

WindowHead::WindowHead(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowHead)
{
    ui->setupUi(this);

    ui->closeBtn->setImage(":/image/components/close_normal.ico",
                           ":/image/components/close_hover.ico",
                           ":/image/components/close_normal.ico");
    ui->minimizeBtn->setImage(":/image/components/close_normal.ico",
                           ":/image/components/close_hover.ico",
                           ":/image/components/close_normal.ico");
    connect(ui->closeBtn, SIGNAL(clicked()), this, SIGNAL(signalBtnClicked()));
    connect(ui->minimizeBtn, SIGNAL(clicked()), this, SIGNAL(signalMinimizeClicked()));
}

WindowHead::~WindowHead()
{
    delete ui;
}

void WindowHead::setImage(const QImage &image)
{
    ui->icon->setPixmap(QPixmap::fromImage(image));
}

void WindowHead::setTitle(const QString &title)
{
    ui->title->setText(title);
}
