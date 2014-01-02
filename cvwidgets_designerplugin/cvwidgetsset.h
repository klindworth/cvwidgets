#ifndef CVWIDGETSSET_H
#define CVWIDGETSSET_H

#include <QObject>

#include <QtDesigner/QtDesigner>
#include <QtCore/qplugin.h>

class CVWidgetsSet: public QObject, public QDesignerCustomWidgetCollectionInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

public:
	CVWidgetsSet(QObject *parent = 0);

	virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
	QList<QDesignerCustomWidgetInterface*> widgets;
};

#endif // CVWIDGETSSET_H
