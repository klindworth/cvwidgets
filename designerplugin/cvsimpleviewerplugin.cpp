#include <cvsimpleviewer.h>
#include "cvsimpleviewerplugin.h"

#include <QtPlugin>

CVSimpleViewerPlugin::CVSimpleViewerPlugin(QObject *parent)
	: QObject(parent)
{
	m_initialized = false;
}

void CVSimpleViewerPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool CVSimpleViewerPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget *CVSimpleViewerPlugin::createWidget(QWidget *parent)
{
	return new CVSimpleViewer(parent);
}

QString CVSimpleViewerPlugin::name() const
{
	return QLatin1String("CVSimpleViewer");
}

QString CVSimpleViewerPlugin::group() const
{
	return QLatin1String("CVWidgets");
}

QIcon CVSimpleViewerPlugin::icon() const
{
	return QIcon();
}

QString CVSimpleViewerPlugin::toolTip() const
{
	return QLatin1String("");
}

QString CVSimpleViewerPlugin::whatsThis() const
{
	return QLatin1String("");
}

bool CVSimpleViewerPlugin::isContainer() const
{
	return false;
}

QString CVSimpleViewerPlugin::domXml() const
{
	return QLatin1String("<widget class=\"CVSimpleViewer\" name=\"cVSimpleViewer\">\n</widget>\n");
}

QString CVSimpleViewerPlugin::includeFile() const
{
	return QLatin1String("cvsimpleviewer.h");
}
//if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(cvsimpleviewerplugin, CVSimpleViewerPlugin)
//endif // QT_VERSION < 0x050000
