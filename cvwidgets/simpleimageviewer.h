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

#ifndef SIMPLEIMAGEVIEWER_H
#define SIMPLEIMAGEVIEWER_H

#include <QWidget>
#include <QPointer>
#include "cvwidgets_config.h"

class AbstractImageProvider;
class InfoDialog;

class CV_WIDGET_LIBSHARED_EXPORT SimpleImageViewer : public QWidget
{
	friend class GenericImageViewer;
	Q_OBJECT
public:
	explicit SimpleImageViewer(QWidget *parent = 0);
	~SimpleImageViewer();
	void setImageSource(const QSharedPointer<AbstractImageProvider>& provider);
	void refreshDisplay();

	//Functions to add a marker
	/**
	 * @brief addRect Adds a rectangular marker
	 * @param rect
	 */
	void addRect(const QRect& rect);
	/**
	 * @brief addCircle Adds a circular marker
	 * @param pt Center of the circle
	 * @param rad Radius of the circle
	 */
	void addCircle(const QPoint& pt, int rad);
	/**
	 * @brief addPoint Adds a point (depicted as cross) as marker
	 * @param pt
	 */
	void addPoint(const QPoint& pt);
	/**
	 * @brief addLine Adds a line as marker
	 * @param line
	 */
	void addLine(const QLine& line);
	/**
	 * @brief resetMarkers Deletes all markers
	 */
	void resetMarkers();

protected:
	void paintEvent(QPaintEvent * event);
	void mouseReleaseEvent(QMouseEvent *ev);
	void mouseMoveEvent(QMouseEvent *ev);
	void wheelEvent ( QWheelEvent * event );

	//Zooming
	int currentZoomstep() const;
	float currentScaling() const;
	const float* zoomstepsBegin() const;
	const float* zoomstepsEnd() const;

	static float m_zoomsteps[15];
	static int m_zoomsteps_count;

	float m_scaling;
	int m_zoomstep_idx;

	//Ressources
	QPixmap m_scaledPixmap;
	QSharedPointer<AbstractImageProvider> m_imagesource;
	QPointer<InfoDialog> m_infodialog;

	//Markers
	bool m_bShowmarkers;
	QList<QRect> m_rects;
	QList<std::pair<QPoint, int> > m_circles;
	QList<QPoint> m_points;
	QList<QLine> m_lines;

	//Actions
	QAction *m_zoomIn, *m_zoomOut, *m_showmarkers;

signals:
	void mouseClicked(int x, int y);
	void mouseMoved(int x, int y);
	void zoomstepChanged(int step);

public slots:
	void saveImage();
	void setScaling(float scaling);
	void setZoomstep(int step);
	void zoomIn();
	void zoomOut();
	void showMarkers(bool show=true);
	void showInfoDialog();
};

#endif // SIMPLEIMAGEVIEWER_H
