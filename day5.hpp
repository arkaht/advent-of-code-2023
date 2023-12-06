#pragma once

#include "common.h"

/**
* common
*/
struct MapRange
{
	uint32_t destination_start;
	uint32_t source_start;
	uint32_t length;

	bool is_within( uint32_t value ) const
	{
		return value >= source_start && value < source_start + length;
	}

	int remap( uint32_t value ) const
	{
		uint32_t offset = value - source_start;
		return destination_start + offset;
	}
};

using almanac_t = std::unordered_map<std::string, std::vector<MapRange>>;

std::vector<std::string> processes
{
	"seed-to-soil",
	"soil-to-fertilizer",
	"fertilizer-to-water",
	"water-to-light",
	"light-to-temperature",
	"temperature-to-humidity",
	"humidity-to-location",
};
almanac_t almanac;

/**
* part one
*/

std::string part_one()
{
	//  open input
	std::ifstream file( "day5.txt" );

	const std::regex REGEX_SEED( "(\\d+)" );
	const std::regex REGEX_RANGE( "(\\d+) (\\d+) (\\d+)" );
	//					      destination source length
	const std::regex REGEX_MAP_NAME( "([\\w\\-]+) map:" );

	std::vector<uint32_t> seeds;
	std::string current_map_name;

	//  gather symbols and numbers
	int line_id = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		int result = 0;

		if ( line.empty() )
		{
			current_map_name.clear();
			continue;
		}

		std::smatch match;
		if ( line_id == 0 )
		{
			//  find seeds
			std::string::const_iterator start_search = line.cbegin();
			while ( std::regex_search( start_search, line.cend(), match, REGEX_SEED ) )
			{
				start_search += match.position() + match[0].str().length();
				//std::cout << match[1].str() << std::endl;

				uint32_t seed = str_to_uint32( match[1].str() );
				seeds.push_back( seed );
			}
		}
		else if ( current_map_name.empty() )
		{
			std::regex_search( line, match, REGEX_MAP_NAME );
			current_map_name = match[1].str();
			almanac[current_map_name] = std::vector<MapRange>();
		}
		else 
		{
			std::regex_search( line, match, REGEX_RANGE );

			uint32_t destination_start = str_to_uint32( match[1].str() );
			uint32_t source_start = str_to_uint32( match[2].str() );
			uint32_t length = str_to_uint32( match[3].str() );
			almanac[current_map_name].push_back( MapRange { destination_start, source_start, length } );

			//  map values
			/*for ( int i = 0; i < length; i++ )
			{
				almanac[current_map_name][start_source + i] = start_destination + i;
			}*/
		}
		
		line_id++;
		//std::cout << line << " | " << current_map_name << std::endl;
	}

	uint32_t minimum_location = std::numeric_limits<uint32_t>().max();
	//uint32_t test = str_to_uint32( "4294967295" );

	//  convert seeds to location
	for ( const auto& seed : seeds ) 
	{
		uint32_t value = seed;
		for ( const auto& process : processes )
		{
			/*if ( process == "fertilizer-to-water" )
			{
				std::cout << std::endl;
			}*/

			const auto& ranges = almanac[process];
			for ( const auto& range : ranges )
			{
				if ( !range.is_within( value ) ) continue;

				value = range.remap( value );
				break;
			}

			std::cout << seed << " : " << process << " : " << value << std::endl;
		}

		//  set the minimum location
		if ( value < minimum_location )
		{
			minimum_location = value;
		}

		std::cout << seed << " : " << value << std::endl;
	}

	return std::to_string( minimum_location );
}

/**
* part two
* 
* ~~highly slow solution, took me 40 minutes to get the answer~~
* 
* multi-threading solves it for ~15 minutes
* 
* try another solution? (e.g. reverse brute force?)
*/
struct SeedRange
{
	uint32_t start;
	uint32_t length;
};

class ThreadMinimalValue
{
public:
	ThreadMinimalValue() {};

	void push( uint32_t value )
	{
		std::lock_guard lock( _mutex );
		if ( value > _value ) return;

		_value = value;
	}

	uint32_t get() { return _value; }

private:
	uint32_t _value = std::numeric_limits<uint32_t>::max();
	std::mutex _mutex;
};

ThreadMinimalValue thread_value;

void thread_compute_seed_range_locations( int id, int size, const SeedRange& seed_range )
{
	for ( uint32_t seed = seed_range.start; seed < seed_range.start + seed_range.length; seed++ )
	{
		uint32_t value = seed;
		for ( const auto& process : processes )
		{
			const auto& ranges = almanac[process];
			for ( const auto& range : ranges )
			{
				if ( !range.is_within( value ) ) continue;

				value = range.remap( value );
				break;
			}

			//std::cout << seed << " : " << process << " : " << value << std::endl;
		}

		//  set the minimum location
		thread_value.push( value );

		//std::cout << seed << " : " << value << std::endl;
	}

	printf( "Thread %d/%d: done!\n", id + 1, size );
}

std::string part_two()
{
		//  open input
	std::ifstream file( "day5.txt" );

	const std::regex REGEX_SEED( "(\\d+) (\\d+)" );
	const std::regex REGEX_RANGE( "(\\d+) (\\d+) (\\d+)" );
	//					      destination source length
	const std::regex REGEX_MAP_NAME( "([\\w\\-]+) map:" );

	std::vector<SeedRange> seed_ranges;
	std::string current_map_name;

	//  gather symbols and numbers
	int line_id = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		int result = 0;

		if ( line.empty() )
		{
			current_map_name.clear();
			continue;
		}

		std::smatch match;
		if ( line_id == 0 )
		{
			//  find seeds
			std::string::const_iterator start_search = line.cbegin();
			while ( std::regex_search( start_search, line.cend(), match, REGEX_SEED ) )
			{
				start_search += match.position() + match[0].str().length();
				//std::cout << match[1].str() << std::endl;

				uint32_t start = str_to_uint32( match[1].str() );
				uint32_t length = str_to_uint32( match[2].str() );
				seed_ranges.push_back( SeedRange { start, length } );
			}
		}
		else if ( current_map_name.empty() )
		{
			std::regex_search( line, match, REGEX_MAP_NAME );
			current_map_name = match[1].str();
			almanac[current_map_name] = std::vector<MapRange>();
		}
		else 
		{
			std::regex_search( line, match, REGEX_RANGE );

			uint32_t destination_start = str_to_uint32( match[1].str() );
			uint32_t source_start = str_to_uint32( match[2].str() );
			uint32_t length = str_to_uint32( match[3].str() );
			almanac[current_map_name].push_back( MapRange { destination_start, source_start, length } );

			//  map values
			/*for ( int i = 0; i < length; i++ )
			{
				almanac[current_map_name][start_source + i] = start_destination + i;
			}*/
		}
		
		line_id++;
		//std::cout << line << " | " << current_map_name << std::endl;
	}

	uint32_t minimum_location = std::numeric_limits<uint32_t>().max();
	//uint32_t test = str_to_uint32( "4294967295" );

	std::vector<std::thread> threads;

	//  convert seeds to location
	int size = seed_ranges.size();
	for ( int i = 0; i < size; i++ ) 
	{
		const auto& seed_range = seed_ranges[i];
		threads.emplace_back( 
			thread_compute_seed_range_locations,
			i, size,
			seed_range
		);
	}

	for ( auto& thread : threads )
	{
		thread.join();
	}

	minimum_location = thread_value.get();

	return std::to_string( minimum_location );
}