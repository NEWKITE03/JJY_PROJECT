#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_JJY_PROJECT.h"

#include "JanggiWidget.h"

class JJY_PROJECT : public QMainWindow
{
    Q_OBJECT

public:
    JJY_PROJECT(QWidget *parent = nullptr);
    ~JJY_PROJECT();

private:
    void gameStart();
    void gameReset();

private:
    Ui::JJY_PROJECTClass ui;
    JanggiWidget *janggi;
};
