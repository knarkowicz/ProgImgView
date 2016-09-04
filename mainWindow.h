#pragma once

class CImageWindow;
class QAction;
class QMenu;
class QMdiArea;
class QMdiSubWindow;

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    CMainWindow();
	void SetStatusLeft( QString const& status );
	void SetStatusRight( QString const& status );


private slots:
    void Open();
	void ReloadCurrent();
    void OpenRecentFile();
	void ChangeChannel( int channel );
	void ChangeFace( int face );
	void ChangeMip( int mip );
	void ChangeVisMin( float visMin );
	void ChangeVisMax( float visMax );
	void ChangeVisGamma( float visGamma );
	void SubWindowActivated( QMdiSubWindow* window );


private:
	void UpdateToolbar( CImageWindow* imageWindow );
	void UpdateTitle( CImageWindow* imageWindow );
	void closeEvent( QCloseEvent* event ) Q_DECL_OVERRIDE;
	void dragEnterEvent( QDragEnterEvent* event ) Q_DECL_OVERRIDE;
	void dropEvent( QDropEvent* event ) Q_DECL_OVERRIDE;
    void CreateToolbar();
    void ReadSettings();
    void WriteSettings();
	void AddToRecentFiles( QString const& fileName );
    bool OpenFile( QString const& fileName);
    CImageWindow* ActiveImageWindow() const;

	QString			m_lastDirectory;
    QMdiArea		m_mdiArea;
    QMenu*			m_windowMenu;
	QComboBox		m_channelComboBox;
	QComboBox		m_faceComboBox;
	QComboBox		m_mipComboBox;
	QDoubleSpinBox	m_visMinSpinBox;
	QDoubleSpinBox	m_visMaxSpinBox;
	QDoubleSpinBox	m_visGammaSpinBox;
	QLabel			m_statusLeft;
	QLabel			m_statusRight;
    QAction*		m_actionRecentFile[ 10 ];
    QAction*		m_actionRecentFileSeparator;
    QAction*		m_actionRecentFileSubMenuAct;
	QAction*		m_actionOpen;
	QAction*		m_actionReloadCurrent;
    QAction*		m_actionClose;
    QAction*		m_actionCloseAll;
    QAction*		m_actionTile;
    QAction*		m_actionCascade;
    QAction*		m_actionNext;
    QAction*		m_actionPrevious;
    QAction*		m_actionWindowMenuSeparator;
};