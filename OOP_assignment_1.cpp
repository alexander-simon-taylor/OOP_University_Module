#include<iostream>
#include<iomanip>
#include<string>

double energy_difference(int n_initial, int n_final, int atomic_number) {
	double transition_energy = 13.6 * std::pow(atomic_number, 2) * (std::pow(n_final, -2) - std::pow(n_initial, -2));
	return transition_energy;
}

bool is_valid_positive_integer(std::string number)
try {
	int test = std::stoi(number);
	if (test >= 1) {
		return true;
	}
	else {
		return false;
	}
}
catch (...) {
	return false;
}

int main() {
	const double electron_charge{ 1.602e-19 };
	bool repeat_calc{ true };
	double transition_energy;
	int n_initial; int n_final; int atomic_number;
	std::string prompted_inputs; std::string continue_choice; std::string energy_unit;
	while (repeat_calc) {
		bool invalid_input{ true };
		while (invalid_input) {
			std::cout << "Enter a positive, non-zero integer for the atomic number of the atom: ";
			std::cin >> prompted_inputs;
			if (is_valid_positive_integer(prompted_inputs)) {
				atomic_number = std::stoi(prompted_inputs);
				invalid_input = false;
			}
		}
		invalid_input = true;
		while (invalid_input) {
			std::cout << "Enter a integer, two or greater, for the initial state of the electron: ";
			std::cin >> prompted_inputs;
			if (is_valid_positive_integer(prompted_inputs)) {
				n_initial = std::stoi(prompted_inputs);
				if (n_initial == 1) {
					invalid_input = false;
				}
				else {
					std::cout << "n = 1 is not a valid initial state for a spontaneous transition." << std::endl;
				}
			}
		}
		invalid_input = true;
		while (invalid_input) {
			std::cout << "Enter an integer, from one up to the initial state, for the final state of the electron: ";
			std::cin >> prompted_inputs;
			if (is_valid_positive_integer(prompted_inputs)) {
				n_final = std::stoi(prompted_inputs);
				if (n_final < n_initial) {
					invalid_input = false;
				}
				else {
					std::cout << "The quantum number for the final state must be smaller than the initial state quantum number." << std::endl;
				}
			}
		}
		invalid_input = true;
		transition_energy = energy_difference(n_initial, n_final, atomic_number);
		while (invalid_input) {
			std::cout << "Select the energy units you wish to use, \"J\" for joules, or \"eV\" for electron volts: ";
			std::cin >> energy_unit;
			if (energy_unit == "J" || energy_unit == "j") {
				std::cout << transition_energy * electron_charge << " J";
				invalid_input = false;
			}
			else if (energy_unit == "ev" || energy_unit == "eV" || energy_unit == "Ev" || energy_unit == "EV" || energy_unit == "e" || energy_unit == "E") {
				std::cout << transition_energy << " eV";
				invalid_input = false;
			}
		}
		invalid_input = true;
		while (invalid_input) {
			std::cout << std::endl << "Would you like to perform another calculation? Enter y for yes, or n for no: ";
			std::cin >> continue_choice;
			if (continue_choice == "n" || continue_choice == "N" || continue_choice == "no" || continue_choice == "No") {
				repeat_calc = false;
				invalid_input = false;
			}
			else if (continue_choice == "y" || continue_choice == "Y" || continue_choice == "yes" || continue_choice == "Yes") {
				invalid_input = false;
			}
		}

	}
	return 0;
}