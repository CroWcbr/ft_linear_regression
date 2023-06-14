#include <fstream>
#include <sstream>
#include <iostream>

static int	find_a_b(const char* train_model, double& a, double& b)
{
	std::ifstream file(train_model);
	if (file)
	{
		if (!file.is_open())
		{
			std::cerr << "ERROR: can't open file" << std::endl;
			return 1;
		}
		if (file >> a >> b)
		{
			char c;
			if (file.get(c) && !file.eof())
			{
				std::cerr << "ERROR: wrong info in model (must be only 2 double)" << std::endl;
				file.close();
				return 1;
			}
		}
		else
		{
			std::cerr << "ERROR: wrong info in model (there are no 2 double)" << std::endl;
			file.close();
			return 1;
		}
	}
	else
	{
		a = b = 0;
	}
	file.close();
	return 0;
}

int	input_mileage(double& mileage)
{
	std::string input;

	std::cout << "Input mileage:\t\t";
	std::getline(std::cin, input);

	std::istringstream iss(input);
	if (iss >> mileage)
	{
		char c;
		if (iss.get(c) && !iss.eof())
		{
			std::cerr << "Error: wrong input! any symbol after number!" << std::endl;
			return 1;
		}
		if (mileage < 0)
		{
			std::cerr << "Error: wrong input! mileage < 0" << std::endl;
			return 1;
		}
	}
	else
	{
		std::cerr << "Error: wrong input!" << std::endl;
		return 1;
	}
	return 0;
}

int main()
{
	const char*	train_model = "./data/model.csv";
	double	a, b; // f(x) = a * x + b;
	double	mileage;

	if (find_a_b(train_model, a, b) || \
		input_mileage(mileage))
		return -1;

	std::cout << "Linear_regression:\tf(x) = " << (a < 0 ? "-" : "")<< a << " * x " << (b < 0 ? "- " : "+ ") << b << std::endl;
	std::cout << "Expected price :\t" << a * mileage + b << std::endl;
	return 0;
}
