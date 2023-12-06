#pragma once

#include <assert.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <string>
#include <regex>

#include <unordered_map>
#include <unordered_set>

#include <thread>
#include <mutex>

/**
* atoi() doesn't account for integers greater than INT_MAX so
* we need a way for uint32_t and uint64_t numbers
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