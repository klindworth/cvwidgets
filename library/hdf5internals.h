#ifndef CVIO_HDF5INTERNALS
#define CVIO_HDF5INTERNALS

#include <vector>
#include <string>

#include <hdf5.h>
#include <stdexcept>

#include "h5types.h"

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
	std::string name() const { return _name; }

	hid_t type() const;
	hid_t handle() const;

	~hdf5dataset();

private:
	hid_t _dataset_id, _type_id;
	const hdf5file& _hfile;
	std::string _name;
	bool _own_type_id;
};

namespace hdf5attribute_impl
{

	void compare_and_read_scalar_attribute_internal(hid_t attribute_id, hid_t type_id, hid_t expected_type, void* buffer);

	template<typename T>
	T compare_and_read_scalar_attribute(hid_t attribute_id, hid_t type_id)
	{
		hid_t expected_type = h5types::type_to_type_id<T>();
		T data;
		compare_and_read_scalar_attribute_internal(attribute_id, type_id, expected_type, &data);
		return data;
	}

	template<>
	std::string compare_and_read_scalar_attribute(hid_t attribute_id, hid_t type_id);

	void write_internal(hdf5dataset& hdataset, const std::string& name, hid_t type_id, const void* buffer, bool overwrite);

	template<typename T>
	void write(hdf5dataset& hdataset, const std::string& name, T value, bool overwrite = false)
	{
		hid_t type_id = h5types::value_to_type_id(value);
		const void* buffer = h5types::get_buffer(value);

		write_internal(hdataset, name, type_id, buffer, overwrite);
	}

	std::vector<std::string> list_attributes(hdf5dataset& hdataset);
}

class hdf5attribute
{
public:
	hdf5attribute(hdf5dataset& hdataset, const std::string& name);
	std::string read_as_string();
	~hdf5attribute();

	hid_t handle() const { return _attribute_id; }
	hid_t type() const { return _type_id; }
	std::string type_as_string() const { return h5types::typeid_to_string(type()); }

	template<typename T>
	T read()
	{
		return hdf5attribute_impl::compare_and_read_scalar_attribute<T>(_attribute_id, _type_id);
	}

private:
	hid_t _attribute_id;
	hid_t _type_id;
};

namespace hdf5attribute_impl {
	template<typename T>
	T read(hdf5dataset& hdataset, const std::string& name)
	{
		hdf5attribute attribute(hdataset, name);
		return attribute.read<T>();
	}
}

}



#endif // HDF5N

