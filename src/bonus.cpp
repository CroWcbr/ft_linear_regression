#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <dirent.h>

int	read_data(const char* data, std::vector<double>& x, std::vector<double>& y);
int	solve_linear_regression(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b);

static const std::string plot_path = "./plot/";
static const std::string data_path = plot_path + "data.csv";
static const std::string linear_regression_path = plot_path + "model.csv";
static const std::string gnuplot_commands_path = plot_path + "gnuplot_commands.gp";

static int	check_dir()
{
	DIR* dir = opendir(plot_path.c_str());
	if (dir)
	{
		closedir(dir);
		std::string deleteCommand = "rm -r " + plot_path;
		int deleteResult = std::system(deleteCommand.c_str());
		if (deleteResult != 0)
		{
			std::cerr << "ERROR: can't delete folder" << std::endl;
			return 1;
		}
	}
	else if (ENOENT != errno)
	{
		std::cerr << "ERROR: Failed to open folder" << std::endl;
		return 1;
	}
	std::string createCommand = "mkdir " + plot_path;
	int createResult = std::system(createCommand.c_str());
	if (createResult != 0)
	{
		std::cerr << "ERROR: can't create folder" << std::endl;
		return 1;
	}
	return 0;
}

int plot(std::string output_path, bool is_linear_regression, double minX, double maxX, double minY, double maxY)
{
	std::ofstream gnuplotFile(gnuplot_commands_path);
	gnuplotFile << "set term png" << std::endl;
	gnuplotFile << "set output '" << plot_path << output_path << "'" << std::endl;
	gnuplotFile << "set xrange [" << minX << ":" << maxX << "]" << std::endl;
	gnuplotFile << "set yrange [" << minY << ":" << maxY << "]" << std::endl;
	gnuplotFile << "plot '" << data_path << "' with points";
	if (is_linear_regression)
		gnuplotFile << ", '" << linear_regression_path << "' with lines" << std::endl;
	else
		gnuplotFile << std::endl;
	gnuplotFile.close();

	std::string command = "gnuplot ./plot/gnuplot_commands.gp";
	std::system(command.c_str());
	return 0;
}

int main()
{
	const char*	data = "./data/data.csv";
	std::vector<double>	x, y;
	double a, b;

	if (read_data(data, x, y) || \
		check_dir())
		return -1;

	double minX = 0;
	double maxX = *std::max_element(x.begin(), x.end()) * 1.1;
	double minY = *std::min_element(y.begin(), y.end()) * 0.9;
	double maxY = *std::max_element(y.begin(), y.end()) * 1.1;

	std::ofstream file(data_path);
	if (!file.is_open())
	{
		std::cerr << "ERROR: can't open/create file" << std::endl;
		return -1;
	}
	std::vector<double>	x_tmp, y_tmp;
	for (size_t i = 0; i < x.size(); ++i)
	{
		file << x[i] << "\t" << y[i] << std::endl;
		x_tmp.push_back(x[i]);
		y_tmp.push_back(y[i]);
		solve_linear_regression(x_tmp, y_tmp, a, b);
		std::ofstream file2(linear_regression_path, std::ios::trunc);
		if (file2.is_open())
		{
			file2 << minX << "\t" << a * minX + b << std::endl;
			file2 << maxX << "\t" << a * maxX + b << std::endl;
			std::cout << i << "\t" << a << "\t" << b << std::endl;
			file2.close();
		}
		else
		{
			std::cerr << "ERROR: can't open/create file" << std::endl;
			return -1;
		}
		plot("input_data_" + std::to_string(i) + ".png", true, minX, maxX, minY, maxY);
	}
	file.close();

	plot("input_data.png", false, minX, maxX, minY, maxY);
	plot("input_data_regres_line.png", true, minX, maxX, minY, maxY);
	return 0;
}

