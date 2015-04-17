#include <opencv2/core/core.hpp>
#include <iostream>

#include "hdf5wrapper.h"
#include "hdf5internals.h"

#include <gtest/gtest.h>

TEST(CvIoHDF5, SaveAndLoadSingleChannel)
{
	std::string filename = "simple_test.hdf5";

	cv::Mat_<short> simple_test = (cv::Mat_<short>(2,3) << -1,2,3,4,5,6);
	cvio::save(simple_test, filename, "neuer/test/a");
	cv::Mat_<short> simple_test_restore = cvio::load(filename, "neuer/test/a");
	ASSERT_TRUE(std::equal(simple_test.begin(), simple_test.end(), simple_test_restore.begin()));
}

TEST(CvIoHDF5, SaveAndLoadMultiChannel)
{
	std::string filename = "simple_test.hdf5";

	cv::Mat_<cv::Vec3b> multi_test = (cv::Mat_<cv::Vec3b>(1,2) << cv::Vec3b(100,120,130), cv::Vec3b(200,210,220));
	cvio::save(multi_test, filename, "neuer/test/b");
	cv::Mat_<cv::Vec3b> multi_test_restore = cvio::load(filename, "neuer/test/b");
	ASSERT_TRUE(std::equal(multi_test.begin(), multi_test.end(), multi_test_restore.begin()));

	cv::Mat_<cv::Vec3b> multi_test_longer = (cv::Mat_<cv::Vec3b>(1,3) << cv::Vec3b(110,120,130), cv::Vec3b(200,210,220), cv::Vec3b(10,20,30));
	cvio::save(multi_test_longer, filename, "neuer/test/b");
	cv::Mat_<cv::Vec3b> multi_test_longer_restore = cvio::load(filename, "neuer/test/b");
	ASSERT_TRUE(std::equal(multi_test_longer.begin(), multi_test_longer.end(), multi_test_longer_restore.begin()));

	cv::Mat_<cv::Vec3b> multi_test_shorter = (cv::Mat_<cv::Vec3b>(1,1) << cv::Vec3b(10,20,30));
	cvio::save(multi_test_shorter, filename, "neuer/test/b");
	cv::Mat_<cv::Vec3b> multi_test_shorter_restore = cvio::load(filename, "neuer/test/b");
	ASSERT_TRUE(std::equal(multi_test_shorter.begin(), multi_test_shorter.end(), multi_test_shorter_restore.begin()));
}

TEST(CvIoHDF5, Attributes)
{
	std::string filename = "simple_test.hdf5";
	cv::Mat_<cv::Vec3b> multi_test_shorter = (cv::Mat_<cv::Vec3b>(1,1) << cv::Vec3b(10,20,30));
	cvio::save(multi_test_shorter, filename, "neuer/test/b");

	cvio::hdf5file hfile(filename);
	cvio::hdf5dataset hdataset(hfile, "neuer/test/b");
	hdataset.set_attribute("timestamp", 500);
	hdataset.set_attribute("debugname", "tester");
	//cvio::hdf5attribute_impl::write(hdataset, "timestamp", 500, true);
	//cvio::hdf5attribute_impl::write(hdataset, "debugname", std::string("tester"), true);

	ASSERT_EQ("tester", cvio::hdf5attribute_impl::read<std::string>(hdataset, "debugname"));
	ASSERT_EQ(500, cvio::hdf5attribute_impl::read<int>(hdataset, "timestamp"));

	ASSERT_EQ(2, cvio::hdf5attribute_impl::list_attributes(hdataset).size());

	cvio::hdf5attribute attr(hdataset, "timestamp");
	ASSERT_EQ(500, attr.read<int>());
	ASSERT_EQ("500", attr.read_as_string());
	ASSERT_EQ("int", attr.type_as_string());

	cvio::hdf5attribute attr2(hdataset, "debugname");
	ASSERT_EQ("tester", attr2.read<std::string>());
	ASSERT_EQ("tester", attr2.read_as_string());
	ASSERT_EQ("string", attr2.type_as_string());

	std::vector<std::string> groups = hfile.subgroups("neuer");
	ASSERT_EQ(1, groups.size());
	ASSERT_EQ("test", groups[0]);

	std::vector<std::string> datasets = hfile.datasets("neuer/test");
	ASSERT_EQ(2, datasets.size());
}

