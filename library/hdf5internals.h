#ifndef CVIO_HDF5INTERNALS
#define CVIO_HDF5INTERNALS

#include <vector>
#include <string>

#include <hdf5.h>
#include <stdexcept>

namespace cv {
class Mat;
}

namespace cvio
{

class hdf5file
{
public:
	hdf5file(const std::string& filename, bool open = true, bool create = true);
	~hdf5file();

	void touch_group(const std::string& name);
	void touch_group_recursive(const std::string& name);

	hid_t handle() const;
	void save(const cv::Mat& dataset, const std::string& name, bool overwrite);
	cv::Mat load(const std::string& name);
	std::string filename() const;
	std::vector<std::string> subgroups(const std::string& path);
	std::vector<std::string> datasets(const std::string& path);
	std::vector<std::string> subelements(const std::string& path, int desired_type);

private:
	std::string _name;
	hid_t _file_id;
};

class hdf5dataset
{
public:
	hdf5dataset(hdf5file& hfile, const std::string& pname);
	hdf5dataset(hdf5file& hfile, const std::string& pname, hid_t type_id, hid_t space_id, bool overwrite);

	void read(void* data);
	void write(void* data);

	hid_t type() const;
	hid_t handle() const;

	~hdf5dataset();

private:
	hid_t _dataset_id, _type_id;
	const hdf5file& _hfile;
	std::string _name;
	bool _own_type_id;
};

namespace hdf5attribute
{

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
	template<> inline hid_t type_to_type_id<std::string>() { return H5T_C_S1; }

	template<typename T>
	hid_t val_to_type_id(const T&)
	{
		return H5Tcopy(type_to_type_id<T>());
	}

	template<>
	hid_t val_to_type_id(const std::string& val);

	void compare_and_read_scalar_attribute_internal(hid_t attribute_id, hid_t type_id, hid_t expected_type, void* buffer);

	template<typename T>
	T compare_and_read_scalar_attribute(hid_t attribute_id, hid_t type_id)
	{
		hid_t expected_type = type_to_type_id<T>();
		T data;
		compare_and_read_scalar_attribute_internal(attribute_id, type_id, expected_type, &data);
		return data;
	}

	template<>
	std::string compare_and_read_scalar_attribute(hid_t attribute_id, hid_t type_id);

	template<typename T>
	T read(hdf5dataset& hdataset, const std::string& name)
	{
		hid_t attribute_id = H5Aopen(hdataset.handle(), name.c_str(), H5P_DEFAULT);
		if(attribute_id < 0)
			throw std::runtime_error("Failed to open attribute (" + name + ") in dataset (" + "not available" + ") file (" + "not available" + ")");

		hid_t type_id = H5Aget_type(attribute_id);

		T result = compare_and_read_scalar_attribute<T>(attribute_id, type_id);

		H5Aclose(attribute_id);
		H5Tclose(type_id);
		return result;
	}

	template<typename T>
	const T* get_buffer(const T& val) { return &val; }
	const char* get_buffer(const std::string& val);

	void write_internal(hdf5dataset& hdataset, const std::string& name, hid_t type_id, const void* buffer, bool overwrite);

	template<typename T>
	void write(hdf5dataset& hdataset, const std::string& name, T value, bool overwrite = false)
	{
		hid_t type_id = val_to_type_id(value);
		const void* buffer = get_buffer(value);

		write_internal(hdataset, name, type_id, buffer, overwrite);
	}
}


int get_ocvtype(hid_t d);
hid_t get_type(int d);

}



#endif // HDF5N

