#ifndef CVSIMPLEVIEWER_H
#define CVSIMPLEVIEWER_H

#include "simpleimageviewer.h"

#include "cvwidgets_config.h"

namespace cv {
	class Mat;
}

class CV_WIDGET_LIBSHARED_EXPORT CVSimpleViewer : public SimpleImageViewer
{
	Q_OBJECT
public:
	CVSimpleViewer(QWidget *parent = 0);
	void setCVMat(const cv::Mat &image, bool contrast = false, bool conversion = true);
};

#endif // CVSIMPLEVIEWER_H
