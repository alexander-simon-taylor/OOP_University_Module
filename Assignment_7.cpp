// Assignment_7.cpp. This file contains classes defining an n-dimensional Cartesian vector, a 3+1 dimensional Minkowski vector, and a particle class combining these.
// These classes have functions returning useful quantities such as dot products, Lorentz boosts, energy, and momentum.

#include <iostream>
#include <vector>
#include <cstdarg>
#include <string>

class cart_vector {
	friend std::ostream& operator<<(std::ostream& output_stream, cart_vector& v);
protected:
	std::vector<double> components{};
public:
	cart_vector() = default;
	~cart_vector() {};
	// Constructor for a vector of a given length
	cart_vector(const int number) :
		components(number, 0.0) {};
	cart_vector(std::vector<double> vector_components) {
		components = vector_components;
	}
	// Copy constructor
	cart_vector(cart_vector& v) :
		components(v.vec_length()) {
			for (int i{}; i < v.vec_length(); i++) {
				components[i] = v[i+1];
			}
		}
	// Move constructor
	cart_vector(cart_vector&& v) noexcept
	{
		std::swap(components, v.components);
	}
	// copy assignment
	cart_vector operator=(cart_vector& v) noexcept
	{
		if (&v == this) { return *this; }
		std::vector<double> components(v.vec_length());
		for (int i{}; i < v.vec_length(); i++) {
			components[i] = v[i];
		}
		return *this;
	}
	// Move assignment
	cart_vector operator=(cart_vector&& v) noexcept
	{
		std::swap(components, v.components);
		return *this;
	}
	// Dot product
	double dot_product(cart_vector& v) {
		int vector_length{ v.vec_length() };
		double dot_result{};
		for (int i{1}; i <= vector_length; i++) {
			dot_result += components[i-1] * v[i];
		}
		return dot_result;
	}
	int vec_length() {
		int convertdata = static_cast<int>(std::size(components));
		return convertdata;
	}
	double operator[] (const int i) {
		if (i <= this->vec_length() && i > 0) {
			return components[i - 1];
		}
		else {
			return 0;
			// Need to insert useful error message
		};
	}
	cart_vector operator+ (cart_vector v) {
		if (v.vec_length() == this->vec_length()) {
			std::vector<double> vector_components(this->vec_length(),0);
			for (int i{}; i < v.vec_length(); i++) {
				vector_components[i] = components[i] + v[i+1];
			}
			cart_vector return_vector{ vector_components };
			return return_vector;
		} else {
			return 0;
			// Need to insert useful error message
		}
	}
	cart_vector operator- (cart_vector v) {
		if (v.vec_length() == this->vec_length()) {
			std::vector<double> vector_components(v.vec_length(),0);
			for (int i{}; i < v.vec_length(); i++) {
				vector_components[i] = components[i] - v[i+1];
			}
			cart_vector return_vector{ vector_components };
			return return_vector;
		} else {
			return 0;
			// Need to insert useful error message
		}
	}
	cart_vector operator* (const double scalar) {
		std::vector<double> multiplied_vector(this->vec_length());
		for (int i{}; i < this->vec_length(); i++) {
			multiplied_vector[i] = components[i]*scalar;
		}
		cart_vector return_vector{ multiplied_vector };
		return return_vector;
	}
	cart_vector operator/ (const double scalar) {
		std::vector<double> divided_vector(this->vec_length());
		for (int i{}; i < this->vec_length(); i++) {
			divided_vector[i] = components[i] / scalar;
		}
		cart_vector return_vector{ divided_vector };
		return return_vector;
	}
};

std::ostream& operator<<(std::ostream& output_stream, cart_vector& v)
{
	output_stream << "(";
	for (int i{1}; i <= v.vec_length(); i++) {
		if (i != v.vec_length()) {
			output_stream << v[i] << ", ";
		} else {
			output_stream << v[i];
		}
	}
	output_stream << ")";
	return output_stream;
}

class mink_vector : public cart_vector {
	friend std::ostream& operator<<(std::ostream& output_stream, mink_vector& v);
private:
	std::vector<double> components;
public:
	mink_vector() : components{ 0,0,0,0 } {}
	~mink_vector() {}
	// Constructor for four separate components
	mink_vector(double ct, double x, double y, double z)
		: components{ ct, x, y, z} {}
		//components[0] = ct, components[1] = x, components[2] = y, components[3] = z {};
	// Constructor for one time-like and one space-like object
	mink_vector(const double& ct, cart_vector& cart)
		: components{ ct,cart[1],cart[2],cart[3] } {}
	// Copy constructor from Cartesian to Minkowski vector
	mink_vector(cart_vector cart)
		: components{ cart[1],cart[2],cart[3],cart[4] } {}
	// Copy constructor
	mink_vector(mink_vector& v)
		: components{ v[0],v[1],v[2],v[3] } {}
	// Move constructor
	mink_vector(mink_vector&& v) noexcept
	{
		std::swap(components, v.components);
	}
	// copy assignment
	mink_vector operator=(mink_vector v) noexcept
	{
		if (&v == this) { return *this; }
		std::vector<double> components(v.vec_length());
		for (int i{}; i < v.vec_length(); i++) {
			components[i] = v[i];
		}
		return *this;
	}
	// Move assignment
	mink_vector operator=(mink_vector&& v) noexcept
	{
		std::swap(components, v.components);
		return *this;
	}
	double operator[] (int i) {
		if (i >= 0 && i <= 3) {
			return components[i];
		}
		else {
			return 0;
			// Need to insert useful error message
		};
	};
	int vec_length() {
		return 4;
	}
	// Minkowski vector dot product
	double dot_product(mink_vector v) const {
		return components[0] * v[0] - components[1] * v[1] - components[2] * v[2] - components[3] * v[3];
	};
	mink_vector lorentz_boost(cart_vector& beta);
};

std::ostream& operator<<(std::ostream& output_stream, mink_vector& v)
{
	output_stream << "(";
	for (int i{0}; i <= 3; i++) {
		if (i != 3) {

			output_stream << v.components[i] << ", ";
		}
		else {
			output_stream << v[i];
		}
	}
	output_stream << ")";
	return output_stream;
}

mink_vector mink_vector::lorentz_boost(cart_vector& beta) {
	beta = beta;
	double gamma_value{ pow(1 - beta.dot_product(beta), -0.5) };
	std::vector<double> three_vec{ components[1],components[2],components[3] };
	cart_vector three_pos{ three_vec };
	double time_like{ gamma_value * (components[0] - beta.dot_product(three_pos)) };
	cart_vector space_like{ three_pos + beta * ((gamma_value - 1)*three_pos.dot_product(beta) / (beta.dot_product(beta)) - gamma_value * components[0]) };
	mink_vector return_vector{ time_like,space_like };
	return return_vector;
}

class particle {
private:
	double mass{};
	cart_vector beta{3};
	mink_vector position{};
public:
	particle() = default;
	~particle() {}
	particle(const double input_mass, cart_vector& input_beta, mink_vector& input_position) :
		mass{ input_mass }, beta{ input_beta }, position{ input_position }
		{std::cout << beta << std::endl; }
	double gamma() {
		double gamma_value{ std::pow(1 - beta.dot_product(beta), -0.5) };
		return gamma_value;
	}
	double total_energy() { return this->gamma()*mass; }
	cart_vector momentum() { return beta*(this->gamma())*mass;
	std::cout << beta << std::endl;
	}
};

int main()
{
	// Initialising a cartesian vector using a vector
	std::vector<double> components_1{ 1,2,3};
	cart_vector c_vector1{components_1};
	std::cout << "The first component of c_vector1 is " << c_vector1[1] << std::endl;
	std::cout << "The second componenet of c_vector1 is " << c_vector1[2] << std::endl;
	std::cout << "c_vector1 is " << c_vector1 << std::endl;
	std::cout << "The legnth of c_vector1 is " << c_vector1.vec_length() << std::endl;
	std::vector<double> components_2{ 2,3,0 };
	cart_vector c_vector2{ components_2 };
	std::cout << "The dot product of c_vector1 and c_vector2 is" << c_vector1.dot_product(c_vector2) << std::endl;
	// Minkowski vector default constructor
	mink_vector m_vector1{};
	std::cout << "The default constructed Mikowski vector is " << m_vector1 << std::endl;
	mink_vector m_vector2{ 2,-1,1,1 };
	mink_vector m_vector3{ 4,-2,-1,1 };
	std::cout << "The dot product of (2,-1,1,1) and (4,-2,-1,1) is " << m_vector2.dot_product(m_vector3) << std::endl;
	cart_vector beta{ c_vector1 / 10 };
	mink_vector boosted_vector{ m_vector2.lorentz_boost(beta) };
	std::cout << "The Lorentz boosted vector " << m_vector2 << " using beta = " << beta << " is " << boosted_vector << std::endl;
	particle electron{ 0.511,beta,m_vector2 };
	std::cout << "The properties of an electron are gamma = " << electron.gamma() << ", total energy = " << electron.total_energy();
	cart_vector electron_mom{electron.momentum()};
	std::cout << " MeV/c^2, and momentum = " << electron_mom << " MeV/c." << std::endl;
}