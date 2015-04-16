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

#ifndef CVMATPROVIDER_H
#define CVMATPROVIDER_H

#include "abstractimageprovider.h"
#include "cvwidgets_config.h"

#include <opencv2/core/core.hpp>

class CV_WIDGET_LIBSHARED_EXPORT CVMatProvider : public AbstractImageProvider
{
public:
	CVMatProvider() {}
	/**
	 * @brief CVMatProvider For explaination of the parameter see @link createRGBMat
	 */
	CVMatProvider(const cv::Mat& mat, bool contrast = false, bool conversion = true) { setCVMat(mat, contrast, conversion); }
	~CVMatProvider() {}
	/**
	 * @brief setCVMat For explaination of the parameter see @link createRGBMat
	 */
	void setCVMat(const cv::Mat &image, bool contrast = false, bool conversion = true);
	const QImage& image() const { return m_image; }
	const cv::Mat& mat() const { return m_mat; }
	QString valueInfoString(const QPoint& qpt) const;
	QString minmax() const;
	QString typeString() const;
	void saveOriginal() const;

	std::vector<unsigned long> createHistogram(unsigned char number) const;
	unsigned char histogramsAvailable() const;

	/**
	 * @brief createRGBMat Creates a RGB representation of a cv::Mat
	 * @param image Image, that will be converted
	 * @param contrast If this parameter is set to true, the minimum value of the image will be 0, the maximum will be 255. This is always done for images, that are not CV_8UC.
	 * @param conversion If this parameter is set to true, the image is converted to an RGB image. If the image is already an RGB image (note: OpenCV creates by default BGR images!), set this to false.
	 * @return Returns always a RGB CV_8UC image.
	 */
	static cv::Mat createRGBMat(const cv::Mat &image, bool contrast = false, bool conversion = true);
private:
	cv::Mat m_mat, m_original;
	QImage m_image;
};
#endif // CVMATPROVIDER_H
