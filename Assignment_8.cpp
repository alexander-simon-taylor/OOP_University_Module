// Assignment_8.cpp : This contains a purely virtual base class shapes, then two derived virtual classes, shapes_2d and shapes_3d.
// Each of those has several different derived classes, each for a different 2d or 3d shape.
// In the base class the functions area, volume, and perimeter are defined. For 3d shapes the perimeter function always gives 0,
//	   for 2d shapes the volume function always gives 0.
// Pi is also defined in the base class, because it is useful.

#include <iostream>
#include <vector>

class shapes
{
protected:
	const double pi{ 3.14159265358979 };
public:
	virtual ~shapes() {}
	virtual double area() = 0;
	virtual double volume() = 0;
	virtual double perimeter() = 0;
	// Area returns area for 2-D shapes, and surface area for 3-D shapes
};

class shapes_2d : virtual public shapes
{
public:
	virtual ~shapes_2d() {}
	virtual double area() = 0;
	double volume() { return 0; };
	virtual double perimeter() = 0;
};

class shapes_3d : virtual public shapes
{
public:
	virtual ~shapes_3d() {}
	virtual double area() = 0;
	virtual double volume() = 0;
	double perimeter() { return 0; }
};

class rectangle : virtual public shapes_2d
{
protected:
	double length{}; double width{};
public:
	rectangle() = default;
	rectangle(double input_x, double input_y) : length{input_x}, width{input_y} {};
	~rectangle() {}
	double area() { return length * width; };
	double perimeter() { return 2 * (length + width); }
};

class square : public rectangle
{
public:
	square() : rectangle() {};
	square(double side_length) : rectangle(side_length, side_length) {};
	~square() {}
};

class ellipse : virtual public shapes_2d
{
protected:
	double semi_major_axis{}; double semi_minor_axis{};
public:
	ellipse() = default;
	ellipse(double major_input, double minor_input) : semi_major_axis{ major_input }, semi_minor_axis{ minor_input } {};
	~ellipse() {}
	double area() { return pi * semi_major_axis*semi_minor_axis; }
	double perimeter() {
		// Using an approximation for the perimeter given by Ramanujan
		double h{ pow(semi_major_axis - semi_minor_axis,2) / pow(semi_major_axis + semi_minor_axis,2) };
		double circumference{ 1 + 3 * h / (10 + sqrt(4 - 3 * h)) };
		return pi*(semi_major_axis + semi_minor_axis)*circumference;
	}
};

class circle : public ellipse
{
public:
	circle() : ellipse() {}
	circle(double radius) : ellipse(radius, radius) {}
	~circle() {}
	double perimeter() { return 2*pi*semi_major_axis; }
	// An exact formula exists for a circle's perimeter - I may as well use it
};

class cuboid : virtual public shapes_3d
{
protected:
	double length{}; double width{}; double height{};
public:
	cuboid() = default;
	cuboid(double length_input, double width_input, double height_input) :
		length{ length_input }, width{ width_input }, height{ height_input } {}
	~cuboid() {}
	double area() { return 2*(length*width + length*height + width*height); }
	double volume() { return length*width*height; }
};

class cube : public cuboid
{
public:
	cube() : cuboid() {}
	cube(double side_length) : cuboid(side_length, side_length, side_length) {}
	~cube() {}
};

class ellipsoid : virtual public shapes_3d
{
protected:
	double axis_a{}; double axis_b{}; double axis_c{};
public:
	ellipsoid() = default;
	ellipsoid(double input_a, double input_b, double input_c) :
		axis_a{ input_a }, axis_b{ input_b }, axis_c{ input_c } {}
	double area() {
		const double power_const{ 1.6075 };
		// This constant is used in the approximation of the surface area
		double surface_area{ pow((pow(axis_a*axis_b,power_const) + pow(axis_a*axis_c,power_const) + pow(axis_b*axis_c,power_const)) / 3, 1/power_const) };
		return 4*pi*surface_area;
	}
	double volume() { return 4 * pi*axis_a*axis_b*axis_c / 3; }
};

class sphere : public ellipsoid
{
public:
	sphere() : ellipsoid() {}
	sphere(double radius) : ellipsoid(radius, radius, radius) {}
	~sphere() {}
	double area() { return 4*pi*pow(axis_a, 2); }
	// An exact formula exists for a sphere's surface area - I may as well use it
};

class prism : virtual public shapes_3d
{
private:
	double face_area{}; double face_perimeter; double depth{};
public:
	prism(shapes_2d& face_input, double depth_input) :
		face_area{ face_input.area() }, face_perimeter{ face_input.perimeter() }, depth{ depth_input } {}
	~prism() {}
	double area() { return 2 * face_area + face_perimeter * depth; }
	double volume() { return face_area * depth; }
};

int main()
{
	std::vector<shapes*> base_class_pointers;
	rectangle rectangle_1{5, 2};
	base_class_pointers.push_back(&rectangle_1);
	square square_1{ 4 };
	base_class_pointers.push_back(&square_1);
	ellipse ellipse_1{ 2,3 };
	base_class_pointers.push_back(&ellipse_1);
	circle circle_1{ 2.5 };
	base_class_pointers.push_back(&circle_1);
	cuboid cuboid_1{ 1,4,9 };
	base_class_pointers.push_back(&cuboid_1);
	cube cube_1{5};
	base_class_pointers.push_back(&cube_1);
	ellipsoid ellipsoid_1{ 1,3,5 };
	base_class_pointers.push_back(&ellipsoid_1);
	sphere sphere_1{ 0.5 };
	base_class_pointers.push_back(&sphere_1);
	prism rectangular_prism_1{ rectangle_1, 3 };
	base_class_pointers.push_back(&rectangular_prism_1);
	std::cout << "The area of a rectangle with side lengths of 5 and 2 is " << base_class_pointers[0]->area() << std::endl;
	std::cout << "The area of a square with a side length of 4 is " << base_class_pointers[1]->area() << std::endl;
	std::cout << "The area of an ellipse with semi-axes of 2 and 3 is " << base_class_pointers[2]->area() << std::endl;
	std::cout << "The area of a circle with a radius of 2.5 is " << base_class_pointers[3]->area() << std::endl;
	std::cout << "The volume of a cuboid with side lengths of 1, 4, and 9 is " << base_class_pointers[4]->volume() << std::endl;
	std::cout << "The volume of a cube with a side length of 5 is " << base_class_pointers[5]->volume() << std::endl;
	std::cout << "The volume of a ellipsoid with semi-axes of 1, 3, and 5 is " << base_class_pointers[6]->volume() << std::endl;
	std::cout << "The volume of a sphere with a radius of 0.5 is " << base_class_pointers[7]->volume() << std::endl;
	std::cout << "The surface area of a sphere with a radius of 0.5 is " << base_class_pointers[7]->area() << std::endl;
	std::cout << "The volume of a rectangular prism with side lengths 5, 2, and a depth of 3 is " << base_class_pointers[8]->volume() << std::endl;
	base_class_pointers.clear();
}
