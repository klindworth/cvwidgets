#include <cvimageviewer.h>
#include "cvimageviewerplugin.h"

#include <QtPlugin>

CVImageViewerPlugin::CVImageViewerPlugin(QObject *parent)
	: QObject(parent)
{
	m_initialized = false;
}

void CVImageViewerPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool CVImageViewerPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget *CVImageViewerPlugin::createWidget(QWidget *parent)
{
	return new CVImageViewer(parent);
}

QString CVImageViewerPlugin::name() const
{
	return QLatin1String("CVImageViewer");
}

QString CVImageViewerPlugin::group() const
{
	return QLatin1String("CVWidgets");
}

QIcon CVImageViewerPlugin::icon() const
{
	return QIcon();
}

QString CVImageViewerPlugin::toolTip() const
{
	return QLatin1String("");
}

QString CVImageViewerPlugin::whatsThis() const
{
	return QLatin1String("");
}

bool CVImageViewerPlugin::isContainer() const
{
	return false;
}

QString CVImageViewerPlugin::domXml() const
{
	return QLatin1String("<widget class=\"CVImageViewer\" name=\"cVImageViewer\">\n</widget>\n");
}

QString CVImageViewerPlugin::includeFile() const
{
	return QLatin1String("cvio/cvimageviewer.h");
}
//if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(cvimageviewerplugin, CVImageViewerPlugin)
//endif // QT_VERSION < 0x050000
