#include "mainw.h"

MainW::MainW(void) : QMainWindow()
{
	board = new BoardView(this, statusBar(), menuBar());
	setCentralWidget(board);
	setCaption(tr("Board Games"));
}
