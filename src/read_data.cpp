#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int	read_data(const char* data, std::vector<double>& x, std::vector<double>& y)
{
	std::ifstream file(data);
	if (!file.is_open())
	{
		std::cerr << "ERROR: can't open file" << std::endl;
		return 1;
	}

	std::string info;
	std::getline(file, info);
	if (info != "km,price")
	{
		std::cerr << "ERROR: wrong first line" << std::endl;
		file.close();
	}

	double tmp_x, tmp_y;
	char c;
	while(std::getline(file, info))
	{
		std::istringstream iss(info);
		if (iss >> tmp_x >> c >> tmp_y)
		{
			if (c != ',' || (iss.get(c) && !iss.eof()))
			{
				std::cerr << "ERROR: wrong data in model (must be only 2 double and comma) in each line: " << std::endl;
				file.close();
				return 1;
			}
			x.push_back(tmp_x);
			y.push_back(tmp_y);
		}
		else
		{
			std::cerr << "ERROR: wrong format data in model (there are no 2 double)" << std::endl;
			file.close();
			return 1;
		}
	}

	file.close();
	return 0;
}