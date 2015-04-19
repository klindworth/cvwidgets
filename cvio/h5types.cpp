#include "h5types.h"

#include <opencv2/core/core.hpp>

namespace cvio
{
namespace h5types {

/**
 * @brief typeid_to_ocv Converts a HDF5 typeid to a OpenCV type
 * @param d HDF5 typeid
 * @return OpenCV type
 */
int typeid_to_ocv(hid_t d)
{
	if(H5Tequal(d, H5T_NATIVE_FLOAT))
		return CV_32F;
	else if(H5Tequal(d, H5T_NATIVE_DOUBLE))
		return CV_64F;
	else if(H5Tequal(d, H5T_NATIVE_INT32))
		return CV_32S;
	else if(H5Tequal(d, H5T_NATIVE_INT16))
		return CV_16S;
	else if(H5Tequal(d, H5T_NATIVE_UINT16))
		return CV_16U;
	else if(H5Tequal(d, H5T_NATIVE_UCHAR))
		return CV_8U;
	else if(H5Tequal(d, H5T_NATIVE_CHAR))
		return CV_8S;
	else
		throw std::runtime_error("unsupported type");
}

/**
 * @brief ocv_to_typeid Converts an OpenCV type to a HDF5 type
 * @param d OpenCV type
 * @return HDF5 typeid
 */
hid_t ocv_to_typeid(int d)
{
	//int d = dataset.depth();
	if(d == CV_32F)
		return H5T_NATIVE_FLOAT;
	else if(d == CV_64F)
		return H5T_NATIVE_DOUBLE;
	else if(d == CV_32S)
		return H5T_NATIVE_INT32;
	else if(d == CV_16S)
		return H5T_NATIVE_INT16;
	else if(d == CV_16U)
		return H5T_NATIVE_UINT16;
	else if(d == CV_8U)
		return H5T_NATIVE_UCHAR;
	else if(d == CV_8S)
		return H5T_NATIVE_CHAR;
	else
		throw std::runtime_error("unsupported type");
}

template<>
hid_t value_to_type_id(const std::string& val)
{
	hid_t type_id = H5Tcopy(H5T_C_S1);
	H5Tset_size(type_id, val.size());
	return type_id;
}

/**
 * @brief typeid_to_string Converts a HDF5 typeid to a string
 * @param d HDF5 typeid
 * @return String with the C++ name of type
 */
std::string typeid_to_string(hid_t d)
{
	if(H5Tequal(d, H5T_NATIVE_FLOAT))
		return "float";
	else if(H5Tequal(d, H5T_NATIVE_DOUBLE))
		return "double";
	else if(H5Tequal(d, H5T_NATIVE_INT32))
		return "int";
	else if(H5Tequal(d, H5T_NATIVE_INT16))
		return "short";
	else if(H5Tequal(d, H5T_NATIVE_UINT16))
		return "unsigned short";
	else if(H5Tequal(d, H5T_NATIVE_UCHAR))
		return "unsigned char";
	else if(H5Tequal(d, H5T_NATIVE_CHAR))
		return "char";
	else if(H5Tget_class(d) == H5T_STRING)
		return "string";
	else
		throw std::runtime_error("unsupported type");
}

const char* get_buffer(const std::string& val) { return val.c_str(); }
}
}
