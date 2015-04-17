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

#ifndef GENERICIMAGEVIEWER_H
#define GENERICIMAGEVIEWER_H

#include <QWidget>
#include <QPointer>
#include "cvwidgets_config.h"

namespace Ui {
class ImageViewer;
}

class AbstractImageProvider;
class SimpleImageViewer;
class QToolBar;
class QComboBox;
class QLabel;

/**
 * @brief The GenericImageViewer class
 * An image viewer widget with some control widgets (zoom)
 */
class CV_WIDGET_LIBSHARED_EXPORT GenericImageViewer : public QWidget
{
	Q_OBJECT

public:
	explicit GenericImageViewer(QWidget *parent = 0);
	void setImageProvider(const QSharedPointer<AbstractImageProvider>& provider);
	SimpleImageViewer *viewer();
	~GenericImageViewer();

private slots:
	void slot_mouseMoved(int x, int y);
	void slot_zoomChanged(int step);
	void on_cbZoomsteps_currentIndexChanged(int index);

private:
	QSharedPointer<AbstractImageProvider> m_imageprovider;

	SimpleImageViewer *m_image;
	QToolBar *m_toolbar;
	QComboBox *m_cbZoomsteps;
	QLabel *m_lblInfo;
};

#endif // GENERICIMAGEVIEWER_H
