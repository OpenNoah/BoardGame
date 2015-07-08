#include <qpainter.h>
#include <qdatetime.h>
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qapplication.h>
#include <qmessagebox.h>
#include <cstdlib>
#include <ctime>
#include "board.h"
#include "gomoku.h"
#include "gomoku3.h"
#include "connect4.h"
#include "tictactoe.h"
#include "reversi.h"
#include "minesweeper.h"
#include "game2048.h"

Board::Board(QWidget *parent, QStatusBar *status, QMenuBar *menu) : QWidget(parent)
{
	int i = 0;
	ai = 0;
	srand(time(NULL));
	game = new BoardGame(this);
	hoverCoordinate.setX(-1);
	this->menu = menu;
	QPopupMenu *m;
	playerType.setAutoDelete(true);

	m = new QPopupMenu;
	menu->insertItem("&Game", m);
	m->insertItem("&Gomoku", ID_Gomoku, i++);
	m->insertItem("Gomoku &3P", ID_Gomoku3, i++);
	m->insertItem("&Connect Four", ID_Connect4, i++);
	m->insertItem("&Tic Tac Toe", ID_TicTacToe, i++);
	m->insertSeparator(i++);
	m->insertItem("&Reversi", ID_Reversi, i++);
	m->insertSeparator(i++);
	m->insertItem("&Minesweeper", ID_Minesweeper, i++);
	m->insertItem("&2048", ID_Game2048, i++);
	m->insertSeparator(i++);
	m->insertItem("&Quit", ID_Quit, i++);

	m = new QPopupMenu;
	menu->insertItem("&Options", m);
	m->insertItem("&AI", ID_AI, i++);
	m->insertItem("&Auto swap AI", ID_AutoSwap, i++);
	m->insertItem("&Swap AI", ID_Swap, i++);
	menu->setItemEnabled(ID_AI, game->aiAvailable());
	menu->setItemEnabled(ID_AutoSwap, game->aiAvailable());
	menu->setItemEnabled(ID_Swap, game->aiAvailable());
	menu->setItemChecked(ID_AI, game->aiAvailable());
	menu->setItemChecked(ID_AutoSwap, game->aiAvailable());

	status->addWidget(lPosition = new QLabel(status), 0, true);
	status->addWidget(lPlayer = new QLabel(status), 0, true);
	status->addWidget(lGame = new QLabel(status), 0, true);
	status->addWidget(lTime = new QLabel(status), 0, true);

	connect(menu, SIGNAL(activated(int)), this, SLOT(menuEvent(int)));
	//setMouseTracking(true);
	setBackgroundMode(FixedPixmap);
	startTimer(500);
	rebuild();
}

void Board::rebuild(void)
{
	game->reset();
	emit resizeRequest();
	player = -1;
	playerType.clear();
	if (menu->isItemChecked(ID_AI))
		for (int i = 0; i < game->playerCount(); i++)
			playerType.append(new int(ai == i ? AI : Human));
	else
		for (int i = 0; i < game->playerCount(); i++)
			playerType.append(new int(Human));
}

QString Board::playerString(const int player)
{
	QString type;
	if (player == -1)
		type = tr("None");
	else
		type = *playerType.at(player) == AI ? tr("AI") : tr("Human");
	return QString(tr("Player %1: %2")).arg(player + 1).arg(type);
}

void Board::refresh(void)
{
	lPlayer->setText(playerString(player));
	lGame->setText(game->gameName());
	game->refresh(&cache);
	setBackgroundPixmap(cache);
	repaint();
}

void Board::newRole(void)
{
next:
	if (++player == game->playerCount())
		player = 0;
	if (*playerType.at(player) == AI) {
		game->place(game->ai(player), player);
		if (++player == game->playerCount())
			player = 0;
		winCheck();
	}
	hoverCoordinate.setX(-1);
	if (!game->spaceAvailable(player)) {
		refresh();
		QMessageBox::information(topLevelWidget(), game->gameName(), tr("No available space for %1.").arg(playerString(player)));
		goto next;
	}
	game->hoverPixmap(&cache, &hoverPixmap, player);
	refresh();
}

void Board::resizeEvent(QResizeEvent *)
{
	cache.resize(size());
	refresh();
}

void Board::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	//painter.drawPixmap(0, 0, cache);	Displayed by FixedPixmap background mode
	if (hoverCoordinate.x() != -1)
		painter.drawPixmap(hoverCoordinate, hoverPixmap);
}

void Board::timerEvent(QTimerEvent *)
{
	QTime t(QTime::currentTime());
	lTime->setText(tr("%1:%2:%3").arg(t.hour(), 2).arg(t.minute(), 2).arg(t.second(), 2));
}

void Board::menuEvent(int itemId)
{
	switch (itemId) {
	case ID_Gomoku:
		delete game;
		game = new Gomoku(this);
		goto reset;
	case ID_Gomoku3:
		delete game;
		game = new Gomoku3(this);
		goto reset;
	case ID_Connect4:
		delete game;
		game = new Connect4(this);
		goto reset;
	case ID_TicTacToe:
		delete game;
		game = new TicTacToe(this);
		goto reset;
	case ID_Reversi:
		delete game;
		game = new Reversi(this);
		goto reset;
	case ID_Minesweeper:
		delete game;
		game = new Minesweeper(this);
		goto reset;
	/*case ID_Game2048:
		delete game;
		game = new Game2048(this);
		goto reset;*/
	case ID_AI:
		menu->setItemChecked(ID_AI, !menu->isItemChecked(ID_AI));
		if (game->playerCount() == -1)
			return;
		break;
	case ID_AutoSwap:
		menu->setItemChecked(ID_AutoSwap, !menu->isItemChecked(ID_AutoSwap));
		return;
	case ID_Swap:
		if (!menu->isItemChecked(ID_AI) || game->playerCount() == -1)
			return;
		if (++ai == game->playerCount())
			ai = 0;
		break;
	case ID_Quit:
		qApp->quit();
	default:
		return;
	}
ret:
	rebuild();
	newRole();
	return;
reset:
	menu->setItemEnabled(ID_AI, game->aiAvailable());
	menu->setItemEnabled(ID_AutoSwap, game->aiAvailable());
	menu->setItemEnabled(ID_Swap, game->aiAvailable());
	menu->setItemChecked(ID_AI, game->aiAvailable());
	menu->setItemChecked(ID_AutoSwap, game->aiAvailable());
	if (ai >= game->playerCount())
		ai = 0;
	goto ret;
}

bool Board::winCheck(void)
{
	int win = game->win();
	if (win == -1)
		return false;
	refresh();
	if (win < 0)
		QMessageBox::information(topLevelWidget(), game->gameName(), game->winMessage(win, tr("Draw.")));
	else
		QMessageBox::information(topLevelWidget(), game->gameName(), \
					 game->winMessage(win, *playerType.at(win) == AI ? tr("AI") : tr("Human")));
	if (menu->isItemChecked(ID_AI) && menu->isItemChecked(ID_AutoSwap) && ++ai == game->playerCount())
		ai = 0;
	rebuild();
	newRole();
	return true;
}

void Board::mouseReleaseEvent(QMouseEvent *e)
{
	hoverCoordinate.setX(-1);
	lPosition->setText(QString::null);
	repaint();
	if (game->playerCount() == -1)
		return;
	if (!game->mouseReleaseEvent(e->button(), game->findPoint(cache.size(), e->pos()), player))
		return;
	if (winCheck())
		return;
	newRole();
}

void Board::mouseMoveEvent(QMouseEvent *e)
{
	QPoint p = game->findPoint(cache.size(), e->pos());
	game->mouseMoveEvent(e->button(), p, player);
	if (p == QPoint(-1, -1)) {
		lPosition->setText(QString::null);
		hoverCoordinate.setX(-1);
	} else {
		lPosition->setText(QString(tr("x: %1, y: %2")).arg(p.x()).arg(p.y()));
		hoverCoordinate = game->hoverCoordinate(&cache, p);
	}
	repaint();
}

BoardView::BoardView(QWidget *parent, QStatusBar *status, QMenuBar *menu) : QScrollView(parent)
{
	addChild(board = new Board(this, status, menu));
	connect(board, SIGNAL(resizeRequest()), this, SLOT(resizeAcknowledge()));
}

void BoardView::resizeAcknowledge(void)
{
	QSize s = board->game->displaySize(viewport()->size());
	if (s.width() != -1) {
		board->resize(s);
		setHScrollBarMode(s.width() > viewport()->size().width() ? AlwaysOn : AlwaysOff);
		setVScrollBarMode(s.height() > viewport()->size().height() ? AlwaysOn : AlwaysOff);
	} else {
		board->resize(viewport()->size());
		setHScrollBarMode(AlwaysOff);
		setVScrollBarMode(AlwaysOff);
	}
}

void BoardView::viewportResizeEvent(QResizeEvent *e)
{
	resizeAcknowledge();
	QScrollView::viewportResizeEvent(e);
}
