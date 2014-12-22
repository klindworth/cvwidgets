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

#include "genericimageviewer.h"
#include "simpleimageviewer.h"
#include "abstractimageprovider.h"
#include <QWheelEvent>

#include <QComboBox>
#include <QToolBar>
#include <QBoxLayout>
#include <QScrollArea>
#include <QLabel>

GenericImageViewer::GenericImageViewer(QWidget *parent) :
	QWidget(parent)
{
	QScrollArea *scrollArea = new QScrollArea(this);
	m_image = new SimpleImageViewer(this);
	m_cbZoomsteps = new QComboBox(this);
	m_toolbar = new QToolBar(this);
	m_toolbar->addWidget(new QLabel("Zoom", this));
	m_toolbar->addWidget(m_cbZoomsteps);
	m_toolbar->addActions(m_image->actions());
	m_lblInfo = new QLabel(this);

	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->addWidget(m_toolbar);
	layout->addWidget(scrollArea);
	layout->addWidget(m_lblInfo);

	m_image->setMouseTracking(true);

	connect(m_image, SIGNAL(mouseMoved(int,int)), this, SLOT(slot_mouseMoved(int,int)));

	for(const float* it = m_image->zoomstepsBegin(); it != m_image->zoomstepsEnd(); ++it)
		m_cbZoomsteps->addItem(QString("%1%").arg((int)(*it * 100)));

	m_cbZoomsteps->setCurrentIndex(7);
	connect(m_cbZoomsteps, SIGNAL(currentIndexChanged(int)), this, SLOT(on_cbZoomsteps_currentIndexChanged(int)));


	connect(m_image, SIGNAL(zoomstepChanged(int)), this, SLOT(slot_zoomChanged(int)));
	setEnabled(false);


	QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	sizePolicy.setHorizontalStretch(1);
	sizePolicy.setVerticalStretch(1);
	sizePolicy.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
	scrollArea->setSizePolicy(sizePolicy);
	scrollArea->setWidgetResizable(true);
	scrollArea->setWidget(m_image);
}

void GenericImageViewer::setImageProvider(const QSharedPointer<AbstractImageProvider>& provider)
{
	if(provider)
	{
		m_imageprovider = provider;
		m_image->setImageSource(provider);
		m_image->refreshDisplay();
		setEnabled(true);
		setBaseSize(m_image->size());
	}
}

GenericImageViewer::~GenericImageViewer()
{
	//delete ui;
}

SimpleImageViewer* GenericImageViewer::viewer()
{
	return m_image;
}

void GenericImageViewer::slot_mouseMoved(int x, int y)
{
	m_lblInfo->setText(QString("(x: %1, y: %2), %3").arg(x).arg(y).arg(m_imageprovider->valueInfoString(QPoint(x,y))));
}

void GenericImageViewer::on_cbZoomsteps_currentIndexChanged(int index)
{
	if(index != m_image->currentZoomstep())
		m_image->setZoomstep(index);
}

void GenericImageViewer::slot_zoomChanged(int step)
{
	m_cbZoomsteps->setCurrentIndex(step);
}

