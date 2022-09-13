#pragma once

#include <QWidget>
#include <vector>
#include "Janggi.h"

class JanggiWidget  : public QWidget
{
	Q_OBJECT

public:
	JanggiWidget(QWidget *parent);
	~JanggiWidget();
    void paint(QPainter* painter);

    void start();
    void reset();
    void togglePosition(JanggiTeam team, int type);


protected:
    void paintEvent(QPaintEvent* event);

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);

private:
    JanggiPosition convertPosition(QPointF point);
    QRectF convertRect(JanggiPosition pos);
    QRectF janggiRect();
    void setSelection(JanggiPosition pos);
    void setFocus(JanggiPosition pos);

private:
    Janggi janggi;
    JanggiTeam turn;
    JanggiTeam winner;
    bool gamestart;
    bool gameend;
    int redtoggle;
    int bluetoggle;

    JanggiPosition selection = { -1,-1 };
    JanggiPosition focus = { -1,-1 };
    std::vector<JanggiPosition> moves;
    QImage peiceImage[2][7] = {};
    constexpr static double CellSize = 50;
    constexpr static double CellPadding = 5;
};
