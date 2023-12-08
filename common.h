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