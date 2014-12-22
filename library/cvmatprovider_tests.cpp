#include "cvmatprovider.h"

#include <iostream>

#include <gtest/gtest.h>

template<typename T>
void matrix_equality(const cv::Mat_<T>& expected, const cv::Mat_<T>& actual)
{
	ASSERT_EQ(expected.size(), actual.size());

	const T* ptr_expected = expected[0];
	const T* ptr_actual = actual[0];

	for(unsigned int i = 0; i < expected.total(); ++i)
	{
		ASSERT_EQ(*ptr_expected++, *ptr_actual++);
	}
}

TEST(CVMatProviderTest, FloatPosImage)
{
	cv::Mat_<float> input = (cv::Mat_<float>(3,1) << 1.0f, 2.0f, 3.0f);
	cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(0,0,0), cv::Vec3b(128,128,128), cv::Vec3b(255,255,255));

	CVMatProvider prov(input);

	cv::Mat_<cv::Vec3b> results = prov.mat();

	matrix_equality(expected, results);
}

TEST(CVMatProviderTest, BigFloatPosImage)
{
	cv::Mat_<float> input = (cv::Mat_<float>(3,1) << 500.0f, 1000.0f, 1500.0f);
	cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(0,0,0), cv::Vec3b(128,128,128), cv::Vec3b(255,255,255));

	CVMatProvider prov(input);

	cv::Mat_<cv::Vec3b> results = prov.mat();

	matrix_equality(expected, results);
}

TEST(CVMatProviderTest, BigColorFloatPosImage)
{
	cv::Mat_<cv::Vec3f> input = (cv::Mat_<cv::Vec3f>(3,1) << cv::Vec3f(500.0f, 1000.0f, 1000.0f), cv::Vec3f(1500.0f, 1500.0f, 500.0f), cv::Vec3f(500.0f, 1000.0f, 1500.0f));
	//cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(0,128,128), cv::Vec3b(255,255,0), cv::Vec3b(0,128,255));
	//because it converts from BGR to RGB it's the following:
	cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(128,128,0), cv::Vec3b(0,255,255), cv::Vec3b(255,128,0));

	CVMatProvider prov(input);

	cv::Mat_<cv::Vec3b> results = prov.mat();

	matrix_equality(expected, results);
}

TEST(CVMatProviderTest, FloatPosNegImage)
{
	cv::Mat_<float> input = (cv::Mat_<float>(3,1) << -1.0f, 0.0f, 1.0f);
	cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(0,0,0), cv::Vec3b(128,128,128), cv::Vec3b(255,255,255));

	CVMatProvider prov(input);

	cv::Mat_<cv::Vec3b> results = prov.mat();

	matrix_equality(expected, results);
}

TEST(CVMatProviderTest, ShortPosImage)
{
	cv::Mat_<short> input = (cv::Mat_<short>(3,1) << 1, 2, 3);
	cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(0,0,0), cv::Vec3b(128,128,128), cv::Vec3b(255,255,255));

	CVMatProvider prov(input);

	cv::Mat_<cv::Vec3b> results = prov.mat();

	matrix_equality(expected, results);
}

TEST(CVMatProviderTest, BigShortPosImage)
{
	cv::Mat_<short> input = (cv::Mat_<short>(3,1) << 1000, 2000, 3000);
	cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(0,0,0), cv::Vec3b(128,128,128), cv::Vec3b(255,255,255));

	CVMatProvider prov(input);

	cv::Mat_<cv::Vec3b> results = prov.mat();

	matrix_equality(expected, results);
}

TEST(CVMatProviderTest, ShortPosNegImage)
{
	cv::Mat_<short> input = (cv::Mat_<short>(3,1) << -1, 0, 1);
	cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(0,0,0), cv::Vec3b(128,128,128), cv::Vec3b(255,255,255));

	CVMatProvider prov(input);

	cv::Mat_<cv::Vec3b> results = prov.mat();

	matrix_equality(expected, results);
}

TEST(CVMatProviderTest, UnsignedShortImage)
{
	cv::Mat_<unsigned short> input = (cv::Mat_<unsigned short>(3,1) << 1, 2, 3);
	cv::Mat_<cv::Vec3b> expected = (cv::Mat_<cv::Vec3b>(3,1) << cv::Vec3b(0,0,0), cv::Vec3b(128,128,128), cv::Vec3b(255,255,255));

	CVMatProvider prov(input);

	cv::Mat_<cv::Vec3b> results = prov.mat();

	matrix_equality(expected, results);
}
