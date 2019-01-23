#include "ImageButton.h"

ImageButton::ImageButton(QWidget *parent)
 : QPushButton(parent)
{
    updateStyle();
}

void ImageButton::setImage(const QString &normalImage, const QString &hoverImage, const QString &pressedImage)
{
    m_strNormalImage = normalImage;
    m_strHoverImage = hoverImage;
    m_strPressedImage = pressedImage;
    updateStyle();
}

void ImageButton::updateStyle()
{
    this->setStyleSheet("QPushButton{background-color:transparent;border-width:0px;border-image:url(" + m_strNormalImage + ");}"
                        "QPushButton:hover{border-image:url(" + m_strHoverImage + ");}"
                        "QPushButton:pressed{border-image:url(" + m_strPressedImage + ");}");
}
