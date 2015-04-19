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

#include "simpleimageviewer.h"

#include <QPixmap>
#include <QPainter>
#include <QMouseEvent>
#include <QImage>
#include <QMenu>
#include <QFileDialog>

#include "abstractimageprovider.h"
#include "infodialog.h"

float SimpleImageViewer::m_zoomsteps[] = {0.05, 0.1, 0.125, 0.25, 0.33, 0.5, 0.75, 1.0, 1.25, 1.5, 2.0, 3.0, 4.0, 5.0, 10.0};
int SimpleImageViewer::m_zoomsteps_count = 15;


SimpleImageViewer::SimpleImageViewer(QWidget *parent) :
	QWidget(parent)
{
	m_bShowmarkers = true;
	m_zoomstep_idx = 7;
	m_scaling = 1.0;

	//init actions for context menu (and toolbar of the generic image viewer)
	QAction *action_save = new QAction(QIcon::fromTheme("document-save-as"), "Save image", this);
	connect(action_save, SIGNAL(triggered()), this, SLOT(saveImage()));

	//QAction *action_original = new QAction(QIcon::fromTheme("document-save-as"), "Save original", this);
	//connect(action_original, SIGNAL(triggered()), this, SLOT(saveOriginal()));

	QAction *action_info = new QAction(QIcon::fromTheme("dialog-information"), "Open info dialog", this);
	connect(action_info, SIGNAL(triggered()), this, SLOT(showInfoDialog()));

	m_zoomIn = new QAction(QIcon::fromTheme("zoom-in"), "Zoom in", this);
	connect(m_zoomIn, SIGNAL(triggered()), this, SLOT(zoomIn()));

	m_zoomOut = new QAction(QIcon::fromTheme("zoom-out"), "Zoom out", this);
	connect(m_zoomOut, SIGNAL(triggered()), this, SLOT(zoomOut()));

	m_showmarkers = new QAction("Show markers", this);
	m_showmarkers->setCheckable(true);
	m_showmarkers->setChecked(true);
	connect(m_showmarkers, SIGNAL(triggered(bool)), this, SLOT(showMarkers(bool)));

	this->addAction(m_zoomIn);
	this->addAction(m_zoomOut);
	this->addAction(action_save);
	this->addAction(action_info);
	this->addAction(m_showmarkers);
	//this->addAction(action_original);

	this->setContextMenuPolicy(Qt::ActionsContextMenu);
}

SimpleImageViewer::~SimpleImageViewer()
{
}

void SimpleImageViewer::saveImage()
{
	if(!m_imagesource.isNull())
	{
		QString name = QFileDialog::getSaveFileName(this, "Choose place", QString(), "*.png");

		if(! name.isEmpty())
			m_imagesource->image().save(name);
	}
}

void SimpleImageViewer::setScaling(float scaling)
{
	m_scaling = scaling;
	refreshDisplay();
}

void SimpleImageViewer::refreshDisplay()
{
	if(!m_imagesource.isNull())
	{
		if(!m_imagesource->image().isNull())
		{
			m_scaledPixmap = QPixmap::fromImage(m_imagesource->image().scaled(m_imagesource->image().size()*m_scaling));
			setMinimumSize(m_scaledPixmap.size());
			update();
		}
	}
}

void SimpleImageViewer::setImageSource(const QSharedPointer<AbstractImageProvider> &provider)
{
	m_imagesource = provider;
	refreshDisplay();
}

void SimpleImageViewer::paintEvent(QPaintEvent * /*event*/)
{
	QPainter painter(this);
	if(!m_scaledPixmap.isNull())
	{
		painter.drawPixmap(0,0, m_scaledPixmap);

		if(m_bShowmarkers)
		{
			QPen pen;
			pen.setColor(Qt::red);
			painter.setPen(pen);

			for(QList<QRect>::iterator it = m_rects.begin(); it != m_rects.end(); ++it)
			{
				QRect rect(it->topLeft()*m_scaling, it->size() * m_scaling);
				painter.drawRect(rect);
			}

			for(QList<std::pair<QPoint,int> >::iterator it = m_circles.begin(); it != m_circles.end(); ++it)
			{
				QPoint center = it->first * m_scaling;
				int rad = it->second * m_scaling;
				painter.drawEllipse(center, rad, rad);
			}

			for(QList<QPoint>::iterator it = m_points.begin(); it != m_points.end(); ++it)
			{
				const int cross_size = 3;
				QPoint center = *it * m_scaling;
				painter.drawLine(center.x()-cross_size,center.y(), center.x()+cross_size,center.y());
				painter.drawLine(center.x(),center.y()-cross_size, center.x(),center.y()+cross_size);
			}

			for(QList<QLine>::iterator it = m_lines.begin(); it != m_lines.end(); ++it)
			{
				QLine line = QLine(it->x1() * m_scaling, it->y1() * m_scaling, it->x2() * m_scaling, it->y2() * m_scaling);
				painter.drawLine(line);
			}
		}
	}
}

void SimpleImageViewer::mouseMoveEvent(QMouseEvent *ev)
{
	if(!m_imagesource.isNull())
	{
		QPoint pt = ev->pos() / m_scaling;
		if(pt.x() < m_imagesource->image().width() && pt.y() < m_imagesource->image().height() )
			emit mouseMoved(pt.x(), pt.y());
	}
}

void SimpleImageViewer::mouseReleaseEvent(QMouseEvent *ev)
{
	if( !m_imagesource.isNull() && ev->button() == Qt::LeftButton)
	{
		QPoint pt = ev->pos() / m_scaling;
		if(pt.x() < m_imagesource->image().width() && pt.y() < m_imagesource->image().height() )
			emit mouseClicked(pt.x(), pt.y());

		if(!m_infodialog.isNull())
			m_infodialog->scrollTo(pt.x(), pt.y());
	}
}

void SimpleImageViewer::setZoomstep(int step)
{
	m_zoomstep_idx = std::max(0, std::min(step, m_zoomsteps_count-1));
	setScaling(SimpleImageViewer::m_zoomsteps[m_zoomstep_idx]);


	m_zoomOut->setEnabled(true);
	m_zoomIn->setEnabled(true);
	if(m_zoomstep_idx == 0)
		m_zoomOut->setEnabled(false);
	if(m_zoomstep_idx == m_zoomsteps_count - 1)
		m_zoomIn->setEnabled(false);

	emit zoomstepChanged(m_zoomstep_idx);
}

void SimpleImageViewer::zoomIn()
{
	setZoomstep(m_zoomstep_idx + 1);
}

void SimpleImageViewer::zoomOut()
{
	setZoomstep(m_zoomstep_idx - 1);
}

int SimpleImageViewer::currentZoomstep() const
{
	return m_zoomstep_idx;
}

float SimpleImageViewer::currentScaling() const
{
	return m_scaling;
}

const float* SimpleImageViewer::zoomstepsBegin() const
{
	return m_zoomsteps;
}

const float* SimpleImageViewer::zoomstepsEnd() const
{
	return m_zoomsteps + m_zoomsteps_count;
}

void SimpleImageViewer::wheelEvent ( QWheelEvent * event )
{
	if(event->modifiers() == Qt::ControlModifier)
	{
		int delta = event->delta() > 0 ? 1 : -1;
		setZoomstep(m_zoomstep_idx + delta);

		event->accept();
	}
	else
		event->ignore();
}

void SimpleImageViewer::addRect(const QRect& rect)
{
	m_rects.append(rect);
}

void SimpleImageViewer::addCircle(const QPoint& pt, int rad)
{
	m_circles.append(std::make_pair(pt, rad));
}

void SimpleImageViewer::addPoint(const QPoint& pt)
{
	m_points.append(pt);
}

void SimpleImageViewer::addLine(const QLine &line)
{
	m_lines.append(line);
}

void SimpleImageViewer::resetMarkers()
{
	m_rects.clear();
	m_circles.clear();
	m_points.clear();
}

void SimpleImageViewer::showMarkers(bool show)
{
	m_bShowmarkers = show;
	update();
}

void SimpleImageViewer::showInfoDialog()
{
	if(m_imagesource)
	{
		if(m_infodialog.isNull())
			m_infodialog = QPointer<InfoDialog>(new InfoDialog(m_imagesource));
		m_infodialog->show();
	}
}

void SimpleImageViewer::saveOriginal()
{
	if(m_imagesource)
	{
		m_imagesource->saveOriginal();
	}
}
