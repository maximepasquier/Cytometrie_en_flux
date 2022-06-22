#ifndef CUSTOMTOOLBUTTON_HPP
#define CUSTOMTOOLBUTTON_HPP

#include <QToolButton>
#include <QDebug>

class CustomToolButton : public QToolButton
{
    Q_OBJECT
public:
    explicit CustomToolButton(QWidget *parent = 0);
};

#endif // CUSTOMTOOLBUTTON_H