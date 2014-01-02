#include "cvwidgetsset.h"

#include "cvsimpleviewerplugin.h"
#include "cvimageviewerplugin.h"

CVWidgetsSet::CVWidgetsSet(QObject *parent) : QObject(parent)
{
	widgets.append(new CVSimpleViewerPlugin(this));
	widgets.append(new CVImageViewerPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> CVWidgetsSet::customWidgets() const
{
	return widgets;
}

Q_EXPORT_PLUGIN2(cvwidgetssetplugin, CVWidgetsSet)
