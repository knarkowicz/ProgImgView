#include "stdafx.h"
#include "mainWindow.h"

CMainWindow* GMainWindow = nullptr;

int WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, char* lpCmdLine, int nShowCmd )
{
    QCoreApplication::setApplicationName( "ProgImgView" );
	QCoreApplication::setOrganizationName( "Kris" );
    QCoreApplication::setApplicationVersion( "1.07" );

    QApplication app( __argc, __argv );

    GMainWindow = new CMainWindow;
    GMainWindow->show();

	for ( unsigned i = 1; i < app.arguments().count(); ++i )
	{
		GMainWindow->OpenFile( app.arguments().at( i ) );
	}

    return app.exec();
}