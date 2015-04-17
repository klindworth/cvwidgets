#ifndef SIMPLEHISTOGRAM_H
#define SIMPLEHISTOGRAM_H

#include <QWidget>
#include <vector>
#include "cvwidgets_config.h"

class CV_WIDGET_LIBSHARED_EXPORT SimpleHistogram : public QWidget
{
	Q_OBJECT
	Q_PROPERTY(bool logYAxis READ logYAxis WRITE setLogYAxis)

	typedef unsigned long BinValue;
	typedef std::vector<BinValue>::iterator BinsIterator;

public:
	explicit SimpleHistogram(QWidget *parent = 0);
	void setData(const std::vector<BinValue>& bins, double min, double max);
	bool logYAxis() const;

public slots:
	void setLogYAxis(bool enable);

protected:
	void paintEvent(QPaintEvent*);
	void mouseMoveEvent(QMouseEvent *ev);
	void resizeEvent(QResizeEvent *);
private:
	void updateInternals();
	float x_pos_for_bin(std::size_t i) const;
	float transformed_value(unsigned long value) const;

	bool m_always_zero_as_axis;
	bool m_log_y_axis;
	std::vector<BinValue> m_bins;
	BinValue m_min, m_max, m_sum;
	float m_actual_min, m_actual_max;
	float m_x_offset, m_y_offset;
	float m_y_per_val, m_x_per_bin;
	double m_min_cl, m_max_cl;
};

#endif // SIMPLEHISTOGRAM_H
