#include "cvsimpleviewer.h"

#include "cvmatprovider.h"

CVSimpleViewer::CVSimpleViewer(QWidget *parent) : SimpleImageViewer(parent)
{
}

void CVSimpleViewer::setCVMat(const cv::Mat &image, bool contrast, bool conversion)
{
	QSharedPointer<CVMatProvider> provider(new CVMatProvider());
	provider->setCVMat(image, contrast, conversion);
	setImageSource(provider);
}
