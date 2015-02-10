/*
Copyright (c) 2013, Kai Klindworth
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "cvmatprovider.h"

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <QFileDialog>

cv::Mat valueScaledImage(const cv::Mat& image)
{
	double dmin, dmax;
	cv::minMaxIdx(image, &dmin, &dmax);
	double scale = 255/(dmax-dmin);

	cv::Mat result;
	if(image.channels() == 1)
		image.convertTo(result, CV_8UC1, scale, -dmin*scale);
	else if(image.channels() == 3)
		image.convertTo(result, CV_8UC3, scale, -dmin*scale);

	return result;
}

cv::Mat CVMatProvider::createRGBMat(const cv::Mat &image, bool contrast, bool conversion)
{
	cv::Mat result;
	if(image.data)
	{
		assert(image.dims == 2);

		cv::Mat temp;
		if(contrast || (image.type() != CV_8UC1 && image.type() != CV_8UC3))
			temp = valueScaledImage(image);
		else
			temp = image;

		if(image.channels() == 1)
			cv::cvtColor(temp, result, CV_GRAY2RGB);
		else if(conversion)
			cv::cvtColor(temp, result, CV_BGR2RGB);
		else
			result = temp;

		assert(result.data);
		assert(result.channels() == 3);
	}
	return result;
}

void CVMatProvider::setCVMat(const cv::Mat &image, bool contrast, bool conversion)
{
	if(image.data)
	{
		m_original = image;
		m_mat = createRGBMat(image, contrast, conversion);
		m_image = QImage(m_mat.data, m_mat.cols, m_mat.rows, m_mat.step, QImage::Format_RGB888);
	}
}

template<typename T>
void getChannelText(std::vector<QString>& channeltext, const cv::Mat& image, cv::Point2i pt)
{
	int channels = image.channels();
	for(int c = 0; c < channels; ++c)
	{
		T temp = image.at<T>((pt.y * image.cols + pt.x)*channels+c);
		channeltext[c] = QString::number(temp);
	}
}

QString CVMatProvider::minmax() const
{
	double dmin, dmax;
	cv::minMaxIdx(m_original, &dmin, &dmax);

	return QString("min: %1, max: %2").arg(dmin).arg(dmax);
}

QString CVMatProvider::typeString() const
{
	QString ftype;
	int type = m_original.depth();
	if(type == CV_8U)
		ftype = "CV_8U";
	else if(type == CV_16U)
		ftype = "CV_16U";
	else if(type == CV_16S)
		ftype = "CV_16S";
	else if(type == CV_32S)
		ftype = "CV_32S";
	else if(type == CV_32F)
		ftype = "CV_32F";
	else if(type == CV_64F)
		ftype = "CV_64F";


	return QString("OpenCV, depth: %1, channels: %2").arg(ftype).arg(m_original.channels());
}

QString CVMatProvider::valueInfoString(const QPoint& qpt) const
{
	QString valuetext;
	if(m_original.data)
	{
		std::vector<QString> channeltext(m_original.channels());

		int type = m_original.depth();
		int channels = m_original.channels();

		cv::Point pt = cv::Point2i(qpt.x(),qpt.y());

		if(type == CV_8U)
			getChannelText<unsigned char>(channeltext, m_original, pt);
		else if(type == CV_16U)
			getChannelText<unsigned short>(channeltext, m_original, pt);
		else if(type == CV_16S)
			getChannelText<short>(channeltext, m_original, pt);
		else if(type == CV_32S)
			getChannelText<int>(channeltext, m_original, pt);
		else if(type == CV_32F)
			getChannelText<float>(channeltext, m_original, pt);
		else if(type == CV_64F)
			getChannelText<double>(channeltext, m_original, pt);


		if(channels == 1)
			valuetext = channeltext[0];
		if(channels == 3)
			valuetext = QString("B: %1, G: %2, R: %3").arg(channeltext[0], channeltext[1], channeltext[2]);
		else
		{
			QString valuetext = "";
			for(int i = 0; i < channels; ++i)
				valuetext += QString("%1: %2,").arg(i).arg(channeltext[i]);
		}
	}
	return valuetext;
}

void CVMatProvider::saveOriginal() const
{
	QString path = QFileDialog::getSaveFileName();
	if(!path.isEmpty())
	{

	}
}
