#include "stdafx.h"
#include "mainWindow.h"
#include "imageWindow.h"

CMainWindow::CMainWindow()
{
    m_mdiArea.setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    m_mdiArea.setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    setCentralWidget( &m_mdiArea );

    CreateToolbar();
    ReadSettings();

    setWindowTitle( "ProgImgView 0.85" );
	setAcceptDrops( true );
	SetStatusRight( "Hold right mouse button to pick texel" );

	connect( &m_mdiArea, &QMdiArea::subWindowActivated, this, &CMainWindow::SubWindowActivated );
}

void CMainWindow::closeEvent( QCloseEvent* event )
{
    m_mdiArea.closeAllSubWindows();
    if ( m_mdiArea.currentSubWindow() ) 
	{
        event->ignore();
    } 
	else 
	{
        WriteSettings();
        event->accept();
    }
}

void CMainWindow::dragEnterEvent( QDragEnterEvent* event )
{
    if ( event->mimeData()->hasUrls() )
	{
        event->acceptProposedAction();
    }
}

void CMainWindow::dropEvent( QDropEvent* event )
{
    QList<QUrl> droppedUrls = event->mimeData()->urls();
    for( unsigned i = 0; i < droppedUrls.size(); i++) 
	{
		OpenFile( droppedUrls[ i ].toLocalFile() );
    }
 
    event->acceptProposedAction();
}

void CMainWindow::Open()
{
    QString const fileName = QFileDialog::getOpenFileName( this, "Open image", m_lastDirectory, "Images (*.dds;*.tga;*.jpg;*.bmp;*.png)" );
    if ( !fileName.isEmpty() )
	{
		m_lastDirectory = fileName;
        OpenFile( fileName );
	}
}

void CMainWindow::ReloadCurrent()
{
	CImageWindow* imageWindow = ActiveImageWindow();
	if ( imageWindow )
	{
		imageWindow->Reload();
	}
}

bool CMainWindow::OpenFile( QString const& fileName )
{
    CImageWindow* imageWindow = new CImageWindow();
	m_mdiArea.addSubWindow( imageWindow );

	bool const ret = imageWindow->LoadFile( fileName );
    if ( ret )	
	{
		UpdateToolbar( imageWindow );
		
		//QStyleOptionTitleBar so;
		//so.titleBarState = 1;
		//so.titleBarFlags = Qt::Window;
		//unsigned titleBarWidth	= imageWindow->parentWidget()->style()->pixelMetric( QStyle::PM_MdiSubWindowFrameWidth, &so, imageWindow->parentWidget() );
		//unsigned titleBarHeight = imageWindow->parentWidget()->style()->pixelMetric( QStyle::PM_TitleBarHeight, &so, imageWindow->parentWidget() );
		imageWindow->parentWidget()->resize( qMax( imageWindow->GetImageWidth() + 30, 128u ), qMax( imageWindow->GetImageHeight() + 30, 128u ) );
		imageWindow->parentWidget()->updateGeometry();
		imageWindow->show();
	}
	else
	{
		imageWindow->close();
	}

    AddToRecentFiles( fileName );
	return ret;
}

void CMainWindow::OpenRecentFile()
{
    if ( QAction const* action = qobject_cast<QAction const*>( sender() ) )
	{
        OpenFile( action->text() );
	}
}

void CMainWindow::ChangeChannel( int channel )
{
	CImageWindow* imageWindow = ActiveImageWindow();
	if ( imageWindow )
	{
		imageWindow->SetViewChannel( (EViewChannel) channel );
	}
}

void CMainWindow::ChangeFace( int face )
{
	CImageWindow* imageWindow = ActiveImageWindow();
	if ( imageWindow )
	{
		imageWindow->SetViewFace( face );
	}
}

void CMainWindow::ChangeMip( int mip )
{
	CImageWindow* imageWindow = ActiveImageWindow();
	if ( imageWindow )
	{
		imageWindow->SetViewMipMap( mip );
	}
}

void CMainWindow::ChangeVisMin( float visMin )
{
	CImageWindow* imageWindow = ActiveImageWindow();
	if ( imageWindow )
	{
		imageWindow->SetViewMin( visMin );
	}
}

void CMainWindow::ChangeVisMax( float visMax )
{
	CImageWindow* imageWindow = ActiveImageWindow();
	if ( imageWindow )
	{
		imageWindow->SetViewMax( visMax );
	}
}

void CMainWindow::ChangeVisGamma( float visGamma )
{
	CImageWindow* imageWindow = ActiveImageWindow();
	if ( imageWindow )
	{
		imageWindow->SetViewGamma( visGamma );
	}
}

void CMainWindow::SubWindowActivated( QMdiSubWindow* window )
{
	UpdateToolbar( ActiveImageWindow() );
}

void CMainWindow::CreateToolbar()
{
	statusBar()->addPermanentWidget( &m_statusLeft, 1 );
	statusBar()->addPermanentWidget( &m_statusRight, 1 );

	QToolBar* toolBar = addToolBar( tr( "Main" ) );

	m_channelComboBox.addItem( "View RGB" );
	m_channelComboBox.addItem( "View R" );
	m_channelComboBox.addItem( "View G" );
	m_channelComboBox.addItem( "View B" );
	m_channelComboBox.addItem( "View A" );
	toolBar->addWidget( &m_channelComboBox );
	connect( &m_channelComboBox, static_cast< void (QComboBox::*)( int ) >( &QComboBox::currentIndexChanged ), this, &CMainWindow::ChangeChannel );

	toolBar->addWidget( &m_faceComboBox );
	connect( &m_faceComboBox, static_cast< void (QComboBox::*)( int ) >( &QComboBox::currentIndexChanged ), this, &CMainWindow::ChangeFace );

	toolBar->addWidget( &m_mipComboBox );
	connect( &m_mipComboBox, static_cast< void (QComboBox::*)( int ) >( &QComboBox::currentIndexChanged ), this, &CMainWindow::ChangeMip );

	m_visMinSpinBox.setSingleStep( 0.1 );
	m_visMinSpinBox.setPrefix( "min: " );
	m_visMinSpinBox.setRange( -FLT_MAX, FLT_MAX );
	toolBar->addWidget( &m_visMinSpinBox );
	connect( &m_visMinSpinBox,  static_cast< void (QDoubleSpinBox::*)( double ) >( &QDoubleSpinBox::valueChanged ), this, &CMainWindow::ChangeVisMin );

	m_visMaxSpinBox.setSingleStep( 0.1 );
	m_visMaxSpinBox.setPrefix( "max: " );
	m_visMaxSpinBox.setRange( -FLT_MAX, FLT_MAX );
	toolBar->addWidget( &m_visMaxSpinBox );
	connect( &m_visMaxSpinBox,  static_cast< void (QDoubleSpinBox::*)( double ) >( &QDoubleSpinBox::valueChanged ), this, &CMainWindow::ChangeVisMax );

	m_visGammaSpinBox.setSingleStep( 0.1 );
	m_visGammaSpinBox.setRange( 0.01, 100.0 );
	m_visGammaSpinBox.setPrefix( "gamma: " );
	toolBar->addWidget( &m_visGammaSpinBox );
	connect( &m_visGammaSpinBox,  static_cast< void (QDoubleSpinBox::*)( double ) >( &QDoubleSpinBox::valueChanged ), this, &CMainWindow::ChangeVisGamma );

    QMenu* fileMenu = menuBar()->addMenu( "File" );

    m_actionOpen = new QAction( "Open...", this );
    m_actionOpen->setShortcuts( QKeySequence::Open );
    connect( m_actionOpen, &QAction::triggered, this, &CMainWindow::Open );
    fileMenu->addAction( m_actionOpen );

    m_actionReloadCurrent = new QAction( "Reload current", this );
    m_actionReloadCurrent->setShortcut( QKeySequence( "R" ) );
    connect( m_actionReloadCurrent, &QAction::triggered, this, &CMainWindow::ReloadCurrent );
    fileMenu->addAction( m_actionReloadCurrent );

    fileMenu->addSeparator();

    QMenu* recentMenu = fileMenu->addMenu( "Recent..." );
    m_actionRecentFileSubMenuAct = recentMenu->menuAction();

    for ( int i = 0; i < ARRAYSIZE( m_actionRecentFile ); ++i ) 
	{
        m_actionRecentFile[ i ] = recentMenu->addAction( QString(), this, &CMainWindow::OpenRecentFile );
        m_actionRecentFile[ i ]->setVisible( false );
    }

    m_actionRecentFileSeparator = fileMenu->addSeparator();
    fileMenu->addSeparator();

    QAction *exitAct = fileMenu->addAction( "Exit", qApp, &QApplication::closeAllWindows);
    exitAct->setShortcuts( QKeySequence::Quit );
    fileMenu->addAction( exitAct );

    m_windowMenu = menuBar()->addMenu( "Window" );

    m_actionClose = new QAction( "Close", this);
    connect( m_actionClose, &QAction::triggered, &m_mdiArea, &QMdiArea::closeActiveSubWindow );

    m_actionCloseAll = new QAction( "Close All", this );
    connect( m_actionCloseAll, &QAction::triggered, &m_mdiArea, &QMdiArea::closeAllSubWindows );

    m_actionTile = new QAction( "Tile", this);
    connect( m_actionTile, &QAction::triggered, &m_mdiArea, &QMdiArea::tileSubWindows );

    m_actionCascade = new QAction( "Cascade", this);
    connect( m_actionCascade, &QAction::triggered, &m_mdiArea, &QMdiArea::cascadeSubWindows );

    m_actionNext = new QAction( "Next", this);
    m_actionNext->setShortcuts( QKeySequence::NextChild );
    connect( m_actionNext, &QAction::triggered, &m_mdiArea, &QMdiArea::activateNextSubWindow );

    m_actionPrevious = new QAction( "Previous", this );
    m_actionPrevious->setShortcuts( QKeySequence::PreviousChild );
    connect( m_actionPrevious, &QAction::triggered, &m_mdiArea, &QMdiArea::activatePreviousSubWindow );

    m_actionWindowMenuSeparator = new QAction( this );
    m_actionWindowMenuSeparator->setSeparator( true );

    m_windowMenu->addAction( m_actionClose );
    m_windowMenu->addAction( m_actionCloseAll );
    m_windowMenu->addSeparator();
    m_windowMenu->addAction( m_actionTile );
    m_windowMenu->addAction( m_actionCascade );
    m_windowMenu->addSeparator();
    m_windowMenu->addAction( m_actionNext );
    m_windowMenu->addAction( m_actionPrevious );
    m_windowMenu->addAction( m_actionWindowMenuSeparator );

	UpdateToolbar( nullptr );
}

void CMainWindow::ReadSettings()
{
    QSettings settings( QCoreApplication::organizationName(), QCoreApplication::applicationName() );
    QByteArray geometry = settings.value( "geometry", QByteArray() ).toByteArray();
	m_lastDirectory		= settings.value( "last_directory" ).toString();

    if ( geometry.isEmpty() ) 
	{
        QRect availableGeometry = QApplication::desktop()->availableGeometry( this );
        resize( availableGeometry.width() / 3, availableGeometry.height() / 2 );
        move( ( availableGeometry.width() - width() ) / 2, ( availableGeometry.height() - height() ) / 2 );
    } 
	else 
	{
        restoreGeometry( geometry );
    }

	unsigned const recentFileNum = settings.beginReadArray( "recent_files" );
    for ( unsigned i = 0; i < recentFileNum && i < ARRAYSIZE( m_actionRecentFile ); ++i ) 
	{
        settings.setArrayIndex( i );
		QString recentFile = settings.value( "file" ).toString();
        m_actionRecentFile[ i ]->setText( recentFile );
        m_actionRecentFile[ i ]->setVisible( true );
    }
}

void CMainWindow::AddToRecentFiles( QString const& fileName )
{
	if ( m_actionRecentFile[ 0 ]->text() != fileName )
	{
		for ( unsigned i = ARRAYSIZE( m_actionRecentFile ) - 1; i > 0; --i )
		{
			m_actionRecentFile[ i ]->setText( m_actionRecentFile[ i - 1 ]->text() );
			m_actionRecentFile[ i ]->setVisible( m_actionRecentFile[ i - 1 ]->isVisible() );
		}
		m_actionRecentFile[ 0 ]->setText( fileName );
		m_actionRecentFile[ 0 ]->setVisible( true );
	}
}

void CMainWindow::WriteSettings()
{
    QSettings settings( QCoreApplication::organizationName(), QCoreApplication::applicationName() );
    settings.setValue( "geometry", saveGeometry() );
	settings.setValue( "last_directory", m_lastDirectory );

	settings.beginWriteArray( "recent_files" );
    for ( unsigned i = 0; i < ARRAYSIZE( m_actionRecentFile ); ++i ) 
	{
		if ( m_actionRecentFile[ i ]->isVisible() )
		{
			settings.setArrayIndex( i );
			settings.setValue( "file", m_actionRecentFile[ i ]->text() );
		}
	}
	settings.endArray();
}

CImageWindow* CMainWindow::ActiveImageWindow() const
{
    if ( QMdiSubWindow* activeSubWindow = m_mdiArea.activeSubWindow() )
	{
        return qobject_cast<CImageWindow *>( activeSubWindow->widget() );
	}
    return nullptr;
}

void CMainWindow::UpdateToolbar( CImageWindow* imageWindow )
{
	m_channelComboBox.blockSignals( true );
	m_faceComboBox.blockSignals( true );
	m_mipComboBox.blockSignals( true );
	m_visMinSpinBox.blockSignals( true );
	m_visMaxSpinBox.blockSignals( true );
	m_visGammaSpinBox.blockSignals( true );


	m_channelComboBox.setCurrentIndex( 0 );

	m_faceComboBox.clear();
	m_faceComboBox.addItem( "Face: 0" );
	m_faceComboBox.setCurrentIndex( 0 );

	m_mipComboBox.clear();
	m_mipComboBox.addItem( "Mip: 0" );
	m_mipComboBox.setCurrentIndex( 0 );

	m_visMinSpinBox.setValue( 0.0 );
	m_visMaxSpinBox.setValue( 1.0 );
	m_visGammaSpinBox.setValue( 1.0 );

	if ( imageWindow )
	{
		for ( unsigned i = 1; i < imageWindow->GetFaceNum(); ++i )
		{
			m_faceComboBox.addItem( "Face: " + QString::number( i ) );
		}	
		m_faceComboBox.setCurrentIndex( imageWindow->GetViewFace() );

		for ( unsigned i = 1; i < imageWindow->GetMipNum(); ++i )
		{
			m_mipComboBox.addItem( "Mip: " + QString::number( i ) );
		}
		m_mipComboBox.setCurrentIndex( imageWindow->GetViewMipMap() );

		m_channelComboBox.setCurrentIndex( (int) imageWindow->GetViewChannel() );
		m_visMinSpinBox.setValue( imageWindow->GetViewMin() );
		m_visMaxSpinBox.setValue( imageWindow->GetViewMax() );
		m_visGammaSpinBox.setValue( imageWindow->GetViewGamma() );
	}


	m_channelComboBox.blockSignals( false );
	m_faceComboBox.blockSignals( false );
	m_mipComboBox.blockSignals( false );
	m_visMinSpinBox.blockSignals( false );
	m_visMaxSpinBox.blockSignals( false );
	m_visGammaSpinBox.blockSignals( false );
}

void CMainWindow::SetStatusLeft( QString const& status )
{
	m_statusLeft.setText( status );
}

void CMainWindow::SetStatusRight( QString const& status )
{
	m_statusRight.setText( status );
}