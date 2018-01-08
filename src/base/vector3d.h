#ifndef _VECTOR_H
#define _VECTOR_H

#include "utilities.h" 

/** \brief Class to manipulate 3d vectors (addition, mult, projections...).
 *
 * Operators overloading are defined, constructor, copy, printing...
 * \todo use it also in BioFVM classes when its 3d object
 *
 * \author Gaelle Letort, Institut Curie
 * \date March 2017
 */
class Vector3d
{
	public:
		/** \brief 3 coordinates of vector */
		double data[3];

	public:

		/** \brief Constructor */
		Vector3d();

		/** \brief Copy Constructor */
		Vector3d( const Vector3d& );

		/** \brief Constructor by values */
		Vector3d( double x, double y, double z );

		/** \brief Destructor */
		~Vector3d();

		/** \brief Set coordinates values */
		inline void set( double x, double y, double z ) { data[0] = x; data[1] = y; data[2] = z; };

		///// Operators
		/** \brief Compare values */
		inline bool operator==( const Vector3d v )
		{ return (data[0]==v.data[0]) && (data[1]==v.data[1]) && (data[2]==v.data[2]) ; }
		/** \brief Copy values */
		Vector3d & operator=( const Vector3d & otherVector3d );
		/** \brief Add each coordinates to current vector */
		Vector3d & operator+=( const Vector3d &otherVector3d );
		/** \brief Substract each coordinates to current vector */
		Vector3d & operator-=( const Vector3d &otherVector3d );
		/** \brief Multiply each coordinates by constant */
		Vector3d & operator*=( const double factor );
		/** \brief Divide each coordinates by constant */
		Vector3d & operator/=( const double factor );
	    /** \brief Access index-th coordinates */	
		double & operator[](const size_t index);
		
		/** \brief Multiply by constant */
		friend const Vector3d operator*( const double factor, const Vector3d& v )
		{ return Vector3d( factor*v.data[0], factor*v.data[1], factor*v.data[2] ); }
		/** \brief Multiply by constant */
		friend const Vector3d operator*( const Vector3d& v, const double factor )
		{ return Vector3d( factor*v.data[0], factor*v.data[1], factor*v.data[2] ); }
		/** \brief Scalar product of the 2 vectors */
		friend const double operator*( const Vector3d &one, const Vector3d &two )
		{ return one.data[0]*two.data[0] + one.data[1]*two.data[1] + one.data[2]*two.data[2]; }
		/** \brief Substraction of the 2 vectors */
		friend const Vector3d operator-( const Vector3d &one, const Vector3d &two )
		{ return Vector3d( one.data[0]-two.data[0], one.data[1]-two.data[1], one.data[2]-two.data[2]); }
		/** \brief Addition of the 2 vectors */
		friend const Vector3d operator+( const Vector3d &one, const Vector3d &two )
		{ return Vector3d( one.data[0]+two.data[0], one.data[1]+two.data[1], one.data[2]+two.data[2]); }
		
		
		//// Functions
		/** \brief Get vector norm */
		inline double norm() const { return sqrt( data[0]*data[0] + data[1]*data[1] + data[2]*data[2] ); }
		/** \brief Get vector squared norm */
		inline double normSqr() const { return ( data[0]*data[0] + data[1]*data[1] + data[2]*data[2] ); }

		/** \brief Get distance to x-axis */
		inline double distance_to_xaxis() const { return sqrt( data[1]*data[1] + data[2]*data[2] ); }
		/** \brief Get distance to y-axis */
		inline double distance_to_yaxis() const { return sqrt( data[0]*data[0] + data[2]*data[2] ); }
		/** \brief Get distance to z-axis */
		inline double distance_to_zaxis() const { return sqrt( data[0]*data[0] + data[1]*data[1] ); }

		/** \brief Assign random values to current vector, with norm 1 */
		void randomize_normed();

		/** \brief Return unit direction of vector */
		inline Vector3d dir() const
		{
			double n = norm();
			if ( n < EPSILON ) return Vector3d(0, 0, 0);
			return Vector3d( data[0]/n, data[1]/n, data[2]/n );
		}

		/** \brief Normalize the vector */
		inline void normalize()
		{
			double n = norm();
			if ( n < EPSILON ) 
			{
				randomize_normed();
				return;
			}
			for ( int i = 0; i < 3; i++ ) data[i] /= n;
		}
		
		/** \brief Return printable string of coordinates values */
		std::string str() const;
		
};

std::ostream & operator << ( std::ostream &, const Vector3d&);
#endif
