#include <QApplication>
#include <opencv2/core/core.hpp>

#include <cvimageviewer.h>
#include <cvsimpleviewer.h>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	//Creation of the test image
	cv::Mat test = cv::Mat(50, 50, CV_64FC1, cv::Scalar(1.5f));
	for(int i = 0; i < 25; ++i)
	{
		for(int j = 0; j < 25; ++j)
			test.at<double>(i,j) = 4.55f;
	}

	for(int i = 25; i < 50; ++i)
	{
		for(int j = 25; j < 50; ++j)
			test.at<double>(i,j) = -3.25f;
	}

	//create a CVImageViewer instance, set as image the previously created cv::Mat and show the widget
	CVImageViewer viewer;
	viewer.setCVMat(test);
	viewer.viewer()->addPoint(QPoint(25,25)); //viewer() returns a CVSimpleViewer instance, therefore you need to include cvsimpleviewer.h
	viewer.show();

	return a.exec();
}
