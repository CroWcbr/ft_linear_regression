#include <iostream>
#include <fstream>

int	write_model(const char* model, const double& a, const double& b)
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