#include "JJY_PROJECT.h"

JJY_PROJECT::JJY_PROJECT(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.red_change_1, &QPushButton::clicked, this, [=]() {janggi->togglePosition(Red, 1); });
    connect(ui.red_change_2, &QPushButton::clicked, this, [=]() {janggi->togglePosition(Red, 2); });
    connect(ui.blue_change_1, &QPushButton::clicked, this, [=]() {janggi->togglePosition(Blue, 1); });
    connect(ui.blue_change_2, &QPushButton::clicked, this, [=]() {janggi->togglePosition(Blue, 2); });
    connect(ui.gamestart, &QPushButton::clicked, this, [=]() {gameStart(); });
    connect(ui.reset, &QPushButton::clicked, this, [=]() {gameReset(); });

    janggi = new JanggiWidget(this);
    ui.janggibase->layout()->addWidget(janggi);
}

JJY_PROJECT::~JJY_PROJECT()
{}

void JJY_PROJECT::gameStart()
{
    ui.red_change_1->setEnabled(false);
    ui.red_change_2->setEnabled(false);
    ui.blue_change_1->setEnabled(false);
    ui.blue_change_2->setEnabled(false);
    janggi->start();
}

void JJY_PROJECT::gameReset()
{
    ui.red_change_1->setEnabled(true);
    ui.red_change_2->setEnabled(true);
    ui.blue_change_1->setEnabled(true);
    ui.blue_change_2->setEnabled(true);
    janggi->reset();
}
