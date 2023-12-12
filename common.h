#pragma once

#include <assert.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <regex>
#include <numeric>

#include <unordered_map>
#include <unordered_set>

#include <thread>
#include <mutex>

std::ifstream read_input( const std::string& filename )
{
	return std::ifstream( "inputs/" + filename );
}

int str_to_int( const std::string& str )
{
	return atoi( str.c_str() );
}

/**
* atoi() doesn't account for integers greater than INT_MAX so
* we need a way for uint32_t numbers
*/
uint32_t str_to_uint32( const std::string& str )
{
	return strtoul( str.c_str(), NULL, 10 );
}

/**
 * str_to_uint32 equivalent for uint64_t
 */
uint64_t str_to_uint64( const std::string& str )
{
	return strtoull( str.c_str(), NULL, 10 );
}

//  https://www.geeksforgeeks.org/lcm-of-given-array-elements/?ref=lbp
/**
 * Least Common Multiple of a given array of numbers
 */
template <typename T>
T lcm( const std::vector<T> numbers )
{
	T result = numbers[0];

	for ( const T& num : numbers )
	{
		result = ( num * result ) / std::gcd( num, result );
	}

	return result;
}

struct Int2
{
	int x;
	int y;

	bool operator==( const Int2& other )
	{
		return x == other.x 
			&& y == other.y;
	}
};

int shoelace_area( const std::vector<Int2> polygon )
{
	int sum_x = 0, sum_y = 0;
	for ( int i = 0; i < polygon.size(); i++ )
	{
		const Int2& a = polygon[i];
		const Int2& b = polygon[( i + 1 ) % polygon.size()];
		//printf( "%d * %d\n", i, ( i + 1 ) % polygon.size() );

		sum_x += a.x * b.y;
		sum_y += a.y * b.x;
	}
	return abs( sum_x - sum_y ) / 2;
}