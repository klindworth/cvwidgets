#include "simplehistogram.h"

#include <algorithm>
#include <numeric>
#include <QPainter>
#include <QMouseEvent>

SimpleHistogram::SimpleHistogram(QWidget *parent) : QWidget(parent)
{
	setMouseTracking(true);
	m_x_offset = 0;
	m_y_offset = 0;
	m_always_zero_as_axis = true;
}

void SimpleHistogram::setData(const std::vector<BinValue>& bins)
{
	m_bins = bins;
	if(!m_bins.empty())
	{
		//C++11
		//std::pair<BinsIterator, BinsIterator> minmax_count = std::minmax_element(m_bins.begin(), m_bins.end());
		//m_min = *(minmax_count.first);
		//m_max = *(minmax_count.second);
		//C++98
		m_min = *(std::min_element(m_bins.begin(), m_bins.end()));
		m_max = *(std::max_element(m_bins.begin(), m_bins.end()));

		m_sum = std::accumulate(m_bins.begin(), m_bins.end(), static_cast<BinValue>(0));
	}
	updateInternals();
	update();
}

float SimpleHistogram::x_pos_for_bin(std::size_t i) const
{
	return m_x_offset + m_x_per_bin*i;
}

void SimpleHistogram::paintEvent(QPaintEvent *)
{
	bool draw_border = true;

	QPainter painter(this);
	if(draw_border)
		painter.drawRect(0,0,width()-1, height()-1);

	std::size_t cl_count = m_bins.size();

	if(cl_count > 1)
	{
		BinValue min_count = m_always_zero_as_axis ? 0 : m_min;

		if(min_count == m_max)
			return;

		QVector<QPointF> points;
		points.append(QPointF(m_x_offset,height()-m_y_offset));
		for(std::size_t i = 0; i < cl_count; ++i)
		{
			QPointF cpoint(x_pos_for_bin(i), height() - (m_y_per_val * (m_bins[i] - min_count) + m_y_offset));
			points.append(cpoint);
		}

		points.append(QPointF(width(), height()-m_y_offset));

		QPolygonF poly(points);
		painter.setBrush(Qt::SolidPattern);
		painter.drawPolygon(poly);
	}
}

void SimpleHistogram::updateInternals()
{
	std::size_t cl_count = m_bins.size();

	if(cl_count > 1)
	{
		BinValue min_count = m_always_zero_as_axis ? 0 : m_min;

		if(min_count == m_max)
			return;

		m_y_per_val = ((float)this->height()-m_y_offset) / (m_max - min_count);
		m_x_per_bin = ((float)this->width()-m_x_offset) / (cl_count -1);
	}
}

void SimpleHistogram::resizeEvent(QResizeEvent *)
{
	updateInternals();
}

void SimpleHistogram::mouseMoveEvent(QMouseEvent *ev)
{
	if(m_bins.size() > 1)
	{
		float x = ev->x();

		std::size_t bin_pos = (x+ m_x_per_bin/2) / m_x_per_bin;

		if(bin_pos < m_bins.size())
			setToolTip("bin: " + QString::number(bin_pos) + ", value: " + QString::number(m_bins[bin_pos]));
	}
	/*int x = ev->x();
	int idx = getValueIndex(x);
	if(idx >= 0 && idx < m_len)
	{
		setToolTip("disparity: " + QString::number(getDisparity(x)) + ", value: " + QString::number(m_values[idx]));
	}*/
}
