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

#include "imagetablemodel.h"
#include "abstractimageprovider.h"

#include <QColor>

ImageTableModel::ImageTableModel(QSharedPointer<AbstractImageProvider>& provider, QObject *parent) : QAbstractTableModel(parent), m_provider(provider)
{
	m_pixelbackground = true;
}

int ImageTableModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_provider->image().height();
}

int ImageTableModel::columnCount(const QModelIndex &/*parent*/) const
{
	return m_provider->image().width();
}

QVariant ImageTableModel::data(const QModelIndex &index, int role) const
{
	if(role == Qt::DisplayRole)
	{
		return m_provider->valueInfoString(QPoint(index.column(), index.row()));
	}
	else if(role == Qt::BackgroundColorRole)
	{
		if(m_pixelbackground)
			return QColor(m_provider->image().pixel(QPoint(index.column(), index.row())));
		else
			return QVariant();
	}
	else
		return QVariant();
}

QVariant ImageTableModel::headerData(int section, Qt::Orientation, int role) const
{
	if(role == Qt::DisplayRole)
		return QString::number(section);
	else
		return QVariant();
}

void ImageTableModel::setPixelcolorAsBackground(bool background)
{
	m_pixelbackground = background;
	//FIXME alternative?
	//reset();
}
