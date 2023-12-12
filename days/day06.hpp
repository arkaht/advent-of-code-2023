#pragma once

#include "common.h"

/**
* common
*/
template <typename T>
struct Race
{
	T time;
	T distance;
};

/**
* part one
*/
std::string part_one()
{
	//  open input
	std::ifstream file = read_input( "day6.txt" );

	const std::regex REGEX_NUMBER( "(\\d+)" );
	const std::regex REGEX_HEADER( "(\\w+):" );

	std::vector<Race<uint32_t>> races;

	//  gather symbols and numbers
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::smatch match;

		//  find header
		std::regex_search( line, match, REGEX_HEADER );
		std::string header = match[1].str();

		//  find numbers
		int race_id = 0;
		std::string::const_iterator start_search = line.cbegin();
		while ( std::regex_search( start_search, line.cend(), match, REGEX_NUMBER ) )
		{
			start_search += match.position() + match[0].str().length();
			//std::cout << match[1].str() << std::endl;

			uint32_t number = str_to_uint32( match[1].str() );
			if ( header == "Time" )
			{
				races.push_back( Race<uint32_t> { number, 0 } );
			}
			else if ( header == "Distance" )
			{
				races[race_id].distance = number;
			}

			race_id++;
		}
	}

	uint32_t solution = 1;
	for ( const auto& race : races )
	{
		uint32_t ways = 0;
		for ( uint32_t time = 0; time <= race.time; time++ )
		{
			uint32_t time_left = race.time - time;
			uint32_t distance = time_left * time;
			if ( distance > race.distance )
			{
				ways++;
			}
		}

		if ( ways > 0 )
		{
			solution *= ways;
		}
		std::cout << race.time << "ms for " << race.distance << "mm" << std::endl;
	}

	return std::to_string( solution );
}

/**
* part two
*/

std::string part_two()
{
		//  open input
	std::ifstream file = read_input( "day6.txt" );

	const std::regex REGEX_NUMBER( "(\\d+)" );
	const std::regex REGEX_HEADER( "(\\w+):" );

	Race<uint64_t> race { 0, 0 };

	//  gather symbols and numbers
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::smatch match;

		//  find header
		std::regex_search( line, match, REGEX_HEADER );
		std::string header = match[1].str();

		//  find numbers
		std::string str_number;
		std::string::const_iterator start_search = line.cbegin();
		while ( std::regex_search( start_search, line.cend(), match, REGEX_NUMBER ) )
		{
			start_search += match.position() + match[0].str().length();
			//std::cout << match[1].str() << std::endl;

			str_number += match[1].str();
		}

		uint64_t number = str_to_uint64( str_number );
		std::cout << str_number << " => " << number << std::endl;

		if ( header == "Time" )
		{
			race.time = number;
		}
		else if ( header == "Distance" )
		{
			race.distance = number;
		}
	}

	std::cout << race.time << "ms for " << race.distance << "mm" << std::endl;

	uint64_t ways = 0;
	for ( uint64_t time = 0; time <= race.time; time++ )
	{
		uint64_t time_left = race.time - time;
		uint64_t distance = time_left * time;
		if ( distance > race.distance )
		{
			ways++;
		}
	}

	return std::to_string( ways );
}