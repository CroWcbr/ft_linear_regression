#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <dirent.h>
#include <cmath>

int	read_data(const char* data, std::vector<double>& x, std::vector<double>& y);
int	solve_linear_regression(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b);

static const std::string plot_path = "./plot/";
static const std::string data_path = plot_path + "data.csv";
static const std::string linear_regression_path = plot_path + "model.csv";
static const std::string gnuplot_commands_path = plot_path + "gnuplot_commands.gp";
static const std::string precision_path = plot_path + "precision.txt";

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

static int	plot(std::string output_path, bool is_linear_regression, double minX, double maxX, double minY, double maxY)
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

static double	mean_absolute_percentage_error(const std::vector<double>& actual, const std::vector<double>& predicted)
{
	if (predicted.size() != actual.size())
	{
		std::cerr << "ERROR: Size mismatch between predicted and actual values." << std::endl;
		return 0;
	}
	double	sum = 0;
	for (size_t i = 0; i < actual.size(); ++i)
	{
		double error = std::abs((actual[i] - predicted[i]) / actual[i]);
		sum += error;
	}
	return (sum / actual.size()) * 100.0;
}

static double	coefficient_of_determination_RSquared(const std::vector<double>& actual, const std::vector<double>& predicted)
{
	if (predicted.size() != actual.size())
	{
		std::cerr << "ERROR: Size mismatch between predicted and actual values." << std::endl;
		return 0;
	}

	double	sumSquaredDifference = 0;
	double	sumSquaredTotal = 0;
	double	meanActual = 0;

	for (size_t i = 0; i < predicted.size(); ++i)
	{
		sumSquaredDifference += std::pow(predicted[i] - actual[i], 2);
		meanActual += actual[i];
	}
	meanActual /= predicted.size();

	for (size_t i = 0; i < predicted.size(); ++i)
	{
		sumSquaredTotal += std::pow(actual[i] - meanActual, 2);
	}
	return 1.0 - (sumSquaredDifference / sumSquaredTotal);
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

	std::ofstream file_data(data_path);
	if (!file_data.is_open())
	{
		std::cerr << "ERROR: can't open/create file: " << data_path << std::endl;
		return -1;
	}
	std::vector<double>	x_tmp, y_tmp;
	for (size_t i = 0; i < x.size(); ++i)
	{
		file_data << x[i] << "\t" << y[i] << std::endl;
		x_tmp.push_back(x[i]);
		y_tmp.push_back(y[i]);
		solve_linear_regression(x_tmp, y_tmp, a, b);
		std::ofstream file_lr(linear_regression_path, std::ios::trunc);
		if (file_lr.is_open())
		{
			file_lr << minX << "\t" << a * minX + b << std::endl;
			file_lr << maxX << "\t" << a * maxX + b << std::endl;
			file_lr.close();
		}
		else
		{
			std::cerr << "ERROR: can't open/create file: " << linear_regression_path << std::endl;
			return -1;
		}
		plot("input_data_" + std::to_string(i) + ".png", true, minX, maxX, minY, maxY);
	}
	file_data.close();
	plot("input_data.png", false, minX, maxX, minY, maxY);
	plot("input_data_regres_line.png", true, minX, maxX, minY, maxY);

	std::ofstream file_precision(precision_path);
	if (file_precision.is_open())
	{
		std::vector<double>	y_predict;
		for (size_t i = 0; i < x.size(); ++i)
		{
			y_predict.push_back(a * x[i] + b);
			file_precision << x[i] << "\t" << y[i] << "\t" << y_predict[i] << std::endl;
		}
		file_precision << std::endl << "MAPE\t" << mean_absolute_percentage_error(y, y_predict) << std::endl;
		file_precision << std::endl << "coefficient_of_determination_RSquared\t" << coefficient_of_determination_RSquared(y, y_predict) << std::endl;
		
		file_precision.close();
	}
	else
	{
		std::cerr << "ERROR: can't open/create file: " << precision_path << std::endl;
		return -1;
	}
	return 0;
}
