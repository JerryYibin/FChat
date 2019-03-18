#include "WindowHead.h"
#include "ui_WindowHead.h"

WindowHead::WindowHead(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WindowHead)
{
    ui->setupUi(this);

    ui->closeBtn->setImage(":/image/components/blue-close-circle.png",
                           ":/image/components/blue-close-circle-fill.png",
                           ":/image/components/blue-close-circle.png");
    ui->minimizeBtn->setImage(":/image/components/blue-minus-circle.png",
                           ":/image/components/blue-minus-circle-fill.png",
                           ":/image/components/blue-minus-circle.png");
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
