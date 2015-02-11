#ifndef H5TYPES_H
#define H5TYPES_H

#include <hdf5.h>
#include <string>
#include <stdexcept>

namespace cvio {
namespace h5types {
	template<typename T>
	hid_t inline type_to_type_id()
	{
		throw std::runtime_error("unknown type");
	}

	template<> inline hid_t type_to_type_id<short>() { return H5T_NATIVE_INT16; }
	template<> inline hid_t type_to_type_id<int>() { return H5T_NATIVE_INT32; }
	template<> inline hid_t type_to_type_id<unsigned int>() { return H5T_NATIVE_UINT32; }
	template<> inline hid_t type_to_type_id<unsigned short>() { return H5T_NATIVE_UINT16; }
	template<> inline hid_t type_to_type_id<unsigned char>() { return H5T_NATIVE_UINT8; }
	template<> inline hid_t type_to_type_id<float>() { return H5T_NATIVE_FLOAT; }
	template<> inline hid_t type_to_type_id<double>() { return H5T_NATIVE_DOUBLE; }
	template<> inline hid_t type_to_type_id<std::string>() { return H5T_C_S1; }

	template<typename T>
	hid_t value_to_type_id(const T&)
	{
		return H5Tcopy(type_to_type_id<T>());
	}

	template<>
	hid_t value_to_type_id(const std::string& val);

	int typeid_to_ocv(hid_t d);
	hid_t ocv_to_typeid(int d);

	template<typename T>
	const T* get_buffer(const T& val) { return &val; }
	const char* get_buffer(const std::string& val);

	std::string typeid_to_string(hid_t d);
}
}
#endif
