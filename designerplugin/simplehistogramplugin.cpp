#include <cvio/simplehistogram.h>
#include "simplehistogramplugin.h"
#include <vector>

#include <QtPlugin>

SimpleHistogramPlugin::SimpleHistogramPlugin(QObject *parent)
	: QObject(parent)
{
	m_initialized = false;
}

void SimpleHistogramPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
	if (m_initialized)
		return;

	// Add extension registrations, etc. here

	m_initialized = true;
}

bool SimpleHistogramPlugin::isInitialized() const
{
	return m_initialized;
}

QWidget *SimpleHistogramPlugin::createWidget(QWidget *parent)
{
	srand(0);
	std::vector<unsigned long> bins(100);
	for(int i = 0; i < 100; ++i)
		bins[i] = rand() % 100;
	SimpleHistogram* hist = new SimpleHistogram(parent);
	hist->setData(bins);

	return hist;
}

QString SimpleHistogramPlugin::name() const
{
	return QLatin1String("SimpleHistogram");
}

QString SimpleHistogramPlugin::group() const
{
	return QLatin1String("CVWidgets");
}

QIcon SimpleHistogramPlugin::icon() const
{
	return QIcon();
}

QString SimpleHistogramPlugin::toolTip() const
{
	return QLatin1String("");
}

QString SimpleHistogramPlugin::whatsThis() const
{
	return QLatin1String("");
}

bool SimpleHistogramPlugin::isContainer() const
{
	return false;
}

QString SimpleHistogramPlugin::domXml() const
{
	return QLatin1String("<widget class=\"SimpleHistogram\" name=\"SimpleHistogram\">\n</widget>\n");
}

QString SimpleHistogramPlugin::includeFile() const
{
	return QLatin1String("cvio/simplehistogram.h");
}
//if QT_VERSION < 0x050000
//Q_EXPORT_PLUGIN2(SimpleHistogramPlugin, SimpleHistogramPlugin)
//endif // QT_VERSION < 0x050000
