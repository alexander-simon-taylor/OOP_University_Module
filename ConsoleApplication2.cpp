// should write blurb explaining purpose of code

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>

double mean(int number, double data[])
{
	double calculated_mean{};
	for (int i{ 0 }; i < number; i++)
	{
		calculated_mean += data[i];
	}
	return calculated_mean / number;
}

double standard_deviation(int number, double data[], double mean)
{
	double variance{};
	for (int i = 0; i < number; i++) 
	{
		variance += std::pow(data[i] - mean, 2);
	}
	return std::sqrt(variance / (number - 1));
}

int main() {
	int number{};
	std::string file_path{ "D:/Documents/University/C++/Assignment 2/millikan2.dat" };
	std::ifstream data_file{ file_path };
	// D:\Documents\University\C++\Assignment 2\;
	if (!data_file.good())
	{
		std::cerr << "File failed to load." << std::endl;
		return 0;
	}
	std::string line;
	std::string loaded_line;
	while (!data_file.eof())
	{
		std::string loaded_line;
		data_file >> loaded_line;
		try
		{
			double tested_line{ std::stod(loaded_line) };
			// tests whether the input can be cast to a double
			number++;
		}
		catch (...)
		{
			continue;
			// if it can't be cast to a double, skip the line
		}
	}
	std::cout << "Number of lines = " << number << std::endl;
	double *loaded_data{ new double[number] };
	int line_index{ 0 };
	data_file.close();
	data_file.open(file_path);
	while (!data_file.eof() && line_index < number)
	{
		data_file >> loaded_line;
		try
		{
			double tested_line{ std::stod(loaded_line) };
			// tests whether the input can be cast to a double
			loaded_data[line_index] = tested_line;
			line_index++;
		}
		catch (...)
		{
			continue;
			// if it can't be cast to a double, skip the line
		}
	}
	data_file.close();
	double data_mean = mean(number, loaded_data);
	double data_std_deviation = standard_deviation(number, loaded_data, data_mean);
	double data_error_on_mean = data_std_deviation / std::sqrt(number);
	delete[] loaded_data;
	char placeholder;
	std::cout << "The mean of the data is " << data_mean << ", the data's standard deviation is " << data_std_deviation << ", and the error on the mean is " << data_error_on_mean << "." << std::endl;
	std::cout << "Enter any character to end.";
	std::cin >> placeholder;
	return 0;
}