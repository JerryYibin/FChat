#ifndef WINDOWHEAD_H
#define WINDOWHEAD_H

#include <QWidget>

namespace Ui {
class WindowHead;
}

class WindowHead : public QWidget
{
    Q_OBJECT

public:
    explicit WindowHead(QWidget *parent = nullptr);
    ~WindowHead();

    void setImage(const QImage &image);
    void setTitle(const QString &title);

signals:
    void signalBtnClicked();

private:
    Ui::WindowHead *ui;
};

#endif // WINDOWHEAD_H
