#ifndef IMAGEBUTTON_H
#define IMAGEBUTTON_H

#include <QPushButton>

class ImageButton : public QPushButton
{
public:
    ImageButton(QWidget *parent = nullptr);

    void setImage(const QString &normalImage,
                  const QString &hoverImage,
                  const QString &pressedImage);

private:
    void updateStyle();

private:
    QString m_strNormalImage;
    QString m_strHoverImage;
    QString m_strPressedImage;
};

#endif // IMAGEBUTTON_H
