#ifndef HDF5WRAPPER_H
#define HDF5WRAPPER_H

#include <string>

namespace cv {
class Mat;
}

namespace cvio
{
	void save(const cv::Mat& dataset, const std::string& filename, const std::string& name, bool overwrite = true);
	cv::Mat load(const std::string& filename, const std::string& name);
}

#endif // HDF5WRAPPER_H

