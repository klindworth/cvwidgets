#include "hdf5internals.h"

#include <hdf5.h>
#include <opencv2/core/core.hpp>
#include <stdexcept>
#include <iostream>
#include <iterator>

#include <boost/tokenizer.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>

namespace cvio
{

namespace hdf5attribute_impl
{
	std::string type_to_string(hid_t d)
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

	void write_internal(hdf5dataset& hdataset, const std::string& name, hid_t type_id, const void* buffer, bool overwrite)
	{
		bool exists = H5Aexists(hdataset.handle(), name.c_str());
		hid_t attribute_id = -1;
		if(exists && overwrite)
		{
			attribute_id = H5Aopen(hdataset.handle(), name.c_str(), H5P_DEFAULT);
			//H5Ldelete(_hfile.handle(), name.c_str(), H5P_DEFAULT);
		}
		else if(exists && !overwrite)
			throw std::runtime_error("Attribute already exists: (" + name + ")");
		else
		{
			hsize_t dims = 1;
			hid_t space_id = H5Screate_simple(1, &dims, NULL);

			attribute_id = H5Acreate(hdataset.handle(), name.c_str(), type_id, space_id, H5P_DEFAULT, H5P_DEFAULT);
			H5Sclose(space_id);
		}

		if(attribute_id < 0)
			throw std::runtime_error("Error creating or opening attribute () in file (" + name + ")");

		H5Awrite(attribute_id, type_id, buffer);
		H5Tclose(type_id);
		H5Aclose(attribute_id);
	}

	const char* get_buffer(const std::string& val) { return val.c_str(); }

	void compare_and_read_scalar_attribute_internal(hid_t attribute_id, hid_t type_id, hid_t expected_type, void* buffer)
	{
		if(H5Tequal(type_id, expected_type))
		{
			herr_t status = H5Aread(attribute_id, type_id, buffer);
			if(status >= 0)
				return;
		}
		throw std::runtime_error("type mismatch");
	}

	template<>
	std::string compare_and_read_scalar_attribute(hid_t attribute_id, hid_t type_id)
	{
		if(H5Tget_class(type_id) == H5T_STRING)
		{
			int size = H5Tget_size(type_id);
			std::string data(size+1, ' ');
			herr_t status = H5Aread(attribute_id, type_id, &(data[0]));
			if(status >= 0)
			{
				data.resize(size);
				return data;
			}
		}
		throw std::runtime_error("type mismatch");
	}

	template<>
	hid_t val_to_type_id(const std::string& val)
	{
		hid_t type_id = H5Tcopy(H5T_C_S1);
		H5Tset_size(type_id, val.size());
		return type_id;
	}

	std::vector<std::string> list_attributes(hdf5dataset& hdataset)
	{
		H5O_info_t info;
		H5Oget_info(hdataset.handle(), &info);
		std::size_t num_attr = info.num_attrs;

		std::vector<std::string> result(num_attr);

		const int MAX_LEN = 256;
		char attr_name[MAX_LEN];

		for(std::size_t i = 0; i < num_attr; ++i)
		{
			hid_t attribute_id = H5Aopen_idx(hdataset.handle(), i);
			H5Aget_name(attribute_id, MAX_LEN, attr_name);
			result[i] = std::string(attr_name);
			H5Aclose(attribute_id);
		}

		return result;
	}
}

hdf5attribute::hdf5attribute(hdf5dataset& hdataset, const std::string& name)
{
	_attribute_id = H5Aopen(hdataset.handle(), name.c_str(), H5P_DEFAULT);
	if(_attribute_id < 0)
		throw std::runtime_error("Failed to open attribute (" + name + ") in dataset (" + "not available" + ") file (" + "not available" + ")");

	_type_id = H5Aget_type(_attribute_id);
}

hdf5attribute::~hdf5attribute()
{
	if(_attribute_id >= 0)
		H5Aclose(_attribute_id);
	if(_type_id >= 0)
		H5Tclose(_type_id);
}


std::string hdf5attribute::read_as_string()
{
	std::string result;
	if(H5Tequal(_type_id, H5T_NATIVE_INT16))
		result = boost::lexical_cast<std::string>(read<short>());
	else if(H5Tequal(_type_id, H5T_NATIVE_INT32))
		result = boost::lexical_cast<std::string>(read<int>());
	else if(H5Tequal(_type_id, H5T_NATIVE_UINT32))
		result = boost::lexical_cast<std::string>(read<unsigned int>());
	else if(H5Tequal(_type_id, H5T_NATIVE_UINT16))
		result = boost::lexical_cast<std::string>(read<unsigned short>());
	else if(H5Tequal(_type_id, H5T_NATIVE_UINT8))
		result = boost::lexical_cast<std::string>(read<unsigned char>());
	else if(H5Tequal(_type_id, H5T_NATIVE_INT8))
		result = boost::lexical_cast<std::string>(read<char>());
	else if(H5Tequal(_type_id, H5T_NATIVE_FLOAT))
		result = boost::lexical_cast<std::string>(read<float>());
	else if(H5Tequal(_type_id, H5T_NATIVE_DOUBLE))
		result = boost::lexical_cast<std::string>(read<double>());
	else if(H5Tget_class(_type_id) == H5T_STRING)
		result = read<std::string>();
	else
		throw std::runtime_error("unknown type");
	return result;
}

hdf5dataset::hdf5dataset(hdf5file& hfile, const std::string& pname) : _hfile(hfile), _name(pname)
{
	_dataset_id = H5Dopen(_hfile.handle(), _name.c_str(), H5P_DEFAULT);
	if(_dataset_id < 0)
		throw std::runtime_error("Failed to open dataset (" + _hfile.filename() + ") in file (" + _name + ")");

	_type_id = H5Dget_type(_dataset_id);
	_own_type_id = true;
}

hdf5dataset::hdf5dataset(hdf5file &hfile, const std::string &name, hid_t type_id, hid_t space_id, bool overwrite): _type_id(type_id), _hfile(hfile), _name(name)
{
	bool exists = H5Lexists(_hfile.handle(), name.c_str(), H5P_DEFAULT);
	if(exists && overwrite)
		H5Ldelete(_hfile.handle(), name.c_str(), H5P_DEFAULT);
	else if(exists && !overwrite)
		throw std::runtime_error("Dataset already exists (" + _hfile.filename() + ") in file (" + name + ")");

	_dataset_id = H5Dcreate(hfile.handle(), name.c_str(), type_id, space_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	_own_type_id = false;

	if(_dataset_id < 0)
		throw std::runtime_error("Error creating or opening dataset (" + _hfile.filename() + ") in file (" + name + ")");

}

void hdf5dataset::read(void* data)
{
	herr_t status = H5Dread(_dataset_id, _type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
	if(status < 0)
		throw std::runtime_error("Failed to read dataset (" + _hfile.filename() + ") in file (" + _name + ")");
}

void hdf5dataset::write(void* data)
{
	herr_t status = H5Dwrite(_dataset_id, _type_id, H5S_ALL, H5S_ALL, H5P_DEFAULT, data );
	if(status < 0)
		throw std::runtime_error("Error writing to dataset (" + _hfile.filename() + ") in file (" + _name + ")");
}

hid_t hdf5dataset::type() const
{
	return _type_id;
}

hid_t hdf5dataset::handle() const
{
	return _dataset_id;
}

hdf5dataset::~hdf5dataset()
{
	if(_dataset_id >= 0)
		H5Dclose(_dataset_id);
	if(_own_type_id && _type_id >= 0)
		H5Tclose(_type_id);
}


hdf5file::hdf5file(const std::string& pfilename, bool open, bool create) : _name(pfilename)
{
	bool existing = boost::filesystem::exists(pfilename);
	_file_id = -1;
	if(open && existing)
		_file_id = H5Fopen(pfilename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
	if(_file_id < 0 && create) //if it doesn't already exist, try to create the file
		_file_id = H5Fcreate(pfilename.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
	if(_file_id < 0)
		throw std::runtime_error("Failed to create/open hdf5 file: " + pfilename);
}

hdf5file::~hdf5file() {
	if(_file_id > 0)
		H5Fclose(_file_id);
}

hid_t hdf5file::handle() const {
	return _file_id;
}

std::string hdf5file::filename() const {
	return _name;
}

void hdf5file::save(const cv::Mat& mat, const std::string& name, bool overwrite)
{
	int rank = mat.dims;
	std::vector<hsize_t> dims(mat.channels() == 1 ? rank : rank + 1);
	std::copy(mat.size.p, mat.size.p + rank, dims.begin());

	if(mat.channels() != 1)
		dims[rank++] = mat.channels();

	hid_t space_id = H5Screate_simple(rank, &(dims[0]), NULL);
	hid_t type_id = get_type(mat.depth());

	hdf5dataset hdataset(*this, name, type_id, space_id, overwrite);

	hdataset.write(mat.data);

	//TODO: if an exception is thrown, the space will not closed
	H5Sclose(space_id);
}

cv::Mat hdf5file::load(const std::string& name)
{
	hdf5dataset hdataset(*this, name);
	hid_t dataset_id = hdataset.handle();

	hid_t space_id = H5Dget_space(dataset_id);
	hid_t type_id = hdataset.type();
	//hid_t type_id = H5Tget_native_type(type_id_nnative, H5T_DIR_DEFAULT);

	hsize_t rank = H5Sget_simple_extent_ndims(space_id);

	std::vector<hsize_t> dims_out(rank);
	H5Sget_simple_extent_dims(space_id, &(dims_out[0]), NULL);

	//due to different types, the dimension sizes are copied in a vector of different type
	std::vector<int> dims_out_i(rank);
	std::copy(dims_out.begin(), dims_out.end(), dims_out_i.begin());

	int channels = 1;

	if(rank == 3 && dims_out[rank-1] <= 4)
	{
		channels = dims_out[rank-1];
		--rank;
	}

	cv::Mat dataset = cv::Mat(rank, &(dims_out_i[0]), CV_MAKETYPE(get_ocvtype(type_id), channels));

	hdataset.read(dataset.data);

	H5Sclose(space_id);

	return dataset;
}

std::vector<std::string> hdf5file::subelements(const std::string& path, int desired_type)
{
	hid_t group_id = H5Gopen(_file_id, path.c_str(), H5P_DEFAULT);

	const size_t MAX_NAME = 256;
	char element_name[MAX_NAME];

	hsize_t object_count;
	herr_t err = H5Gget_num_objs(group_id, &object_count);

	std::vector<std::string> result;

	for(std::size_t i = 0; i < object_count; ++i)
	{

		int object_type = H5Gget_objtype_by_idx(group_id, i);
		H5Gget_objname_by_idx(group_id, i, element_name, MAX_NAME);

		if(object_type == desired_type)
			result.push_back(std::string(element_name));
	}

	return result;
}

std::vector<std::string> hdf5file::subgroups(const std::string& path)
{
	return subelements(path, H5G_GROUP);
}

std::vector<std::string> hdf5file::datasets(const std::string &path)
{
	return subelements(path, H5G_DATASET);
}

int get_ocvtype(hid_t d)
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

hid_t get_type(int d)
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

void hdf5file::touch_group(const std::string& name)
{
	//std::cout << "touch: " << name << std::endl;
	hid_t location_id = handle();
	hid_t result = -1;
	if(! H5Lexists(location_id, name.c_str(), H5P_DEFAULT))
		result = H5Gcreate(location_id, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
	else
		result = H5Gopen(location_id, name.c_str(), H5P_DEFAULT);
	H5Gclose(result);
}

void hdf5file::touch_group_recursive(const std::string& name)
{
	std::string::size_type pos = name.find_last_of('/');
	std::string path;
	if(pos != std::string::npos)
		path = name.substr(0, pos);
	else
		return;

	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	boost::char_separator<char> sep("/");
	tokenizer tokens(path, sep);

	std::string fullpath = "";
	for (tokenizer::iterator tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
	{
		//std::cout << "<" << *tok_iter << "> ";
		fullpath += "/" + *tok_iter;
		//std::cout << fullpath << std::endl;
		touch_group(fullpath);
	}
}

}
