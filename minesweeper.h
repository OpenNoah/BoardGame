#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <qdialog.h>
#include <qpushbutton.h>
#include <qslider.h>
#include <qlabel.h>
#include <qobject.h>
#include "boardgame.h"

class Minesweeper : public BoardGame
{
	Q_OBJECT
public:
	Minesweeper(QObject *parent) : BoardGame(parent) {}
	~Minesweeper(void) {constructed = false;}
	virtual void reset(void);
	virtual void refresh(QPixmap *cache);

	// Game type information
	virtual QSize size(void) const {return QSize(info.w, info.h);}
	virtual int playerCount(void) const {return 1;}
	virtual QString gameName(void) const {return tr("Minesweeper");}
	virtual QString winMessage(const int player, const QString msg) const;
	virtual bool aiAvailable(void) const {return false;}
	virtual QSize displaySize(const QSize& disp) const;
	virtual int win(void);
	virtual bool mouseReleaseEvent(const ButtonState button, const QPoint& point, const int player);

protected:
	virtual bool place(const QPoint& point, const int player);
	virtual void paintPoint(QPainter *painter, const QRect& rect, const int id);
	virtual void drawHover(QPixmap *hover, QPainter *painter, const int player);

	// Game type information
	virtual BoardTypes boardType(void) const {return NoLine;}
	virtual QColor playerColor(int player) const {return idColor(player);}

private:
	int mineCount(const QPoint& point) const;
	int flagCount(const QPoint& point) const;
	QColor idColor(int index) const;

	enum IDs {Empty = 0, ID1, ID2, ID3, ID4, ID5, ID6, ID7, ID8, Mine, FlagBG, FlagFG, Uncovered, Background, TotalIDs, \
		  UncoveredPosition = 0x01000000, FlaggedPosition = 0x10000000};

	static bool constructed;
	struct {
		bool first, died;
		int w, h, total, remaining;
		QPixmap flag, mine;
	} info;
	static const QColor idColors[TotalIDs];
};

class MinesweeperSelector : public QDialog
{
	Q_OBJECT
	friend class Minesweeper;
public:
	MinesweeperSelector(QWidget *parent);

private slots:
	void easy(void);
	void medium(void);
	void expert(void);
	void sliderUpdate(int);

private:
	QLabel *lWidth, *lHeight, *lMines;
	QSlider *sWidth, *sHeight, *sMines;
	QPushButton *pbEasy, *pbMedium, *pbExpert, *pbAccept;
};

#endif
