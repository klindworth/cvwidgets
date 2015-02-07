#include "hdf5wrapper.h"

#include "hdf5internals.h"
#include <opencv2/core/core.hpp>

namespace cvio
{
void save(const cv::Mat& mat, const std::string& filename, const std::string& name, bool overwrite)
{
	hdf5file hfile(filename);
	hfile.touch_group_recursive(name);
	hfile.save(mat, name, overwrite);
}

cv::Mat load(const std::string& filename, const std::string& name)
{
	hdf5file hfile(filename, true, false);
	return hfile.load(name);
}

}
