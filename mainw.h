#ifndef MAINW_H
#define MAINW_H

#include <qmainwindow.h>
#include <qobject.h>
#include "board.h"

class MainW : public QMainWindow
{
	Q_OBJECT
public:
	MainW(void);

private:
	BoardView *board;
};

#endif
