#include <cmath>
#include <sstream>
#include "vector3d.h"

Vector3d::Vector3d()
{
	for ( int i = 0; i < 3; i++ )
		data[i] = 0;
}

Vector3d::~Vector3d() {}

Vector3d::Vector3d( double x, double y, double z )
{
	data[0] = x;
	data[1] = y;
	data[2] = z;
}

Vector3d::Vector3d( const Vector3d& v )
{
	for ( int i = 0; i < 3; i++ )
		data[i] = v.data[i];
}

Vector3d& Vector3d::operator=( const Vector3d& otherVector3d )
{
	for ( int i = 0; i < 3; i++ )
		data[i] = otherVector3d.data[i];
	return *this;
}

double& Vector3d::operator[]( const size_t index )
{
#ifdef NDEBUG
	if( index >= 3 )
	{
		std::cout << "Debug: Error in vector3d: index is greater than 2" << std::endl;
	}
#endif
	return data[index];
}
  
Vector3d & Vector3d::operator+=( const Vector3d &otherVector3d )
{
	for ( int i = 0; i < 3; i++ )
		data[i] += otherVector3d.data[i];
	return (*this);
}

Vector3d & Vector3d::operator-=( const Vector3d &otherVector3d )
{
	for ( int i = 0; i < 3; i++ )
		data[i] -= otherVector3d.data[i];
	return (*this);
}

Vector3d & Vector3d::operator*=( const double factor )
{
	for ( int i = 0; i < 3; i++ )
		data[i] *= factor;
	return (*this);
}

Vector3d & Vector3d::operator/=( const double factor )
{
	double f = factor;
	// not to divide by 0
	if ( fabs(factor) < EPSILON )
		f = sign(factor) * EPSILON * 10;
	for ( int i = 0; i < 3; i++ )
		data[i] /= f;
	return (*this);
}

std::string Vector3d::str() const
{
	std::ostringstream convert;
	convert << data[0] << delim << data[1] << delim << data[2];
	return convert.str();
}

std::ostream & operator << ( std::ostream & os, const Vector3d& v)
{
	os << v.str();
	return os;
}
		
/** Assign random values to current vector, with norm 1 */
void Vector3d::randomize_normed()
{
	double temp_angle = 2 * M_PI * UniformRandom();
	double temp_phi = M_PI * UniformRandom();
	data[0] = cos( temp_angle ) * sin( temp_phi );
	data[1] = sin( temp_angle ) * sin( temp_phi );
	data[2] = cos( temp_phi );
}
