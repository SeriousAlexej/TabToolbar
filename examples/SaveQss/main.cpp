#include <QApplication>
#include "TabToolbar/StyleTools.h"
#include "TabToolbar/Styles.h"
#include <QFile>

using namespace tt;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

	QStringList styleNames = GetRegisteredStyles();
	std::unique_ptr<StyleParams> pStyleParams;
    QString appDirPath = QApplication::applicationDirPath();
	for (const QString & styleName : styleNames)
	{
		pStyleParams.reset(CreateStyle(styleName).release());
		QString styleSheet = GetSheetForStyle(*pStyleParams);

		QFile qssFile(appDirPath + "/" + styleName + ".qss");
		qssFile.open(QFile::WriteOnly);
		qssFile.write(styleSheet.toLocal8Bit());
		qssFile.close();
	}

//    int ret=a.exec();
    return 0;
}
