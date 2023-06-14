#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

static int	find_x_y(const char* data, std::vector<double>& x, std::vector<double>& y)
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

static int	solve_linear_regression(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b)
{
	if (x.size() != y.size() || x.empty())
	{
		std::cerr << "ERROR: wrong x, y" << std::endl;
		return 1;
	}

	double	sum_x, sum_y, sum_xy, sum_xx;
	sum_x = sum_y = sum_xy = sum_xx = 0;

	for (size_t i = 0; i < x.size(); ++i)
	{
		sum_x += x[i];
		sum_y += y[i];
		sum_xy += x[i] * y[i];
		sum_xx += x[i] * x[i];
	}

	double	n = static_cast<double>(x.size());
	double	mean_x = sum_x / n;
	double	mean_y = sum_y / n;

	a = (sum_xy - n * mean_x * mean_y) / (sum_xx - n * mean_x * mean_x);
	b = mean_y - a * mean_x;
	return 0;
}

static int	write_model(const char* model, const double& a, const double& b)
{
	std::ofstream file(model);

	if (file.is_open())
	{
		file << a << " " << b;
		file.close();
	}
	else
	{
		std::cerr << "ERROR: can't open/create file" << std::endl;
		return 1;
	}

	return 0;
}

int main()
{
	const char*	data = "./data/data.csv";
	const char* model = "./data/model.csv";
	std::vector<double>	x, y;
	double a, b;
	if (find_x_y(data, x, y) || \
		solve_linear_regression(x, y, a, b) || \
		write_model(model, a, b))
		return -1;
	return 0;
}
