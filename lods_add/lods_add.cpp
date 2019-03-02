#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>
#include <iterator>
#include <windows.h>

#define WIN32_LEAN_AND_MEAN

// -------------------------------- //

class Locale
{
	UINT cp;
	UINT output_cp;
public:
	Locale(const int& l);
	~Locale();
};

Locale::Locale(const int& l)
{
	cp = GetConsoleCP();
	output_cp = GetConsoleOutputCP();
	SetConsoleCP(l);
	SetConsoleOutputCP(l);
}

Locale::~Locale()
{
	SetConsoleCP(cp);
	SetConsoleOutputCP(output_cp);
}

// ------------------------------- //
// split line
template<char del>
class line_split : public std::string
{
public:
	template<char d>
	friend std::istream& operator>>(std::istream& is, line_split& output);
};

template<char d>
std::istream & operator>>(std::istream & is, line_split<d> & output)
{
	std::getline(is, output, d);
	return is;
}

// ----------------------------------------------- //
struct matrix4;

struct vertex4
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1; // �� ������������, ��� �������

	vertex4 operator*=(matrix4& m); // �������� ������� �� �������
	bool operator==(const int& num)const; // ��������� � ������
};

struct matrix4
{
	vertex4 matrix[4]; //  ������� 4 x 4

	void createIdentity(); // ������� ���������
	void createTranslation(float x, float y, float z); // ������� �����������
	const vertex4& operator[](int ind); // ������ � ������ �������

};

// ------------------------------------------ //

void matrix4::createIdentity()
{
	matrix[0].x = matrix[1].y = matrix[2].z = matrix[3].w = 1;
}

void matrix4::createTranslation(float x, float y, float z)
{
	this->createIdentity();
	matrix[3].x = x;
	matrix[3].y = y;
	matrix[3].z = z;
	matrix[3].w = 1;
}

const vertex4 & matrix4::operator[](int ind)
{
	return this->matrix[ind]; // �������� ��� !!!(�� �������� ������), TODO
}

// ----------------------------------------------- //

vertex4 vertex4::operator*=(matrix4 & m)
{
	vertex4 out;
	out.x = this->x * m[0].x +this->y * m[1].x + this->z * m[2].x + this->w * m[3].x;
	out.y = this->x * m[0].y + this->y * m[1].y + this->z * m[2].y + this->w * m[3].y;
	out.z = this->x * m[0].z + this->y * m[1].z + this->z * m[2].z + this->w * m[3].z;
	out.w = this->x * m[0].w + this->y * m[1].w + this->z * m[2].w + this->w * m[3].w;
	return (*this = out);
}

bool vertex4::operator==(const int & num) const
{
	return this->x == num && this->y == num && this->z == num; //  w �������������
}

// ---------------------------------------- //

struct section
{
	std::string section_number;
	std::string clsid;
	std::string co_flags;
	std::string flags;
	std::string name;
	std::string position;
	std::string reference_name;
	std::string rotation;
	std::string scale;
	std::string version;
};

// --------------------------------------------- //

class scene_object_out // �������� ������
{
protected:
	int count_object; // ���������� ��������
	std::string name_file; // ��� �����
	std::list<section> sect; // ������ ������
	std::vector<std::string> temp_data; // ��������� ������
public:
	scene_object_out(const std::string& n) : name_file(n) {}
	bool decompile(); // ������ ����
	int get_count() { return this->count_object; }; // �������� ���������� ������
	scene_object_out& operator+=(const scene_object_out& soo); // ��������� � ����� ������ ����
	bool write_new_file(const std::string& out_path); // ������������� ����� ����
};

bool scene_object_out::decompile()
{
	std::ifstream file(name_file);
	if (!file)
	{
		std::cout << "Error open file " << name_file << std::endl;
		return false;
	}
	std::string tmp;
	while (true) // ���������� �����
	{
		std::getline(file, tmp, '\n');
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		if (tmp.find("[object_") != -1) // ����� ������ ������ - ������������� �������� ��������
			break;
		temp_data.push_back(tmp);
		if (tmp.find("objects_count") != -1) // ��������� ���������� ��������
		{
			tmp.erase(0, tmp.find_last_of(' ') + 1);
			this->count_object = atoi(tmp.c_str());
		}
	}
	while (true) // ������ ��� ������ 
	{
		section sc;
		tmp.resize(tmp.length() - 1);
		tmp.erase(0, 8);
		sc.section_number = tmp;
		std::getline(file, sc.clsid, '\n'); // clsid
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		std::getline(file, sc.co_flags, '\n'); // co_flags
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		std::getline(file, sc.flags, '\n'); // flags
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		std::getline(file, sc.name, '\n'); // name
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		std::getline(file, sc.position, '\n'); // position
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		std::getline(file, sc.reference_name, '\n'); // reference_name
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		std::getline(file, sc.rotation, '\n'); // rotation
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		std::getline(file, sc.scale, '\n'); // scale
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		std::getline(file, sc.version, '\n'); // version
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		sect.push_back(sc); // ��������� ������
		std::getline(file, tmp, '\n'); // ���������� ������

		std::getline(file, tmp, '\n'); // section number
		if (file.eof())
			break;
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
	}
	file.close();
	return true;
}

scene_object_out & scene_object_out::operator+=(const scene_object_out & soo)
{
	this->count_object += soo.count_object;
	this->sect.insert(this->sect.end(), soo.sect.begin(), soo.sect.end());
	return *this;
}

bool scene_object_out::write_new_file(const std::string & out_path)
{
	std::ofstream file(out_path);
	file.imbue(std::locale("rus_rus.1251"));
	if (!file)
	{
		std::cout << "Error create file " << out_path << std::endl;
		return false;
	}
	for (auto& data : this->temp_data)
	{
		if (data.find("objects_count") != -1) // �������� ���������� ��������
		{
			data.erase(data.find_last_of(' ') + 1);
			data += std::to_string(this->count_object);
		}
		file << data << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
	}

	int y(0);
	for(auto& sc: this->sect) // ���������� ��� ������ 
	{
		file << "[object_" << sc.section_number << "]" << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file  << sc.clsid << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file << sc.co_flags << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file << sc.flags << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file << sc.name << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file << sc.position << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file << sc.reference_name << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file << sc.rotation << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file << sc.scale << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}
		file << sc.version << std::endl << std::endl;
		if (!file)
		{
			std::cout << "Error write file " << out_path << std::endl;
			return false;
		}

	}
	file.close();
	return true;
}

// ---------------------------------------- //

class scene_object_in: public scene_object_out// ������� ������
{
	std::vector<std::string> folders; // �����, ��� ������������ � ����������� ������
	vertex4 vert; // �������, ���� ����� ����������
	bool status; // ������ ������������� �������
	bool calculate; // ����� �� ���������� ������
public:
	scene_object_in(const std::string& n, char** argv, int argc);
	void shift_section_number(int count); // �������� ��� ������ �� �������� ����������(���������� ������ ��� ���������)
	void clear_unnecessary_section(); // ������� �������� ������
	void translation_objects(); // ���������� ��� �������
	bool isCalculate()const { return this->calculate; } // ������ ����������� ��������
	bool isStatus()const { return this->status; } // ������ ������������� ������
	void print_section_references_name(); // �������� ������ ������ �� �������
};

scene_object_in::scene_object_in(const std::string & n, char** argv, int argc):scene_object_out(n)
{
	// ������ ������
	std::ifstream config("path.txt");
	if (config)
	{
		std::string tmp;
		while (true)
		{
			std::getline(config, tmp, '\n');
			if (!config)
			{
				std::cout << "Error read path.txt." << std::endl;
				status = false;
				return;
			}
			folders.push_back(tmp);
			if (config.eof())
				break;
		}
	}
	else
	{
		std::cout << "Error open path.txt." << std::endl;
		status = false;
		return;
	}

	// ����������� ��������� cmd
	for (int i(1); i < argc; i++)
	{
		if (!strcmp(argv[i], "-translation")) // ���� ��������� ����(����������� ��������)
		{
			// ���������, ���� �� ����������
			if (i + 3 >= argc)
			{
				std::cout << "����������� ��� ��������� � ����� -translation. ��������� ����������." << std::endl;
				status = false;
				return;
			}
			// ���������� ��� ��� ����������
			std::stringstream stream;
			stream << argv[++i];
			stream >> this->vert.x;
			if (!stream)
			{
				std::cout << "�������� �������� � 1 � ����� -translation. ��������� ����������." << std::endl;
				status = false;
				return;
			}

			stream.clear();
			stream << argv[++i];
			stream >> this->vert.y;
			if (!stream)
			{
				std::cout << "�������� �������� � 2 � ����� -translation. ��������� ����������." << std::endl;
				status = false;
				return;
			}

			stream.clear();
			stream << argv[++i];
			stream >> this->vert.z;
			if (!stream)
			{
				std::cout << "�������� �������� � 3 � ����� -translation. ��������� ����������." << std::endl;
				status = false;
				return;
			}
		}
	}

	// ����������� �� �������, ���� �����
	if (this->vert == 0) // ������� == 0. ������� ����������� �� �����
	{
		calculate = false;
		return;
	}
	calculate = true;
}

void scene_object_in::shift_section_number(int count)
{
	int i(0);
	for (auto& elem : this->sect)
		elem.section_number = std::to_string(i++ + count);
}

void scene_object_in::clear_unnecessary_section()
{
	std::vector<std::string>* f = &this->folders;
	int *count = &this->count_object;
	this->sect.erase(std::remove_if(this->sect.begin(), this->sect.end(), [f, count](const section& sc) {
		for (auto& elem : *f)
		{
			std::string tmp = sc.reference_name;
			tmp.erase(0, tmp.find("=") + 2);
			if (tmp.find(elem) == 0) // ���� � ���� ����������� ��� ������ ����� ������� - �� �� �������
				return false;
		}
		(*count)--;
		return true;
	}), this->sect.end());
}

void scene_object_in::translation_objects()
{
	// �������� ��� ������� �� ������� �����������
	for (auto& elem : this->sect)
	{
		vertex4 v;
		std::string tmp(elem.position); tmp.erase(0, tmp.find('=') + 1);
		std::istringstream iss(tmp);
		std::vector<std::string> results((std::istream_iterator<line_split<','>>(iss)),
			std::istream_iterator<line_split<','>>());
		float *coord[] = { &v.x, &v.y, &v.z };
		for(int i(0); i < 3; i++)
		{
			std::stringstream stream;
			stream << results[i]; stream >> **(coord + i);
		};
		matrix4 m; m.createTranslation(vert.x, vert.y, vert.z); // ������� ������� ����������� �� �������� �������
		v *= m; // ��������, ������ ���������� ���������� �� ������
		tmp = elem.position; tmp.erase(tmp.find('=') + 2);
		tmp += std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z);
		elem.position = tmp;
	}
}

void scene_object_in::print_section_references_name()
{
	std::cout << "List reference name:" << std::endl;
	std::for_each(this->sect.begin(), this->sect.end(),  [](const section& sc) {
		std::cout << sc.reference_name << std::endl;
	});
	std::cout << std::endl;
}

// ---------------------------------------------- //

int main(int argc, char* argv[])
{
	// ����������
	// ��������:
	// ��������� ��������� ������ �����, ������ �� ����� ������� � ������, ������� ������� �� �������� �������������

	// ���������:
	// -translation "��� float - ����� ����� ������",  ������� ����� ���������� ������. �������� �������, ���� ���������� ������� ������
	// �����:
	// levels\\in\\scene_object.part - ����, �� �������� ����� ������� ������
	// levels\\modify\\scene_object.part - ����, � �������� ����� ����������� ������
	// levels\\out\\scene_object.part - �������� ����, �������� ������ ���� �������

	// -translation -1922.477 -0.0 168.273(���������� z, ���� �������� ���������� � maya, ������� �������������)

	// ������������� ������
	Locale locale(1251);

	// ������ ����, �� �������� ����� ���������
	scene_object_in in("levels\\in\\scene_object.part", argv, argc);
	if (in.isStatus())
		return 0;
	if (!in.decompile())
		return 0;

	// ������ ����, � ������� ����� ���������
	scene_object_out out("levels\\modify\\scene_object.part");
	if (!out.decompile())
		return 0;

	// ������� �������� ������ � �������� ���������� ������ � �����
	//in.clear_unnecessary_section();
	//in.print_section_references_name();

	// �������� ������ �����, �� �������� ����� ���������
	in.shift_section_number(out.get_count());

	// ��������� ����������� ��������, ���� ����
	if (in.isCalculate())
		in.translation_objects();

	// ��������� ����������� ���� � ��������� �����
	out += in;

	// ���������� ���������� ���� � ����� �����
	if (!out.write_new_file("levels\\out\\scene_object.part"))
		return 0;

	return 0;
}

