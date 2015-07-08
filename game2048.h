#ifndef GAME2048_H
#define GAME2048_H

#include "boardgame.h"

class Game2048 : public BoardGame
{
	Q_OBJECT
public:
	Game2048(QObject *parent) : BoardGame(parent) {}
	//~Game2048(void) {constructed = false;}
	//virtual void reset(void);
	virtual void refresh(QPixmap *cache);
	virtual void hoverPixmap(QPixmap *cache, QPixmap *hover, const int player) {hover->resize(-1, -1);}

	// Game type information
	virtual QSize size(void) const {return QSize(4, 4);}
	virtual int playerCount(void) const {return 1;}
	virtual QString gameName(void) const {return tr("2048");}
	virtual QString winMessage(const int player, const QString msg) const {return "Gameover.";}
	virtual bool aiAvailable(void) const {return false;}
	virtual int win(void) {return -1;}
	//virtual bool mouseReleaseEvent(const ButtonState button, const QPoint& point, const int player);

protected:
	virtual bool place(const QPoint& point, const int player) {return false;}
	//virtual void paintPoint(QPainter *painter, const QRect& rect, const int id);
	//virtual void drawHover(QPixmap *hover, QPainter *painter, const int player);

	// Game type information
	virtual BoardTypes boardType(void) const {return EdgeLine;}
	//virtual QColor playerColor(int player) const {return idColor(player);}

private:
	//int mineCount(const QPoint& point) const;
	//int flagCount(const QPoint& point) const;
	//QColor idColor(int index) const;

	enum Displays {Empty = 0, D2, D4, D8, D16, D32, D64, D128, D256, D512, D1024, D2048, D4096, D8192, D16384, D32768, D65536, D131072, Total};

	//static bool constructed;
	struct {
		//bool first, died;
		//int w, h, total, remaining;
		//QPixmap flag, mine;
		QPixmap display[Total];
	} info;
	//static const unsigned long tileData[13][3];
	//static const QColor idColors[TotalIDs];

	class Tile
	{
	public:
		enum DataType {Background = 0, Foreground = 1, FontSize = 2};

		static QColor foreground(const unsigned int value);
		static QColor background(const unsigned int value);
		static QFont font(const unsigned int value);

	private:
		static unsigned long data(const unsigned long index, const DataType type);
		static unsigned long toIndex(const unsigned long value);

		static const unsigned long d[13][3];
	};
};

#endif
