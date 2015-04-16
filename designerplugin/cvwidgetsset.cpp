#include "cvwidgetsset.h"

#include "cvsimpleviewerplugin.h"
#include "cvimageviewerplugin.h"
#include "simplehistogramplugin.h"

CVWidgetsSet::CVWidgetsSet(QObject *parent) : QObject(parent)
{
	widgets.append(new CVSimpleViewerPlugin(this));
	widgets.append(new CVImageViewerPlugin(this));
	widgets.append(new SimpleHistogramPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> CVWidgetsSet::customWidgets() const
{
	return widgets;
}

//Q_EXPORT_PLUGIN2(cvwidgets_designerplugin, CVWidgetsSet)
//Q_EXPORT_PLUGIN(cvwidgets_designerplugin)
