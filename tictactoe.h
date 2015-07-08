#ifndef TICTACTOE_H
#define TICTACTOE_H

#include "boardgame.h"

class TicTacToe : public BoardGame
{
	Q_OBJECT
public:
	TicTacToe(QObject *parent) : BoardGame(parent) {}
	//virtual void refresh(QPixmap *cache) {generalRefresh(cache);}
	virtual QPoint ai(const int player);

	// Game type information
	virtual QSize size(void) const {return QSize(3, 3);}
	virtual int playerCount(void) const {return 2;}
	virtual QString gameName(void) const {return tr("Tic Tac Toe");}
	virtual bool aiAvailable(void) const {return true;}

protected:
	virtual void paintPoint(QPainter *painter, const QRect& rect, const int player);

	// Game type information
	virtual int winLength(void) const {return 3;}
	virtual BoardTypes boardType(void) const {return EdgeLine;}
	virtual QColor playerColor(int player) const {return playerColors[player];}

private:
	enum Players {O = 0, X = 1};

	static const QColor playerColors[2];
};

#endif
