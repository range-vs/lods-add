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
	float w = 1; // не используется, для матрицы

	vertex4 operator*=(matrix4& m); // умножаем вершину на матрицу
	bool operator==(const int& num)const; // сравнение с числом
};

struct matrix4
{
	vertex4 matrix[4]; //  матрица 4 x 4

	void createIdentity(); // создаем единичную
	void createTranslation(float x, float y, float z); // создаем перемещения
	const vertex4& operator[](int ind); // доступ к строке матрицы

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
	return this->matrix[ind]; // проверки нет !!!(не неверный индекс), TODO
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
	return this->x == num && this->y == num && this->z == num; //  w отбрасывается
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

class scene_object_out // выходной объект
{
protected:
	int count_object; // количество объектов
	std::string name_file; // имя файла
	std::list<section> sect; // массив секций
	std::vector<std::string> temp_data; // остальные данные
public:
	scene_object_out(const std::string& n) : name_file(n) {}
	bool decompile(); // грузим файл
	int get_count() { return this->count_object; }; // получаем количество секций
	scene_object_out& operator+=(const scene_object_out& soo); // добавляем в конец другой файл
	bool write_new_file(const std::string& out_path); // перезаписывае новый файл
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
	while (true) // пропускаем шапку
	{
		std::getline(file, tmp, '\n');
		if (!file)
		{
			std::cout << "Error read file " << name_file << std::endl;
			return false;
		}
		if (tmp.find("[object_") != -1) // нашли первую секцию - останавливаем проверку объектов
			break;
		temp_data.push_back(tmp);
		if (tmp.find("objects_count") != -1) // добавляем количество объектов
		{
			tmp.erase(0, tmp.find_last_of(' ') + 1);
			this->count_object = atoi(tmp.c_str());
		}
	}
	while (true) // грузим все секции 
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
		sect.push_back(sc); // добавляем секцию
		std::getline(file, tmp, '\n'); // пропускаем пробел

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
		if (data.find("objects_count") != -1) // изменяем количество объектов
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
	for(auto& sc: this->sect) // записываем все секции 
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

class scene_object_in: public scene_object_out// входной объект
{
	std::vector<std::string> folders; // папки, для соответствия и копирования секций
	vertex4 vert; // позиция, куда нужно перемещать
	bool status; // статус инициализации объекта
	bool calculate; // нужно ли перемещать секции
public:
	scene_object_in(const std::string& n, char** argv, int argc);
	void shift_section_number(int count); // сдвигаем все секции на заданное количество(количество секций уже имеющихся)
	void clear_unnecessary_section(); // удаляем ненужные секции
	void translation_objects(); // перемещаем все объекты
	bool isCalculate()const { return this->calculate; } // статус калькуляции объектов
	bool isStatus()const { return this->status; } // статус инициализации класса
	void print_section_references_name(); // печатаем список ссылок на объекты
};

scene_object_in::scene_object_in(const std::string & n, char** argv, int argc):scene_object_out(n)
{
	// читаем конфиг
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

	// преобразуем аргументы cmd
	for (int i(1); i < argc; i++)
	{
		if (!strcmp(argv[i], "-translation")) // если аргументы ерен(перемещение объектов)
		{
			// проверяем, есть ли координаты
			if (i + 3 >= argc)
			{
				std::cout << "Отсутствуют все аргументы у ключа -translation. Аварийное завершение." << std::endl;
				status = false;
				return;
			}
			// запоминаем все три координаты
			std::stringstream stream;
			stream << argv[++i];
			stream >> this->vert.x;
			if (!stream)
			{
				std::cout << "Неверный аргумент № 1 у ключа -translation. Аварийное завершение." << std::endl;
				status = false;
				return;
			}

			stream.clear();
			stream << argv[++i];
			stream >> this->vert.y;
			if (!stream)
			{
				std::cout << "Неверный аргумент № 2 у ключа -translation. Аварийное завершение." << std::endl;
				status = false;
				return;
			}

			stream.clear();
			stream << argv[++i];
			stream >> this->vert.z;
			if (!stream)
			{
				std::cout << "Неверный аргумент № 3 у ключа -translation. Аварийное завершение." << std::endl;
				status = false;
				return;
			}
		}
	}

	// перемножаем на матрицу, если нужно
	if (this->vert == 0) // вершина == 0. матрица перемещения не нужна
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
			if (tmp.find(elem) == 0) // если в пути встречается имя нужной папки СНАЧАЛА - то не удаляем
				return false;
		}
		(*count)--;
		return true;
	}), this->sect.end());
}

void scene_object_in::translation_objects()
{
	// умножаем все позиции на матрицу перемещения
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
		matrix4 m; m.createTranslation(vert.x, vert.y, vert.z); // создаем матрицу перемещения на заданную позицию
		v *= m; // умножили, теперь возвращаем координаты по местам
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
	// информация
	// описание:
	// программы добавляет секции лодов, взятые из одной локации к другой, изменяя позицию на заданную пользователем

	// аргументы:
	// -translation "три float - числа через пробел",  дробная часть отделяется точкой. Означает позицию, куда перемещать входные секции
	// папки:
	// levels\\in\\scene_object.part - файл, из которого будут браться секции
	// levels\\modify\\scene_object.part - файл, к которому будут добавляться секции
	// levels\\out\\scene_object.part - выходной файл, хранящий секции двух локаций

	// -translation -1922.477 -0.0 168.273(координату z, если смотрели координаты в maya, следует инвертировать)

	// устанавливаем локаль
	Locale locale(1251);

	// грузим файл, из которого будем добавлять
	scene_object_in in("levels\\in\\scene_object.part", argv, argc);
	if (in.isStatus())
		return 0;
	if (!in.decompile())
		return 0;

	// грузим файл, в который будем добавлять
	scene_object_out out("levels\\modify\\scene_object.part");
	if (!out.decompile())
		return 0;

	// удаляем ненужные секции и печатаем оставшиеся секции в вывод
	//in.clear_unnecessary_section();
	//in.print_section_references_name();

	// сдвигаем секции файла, из которого будем добавлять
	in.shift_section_number(out.get_count());

	// выполняем калькуляцию объектов, если надо
	if (in.isCalculate())
		in.translation_objects();

	// добавляем выгруженный файл к исходному файлу
	out += in;

	// записываем полученный файл в новое место
	if (!out.write_new_file("levels\\out\\scene_object.part"))
		return 0;

	return 0;
}

