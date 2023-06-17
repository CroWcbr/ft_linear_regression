#include <vector>

int	read_data(const char* data, std::vector<double>& x, std::vector<double>& y);
int	solve_linear_regression(const std::vector<double>& x, const std::vector<double>& y, double& a, double& b);
int	write_model(const char* model, const double& a, const double& b);

int main()
{
	const char*	data = "./data/data.csv";
	const char* model = "./data/model.csv";
	std::vector<double>	x, y;
	double a, b;
	if (read_data(data, x, y) || \
		solve_linear_regression(x, y, a, b) || \
		write_model(model, a, b))
		return -1;
	return 0;
}
