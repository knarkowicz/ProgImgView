#include "stdafx.h"
#include "mainWindow.h"

CMainWindow* GMainWindow = nullptr;

int WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, char*, int nShowCmd )
{
    QCoreApplication::setApplicationName( "ProgImgView" );
	QCoreApplication::setOrganizationName( "Kris" );
    QCoreApplication::setApplicationVersion( "0.88" );

	int argc = 0;
    QApplication app( argc, 0 );

    GMainWindow = new CMainWindow;
    GMainWindow->show();

    return app.exec();
}