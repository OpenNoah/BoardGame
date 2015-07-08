#ifndef BOARD_H
#define BOARD_H

#include <qwidget.h>
#include <qpixmap.h>
#include <qstatusbar.h>
#include <qmenubar.h>
#include <qlabel.h>
#include <qlist.h>
#include <qscrollview.h>
#include "boardgame.h"

class Board : public QWidget
{
	Q_OBJECT
	friend class BoardView;
public:
	Board(QWidget *parent, QStatusBar *status, QMenuBar *menu);

protected:
	void paintEvent(QPaintEvent *);
	void resizeEvent(QResizeEvent *);
	void timerEvent(QTimerEvent *);
	void mousePressEvent(QMouseEvent *e) {game->mousePressEvent(e->button(), game->findPoint(cache.size(), e->pos()), player);}
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

signals:
	void resizeRequest(void);

private slots:
	void menuEvent(int itemId);

private:
	void rebuild(void);
	void refresh(void);
	void newRole(void);
	bool winCheck(void);
	QString playerString(const int player);

	enum ItemIds {ID_Gomoku, ID_Gomoku3, ID_Connect4, ID_TicTacToe, ID_Reversi, ID_Minesweeper, ID_Game2048, ID_Quit, \
		      ID_AI, ID_Swap, ID_AutoSwap};
	enum PlayerTypes {AI = 0, Human = 1};

	int player, ai;
	QList<int> playerType;	// 0: AI, 1: Player
	QPoint hoverCoordinate;
	QPixmap cache, hoverPixmap;
	BoardGame *game;
	QLabel *lTime, *lPlayer, *lGame, *lPosition;
	QMenuBar *menu;
};

class BoardView : public QScrollView
{
	Q_OBJECT
public:
	BoardView(QWidget *parent, QStatusBar *status, QMenuBar *menu);

protected:
	void viewportResizeEvent(QResizeEvent *e);

private slots:
	void resizeAcknowledge(void);

private:
	Board *board;
};

#endif
