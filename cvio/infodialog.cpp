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

#include "infodialog.h"
#include "ui_infodialog.h"

#include "abstractimageprovider.h"
#include "imagetablemodel.h"

InfoDialog::InfoDialog(QSharedPointer<AbstractImageProvider>& info, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::InfoDialog),
	m_info(info)
{
	ui->setupUi(this);

	ui->lblType->setText(m_info->typeString());
	ui->lblSize->setText(QString("%1x%2").arg(m_info->image().width()).arg(m_info->image().height()));
	m_model = new ImageTableModel(info);
	ui->tableView->setModel(m_model);
	connect(ui->cbBackground, SIGNAL(toggled(bool)), m_model, SLOT(setPixelcolorAsBackground(bool)));
	connect(ui->cbChannelSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(channelSelected(int)));


	std::size_t icount = m_info->channelInformationsCount();
	for(std::size_t i = 0; i < icount; ++i)
		ui->cbChannelSelector->addItem(QString("Channel %1").arg(i));

	//ui->lblStat->setText(QString("mean: %1, stddev: %2").arg(m_info->mean()).arg(m_info->stddev()));
	//ui->lblExtrema->setText(QString("min: %1, max: %2").arg(m_info->min()).arg(m_info->max()));
}

void InfoDialog::channelSelected(int nr)
{
	if(nr >= 0 && nr < (int)m_info->channelInformationsCount())
	{
		ChannelInformation info = m_info->channelInformation(nr);
		ui->lblStat->setText(QString("mean: %1, stddev: %2").arg(info.mean).arg(info.stddev));
		ui->lblExtrema->setText(QString("min: %1, max: %2").arg(info.min).arg(info.max));

		ui->histogram->setData(m_info->createHistogram(nr), m_info->min(), m_info->max());
	}
}

void InfoDialog::scrollTo(int x, int y)
{
	ui->tableView->scrollTo(m_model->index(y,x));
}

InfoDialog::~InfoDialog()
{
	delete ui;
}
