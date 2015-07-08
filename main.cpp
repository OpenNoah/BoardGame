#include <qpe/qpeapplication.h>
#include <qtranslator.h>
#include <qtextcodec.h>
#include "mainw.h"

int main(int argc, char *argv[])
{
	QPEApplication a(argc, argv);
	/*QTranslator translator(0);
	qDebug(QPEApplication::qpeDir() + "i18n/" + QTextCodec::locale());
	if (!translator.load("BoardGame.qm", QPEApplication::qpeDir() + "i18n/" + QTextCodec::locale()))
		qDebug("Loading failed.");
	if (!translator.load("BoardGame.qm", "."))
		qDebug("Loading failed.");
	a.installTranslator(&translator);*/
	MainW w;
	a.setMainWidget(&w);
	w.showMaximized();
	return a.exec();
}
