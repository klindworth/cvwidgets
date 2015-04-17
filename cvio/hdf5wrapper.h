#ifndef HDF5WRAPPER_H
#define HDF5WRAPPER_H

#include <string>

namespace cv {
class Mat;
}

namespace cvio
{
	/**
	 * @brief save Saves a OpenCV matrix to a HDF5 file
	 * @param dataset Matrix, that will be saved
	 * @param filename Name/Path of the HDF5 file
	 * @param name Name/Path of the dataset/matrix within the HDF5 file
	 * @param overwrite If true, a already existing HDF5 Dataset under name path will be silently overwritten
	 */
	void save(const cv::Mat& dataset, const std::string& filename, const std::string& name, bool overwrite = true);

	/**
	 * @brief load Loads a OpenCV matrix from a HDF5 file
	 * @param filename Name/Path of the HDF5 file
	 * @param name Name/Path of the dataset within the HDF5 file
	 * @return Loaded matrix
	 */
	cv::Mat load(const std::string& filename, const std::string& name);
}

#endif // HDF5WRAPPER_H

