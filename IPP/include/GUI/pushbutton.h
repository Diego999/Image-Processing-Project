#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(const QString& text, QWidget* parent = 0);
    explicit PushButton(const QIcon& icon, const QString& text, QWidget* parent = 0);

signals:

public slots:

private:
    void setUpStyle();

};

#endif // PUSHBUTTON_H
