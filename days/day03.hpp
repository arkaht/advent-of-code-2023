#pragma once

#include "common.h"

/**
* common
*/
struct Int2
{
	int x;
	int y;
};

/**
* part one
*/
struct Number
{
	int number;
	Int2 region_start;
	Int2 region_end;
};

bool is_aabb( const Int2& start, const Int2& end, const Int2 point )
{
	return point.x >= start.x 
		&& point.x <= end.x
		&& point.y >= start.y
		&& point.y <= end.y;
}

std::string part_one()
{
	//  open input
	std::ifstream file = read_input( "day3.txt" );

	std::vector<Int2> symbols;
	std::vector<Number> numbers;

	//  gather symbols and numbers
	int y = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::cout << y << ": " << line << std::endl;

		std::string str_number;
		Int2 region_start_number { 0, 0 };
		for ( int x = 0; x < line.size(); x++ )
		{
			char letter = line[x];

			//  constructing number
			if ( std::isdigit( letter ) )
			{
				//  setup start region
				if ( str_number.empty() )
				{
					region_start_number.x = x - 1;
					region_start_number.y = y - 1;
				}

				str_number += letter;
				continue;
			}

			//  append number
			if ( !str_number.empty() )
			{
				numbers.push_back( 
					Number {
						std::atoi( str_number.c_str() ),
						region_start_number,
						Int2 { x, y + 1 }
					}
				);
				str_number.clear();
			}
			
			//  append symbols
			if ( letter != '.' )
			{
				symbols.push_back( Int2 { x, y } );
			}
		}

		//  append number
		if ( !str_number.empty() )
		{
			numbers.push_back( 
				Number {
					std::atoi( str_number.c_str() ),
					region_start_number,
					Int2 { (int)line.size(), y + 1 }
				}
			);
		}

		y++;
	}

	//  summing up all collisions between symbols and numbers
	int sum = 0;
	for ( auto& number : numbers )
	{
		for ( auto& symbol_point : symbols )
		{
			if ( !is_aabb( number.region_start, number.region_end, symbol_point ) ) continue;
			
			sum += number.number;
		}
	}

	return std::to_string( sum );
}

/**
* part two
*/
struct Gear
{
	Int2 point;
	Number* part1 { nullptr };
	Number* part2 { nullptr };
};

std::string part_two()
{
	//  open input
	std::ifstream file = read_input( "day3.txt" );

	std::vector<Gear> gears;
	std::vector<Number> numbers;

	//  gather symbols and numbers
	int y = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::cout << y << ": " << line << std::endl;

		std::string str_number;
		Int2 region_start_number { 0, 0 };
		for ( int x = 0; x < line.size(); x++ )
		{
			char letter = line[x];

			//  constructing number
			if ( std::isdigit( letter ) )
			{
				//  setup start region
				if ( str_number.empty() )
				{
					region_start_number.x = x - 1;
					region_start_number.y = y - 1;
				}

				str_number += letter;
				continue;
			}

			//  append number
			if ( !str_number.empty() )
			{
				numbers.push_back( 
					Number {
						std::atoi( str_number.c_str() ),
						region_start_number,
						Int2 { x, y + 1 }
					}
				);
				str_number.clear();
			}
			
			//  append symbols
			if ( letter == '*' )
			{
				gears.push_back( 
					Gear {
						Int2 { x, y },
						nullptr,
						nullptr
					}
				);
			}
		}

		//  append number
		if ( !str_number.empty() )
		{
			numbers.push_back( 
				Number {
					std::atoi( str_number.c_str() ),
					region_start_number,
					Int2 { (int)line.size(), y + 1 }
				}
			);
		}

		y++;
	}

	//  summing up all collisions between gears and numbers
	int sum = 0;
	for ( auto& number : numbers )
	{
		for ( auto& gear : gears )
		{
			if ( gear.part1 && gear.part2 ) continue;
			if ( !is_aabb( number.region_start, number.region_end, gear.point ) ) continue;
			
			if ( gear.part1 == nullptr )
			{
				gear.part1 = &number;
			}
			else
			{
				gear.part2 = &number;
				sum += gear.part1->number * number.number;
			}
		}
	}

	return std::to_string( sum );
}