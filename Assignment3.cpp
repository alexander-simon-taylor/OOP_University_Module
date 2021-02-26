// Assignment_3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <locale>
#include <algorithm>

std::vector<std::string> load_course_list()
{
	// Loads the file courselist.dat as a vector of possible courses to compare to course inputs
	std::vector<std::string> possible_courses;
	std::ifstream possible_courses_file{ "D:/Documents/University/C++/Assignment_3/Assignment3_courselist.dat" };
	while (!possible_courses_file.eof())
	{
		std::string courses_line;
		std::getline(possible_courses_file, courses_line);
		possible_courses.push_back(courses_line);
	}
	return possible_courses;
}

std::string str_to_lower_case(std::string input_string)
{
	std::locale loc;
	int string_length = input_string.size();
	for (int i{}; i < string_length; i++)
	{
		input_string[i] = std::tolower(input_string[i],loc);
	}
	return input_string;
}

void print_courses(const std::vector<std::string> course_list, const std::string print_input)
{
	size_t course_list_length{ course_list.size() };
	for (int i{}; i < course_list_length; i++)
	{
		std::string first_char{course_list[i][0]};
		if (first_char == print_input || print_input == "all")
		{
			std::cout << "PHYS " << course_list[i] << std::endl;
		}
	}
}

std::vector<std::string> sort_courses(std::vector<std::string> course_list, const std::string sort_choice)
{
	if (sort_choice == "number")
	{
		sort(course_list.begin(), course_list.end());
	}
	else if (sort_choice == "name")
	{
		for (int i{}; i < size(course_list); i++)
		{
			size_t course_length{ std::size(course_list[i]) };
			course_list[i] = course_list[i].substr(5, course_length + 1) + course_list[i].substr(0, 5);
			// Puts the course number at the end of the string, letting the sort first act on the course name
		}
		sort(course_list.begin(), course_list.end());
		for (int i{}; i < size(course_list); i++)
		{
			size_t course_length{ std::size(course_list[i]) };
			course_list[i] = course_list[i].substr(course_length - 5, course_length) + course_list[i].substr(0, course_length - 5);
			// Undoes the rearrangement
		}
	}
	return course_list;
}

std::vector<std::string> course_list_extend(std::vector<std::string> course_list, const std::string course_input, const std::vector<std::string> possible_courses)
{
	std::size_t possible_course_length{possible_courses.size()};
	for (int i{}; i < possible_course_length; i++)
	{
		if (str_to_lower_case(course_input) == str_to_lower_case(possible_courses[i]))
		{
			course_list.push_back(course_input);
			break;
		}
	}
	return course_list;
}

int main()
{
	bool program_check{ true };
	const std::vector<std::string> possible_courses{ load_course_list() };
	std::vector<std::string> entered_course_list;
	while (program_check)
	{
		bool invalid_input_check{ true };
		while (invalid_input_check)
		{
			std::string entered_input;
			std::cout << "To enter a new course, enter 'new'. To print out courses, enter 'print'. To sort the courses, enter 'sort'. To close the program, enter 'close'." << std::endl;
			std::getline(std::cin, entered_input);
			if (entered_input == "close") { return 0; }
			else if (entered_input == "new")
			{
				while (invalid_input_check)
				{
					std::string course_input;
					std::cout << "To go back enter 'back', to close the program enter 'close', or enter a course number and name." << std::endl;
					std::getline(std::cin, course_input);
					if (course_input == "close") { return 0; }
					else if (course_input == "back")
					{
						invalid_input_check = false;
					}
					else
					{
						std::size_t old_size{ entered_course_list.size() };
						entered_course_list = course_list_extend(entered_course_list,course_input,possible_courses);
						if (entered_course_list.size() == old_size)
						{
							std::cout << "Invalid course name." << std::endl;
						}
					}
				}
			}
			else if (entered_input == "sort")
			{
				if (entered_course_list.size() > 1)
					// Only sorting lists with two of more elements
				{
					std::string sort_input;
					while (invalid_input_check)
					{
						std::cout << "To go back enter 'back', to close the program enter 'close', to sort by number enter 'number', to sort by name enter 'name'." << std::endl;
						std::getline(std::cin, sort_input);
						if (sort_input == "close") { return 0; }
						else if (sort_input == "back")
						{
							invalid_input_check = false;
						}
						else if (sort_input == "number" || sort_input == "name")
						{
							entered_course_list = sort_courses(entered_course_list, sort_input);
							invalid_input_check = false;
						}
					}
				}
				else // list of entered courses has only one or zero elements
				{
					std::cout << "Cannot sort a list with fewer than two elements." << std::endl;
				}
				invalid_input_check = false;
			}
			else if (entered_input == "print")
			{
				if (entered_course_list.size() > 0 && invalid_input_check == true)
					// Only priting lists with more than one element
				{
					std::string print_input;
					while (invalid_input_check)
					{
						std::cout << "To go back enter 'back', to close the program enter 'close', to print all courses enter 'all', or to print only courses for a specific year, enter that year's number." << std::endl;
						std::cin >> print_input;
						if (print_input == "close") { return 0; }
						else if (print_input == "back")
						{
							invalid_input_check = false;
						}
						else if (print_input == "1" || print_input == "2" || print_input == "3" || print_input == "4" || print_input == "all")
						{
							print_courses(entered_course_list, print_input);
							invalid_input_check = false;
						}
					}
				}
				else // list of entered courses has only one or zero elements
				{
					std::cout << "Cannot print a list with no elements." << std::endl;
				}
				invalid_input_check = false;
			}
		}
	}
	return 0;
}
