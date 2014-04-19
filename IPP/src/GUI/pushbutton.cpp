#include "include/GUI/pushbutton.h"

PushButton::PushButton(const QString& text, QWidget* parent) :
    QPushButton(text, parent)
{
    setUpStyle();
}

PushButton::PushButton(const QIcon& icon, const QString& text, QWidget* parent) :
    QPushButton(icon, text, parent)
{
    setUpStyle();
}

void PushButton::setUpStyle()
{
    setFlat(true);
    setStyleSheet("* {  \
background-color: #55FFFFFF; \
color: #000; \
font-size: 25px; \
padding: 8px 15px;\
}");
}
