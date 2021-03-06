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

#ifndef ABSTRACTIMAGEPROVIDER_H
#define ABSTRACTIMAGEPROVIDER_H

#include <QImage>

/**
 * @brief The ChannelInformation class holds various informations about an image channel
 */
class ChannelInformation
{
public:
	double min, max, mean, stddev;
};

class AbstractImageProvider
{
public:
	virtual ~AbstractImageProvider() {}
	/**
	 * @brief image Returns a QImage representation of this Image
	 * @return QImage instance of the image
	 */
	virtual const QImage& image() const = 0;
	/**
	 * @brief valueInfoString Returns a text presentation of the value at point qpt
	 * @param qpt
	 * @return A QString with a text representation of the value at point qpt
	 */
	virtual QString valueInfoString(const QPoint& qpt) const = 0;

	/**
	 * @brief typeString Returns a text representation of the type of this image (e.g. '16 bit image')
	 * @return
	 */
	virtual QString typeString() const = 0;

	virtual void saveOriginal() const = 0;

	/**
	 * @brief createHistogram Creates a histogram for a single channel
	 * @param number Number of the channel, for which a histogram will be created
	 * @return A vector with all the bins. Since the histogram is created from the viewable interpretation of the image, it will container 256 bins.
	 */
	virtual std::vector<unsigned long> createHistogram(unsigned char number) const = 0;

	/**
	 * @brief channelInformationsCount Returns the number of the available ChannelInformation instances (equal to the number of channels)
	 * @return Channels
	 */
	virtual std::size_t channelInformationsCount() const = 0;

	/**
	 * @brief channelInformation Returns an instance of ChannelInformation with some information like mean, min, etc.
	 * @param channelnr Number of the channel, for which the information should be returned
	 * @return Information about the requested image channel
	 */
	virtual ChannelInformation channelInformation(std::size_t channelnr) const = 0;
};

#endif // ABSTRACTIMAGEPROVIDER_H
