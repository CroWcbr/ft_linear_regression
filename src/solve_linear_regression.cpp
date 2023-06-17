#include <vector>
#include <iostream>

int	solve_linear_regression(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b)
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

	// a = (sum_xy - n * mean_x * mean_y) / (sum_xx - n * mean_x * mean_x);
	double denominator = sum_xx - n * mean_x * mean_x;
	if (denominator != 0)
	{
		a = (sum_xy - n * mean_x * mean_y) / denominator;
	}
	else
	{
		a = 0;
	}
	b = mean_y - a * mean_x;
	return 0;
}
