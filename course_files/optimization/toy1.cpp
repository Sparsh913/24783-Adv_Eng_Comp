#include "solnp.hpp"
#include <iostream>

// Return value is a 1x1 matrix.
// Input parameter is Nx1 matrix, where N is the number of parameters.
dlib::matrix<double, 1, 1> objective_function(const dlib::matrix<double, 2, 1> &m)
{
    double x=m(0);
    double y=m(1);

    dlib::matrix<double,1,1> res;
    res(0)=(x-3.0)*(x-3.0)+(y-2.0)*(y-2.0)+1.0;

	std::cout << m(0) << " " << m(1) << " " << res(0) << "\n";

    return res;
}

int main(void)
{
    // 2 parameters
    // 3 values for each parameter, initial value, minimum, maximum
    dlib::matrix<double, 2, 3> parameter_data;
    // Weird syntax that destroys the beauty of C++, but that's what they like.
    parameter_data=
        0, -10.0,10.0,
        0, -10.0,10.0;

    cppsolnp::SolveResult calculate = cppsolnp::solnp(objective_function, parameter_data);

    double x=calculate.optimum(0);
    double y=calculate.optimum(1);
    double minimum=calculate.solve_value;

    std::cout << x << " " << y << " " << minimum << "\n";

    return 0;
}

