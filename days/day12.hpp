#pragma once

#include "common.h"
#include <windows.h>

/**
* common
*/

/**
* part one
*/
std::string part_one()
{
	//  open input
	std::ifstream file = read_input( "day12.txt" );

	//  parse file
	int y = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		int x;
		for ( x = 0; x < line.size(); x++ )
		{
			char letter = line[x];

			switch ( letter )
			{
				case '.':
					//  set column as expandable if initializing
					if ( y == 0 )
					{
						to_expand_columns_set.emplace( x );
					}
					break;
				case '#':
					galaxies.push_back( Int2 { x, y } );
					found_a_galaxy = true;

					//  remove column from being expandable
					auto itr = to_expand_columns_set.find( x );
					if ( itr != to_expand_columns_set.end() ) 
					{
						to_expand_columns_set.erase( itr );
					}
					break;
			}
		}

		y++;
	}

	/*std::cout << manhattan_distance( galaxies[0], galaxies[6] ) << std::endl;
	std::cout << manhattan_distance( galaxies[2], galaxies[5] ) << std::endl;
	std::cout << manhattan_distance( galaxies[7], galaxies[8] ) << std::endl;
	std::cout << manhattan_distance( galaxies[4], galaxies[8] ) << std::endl;*/

	printf( "Pairs count: %d\n", pairs_count );
	return std::to_string( sum );
}

/**
* part two
*/
using custom_int = int;

std::string part_two()
{
	//  open input
	std::ifstream file = read_input( "day11.txt" );

	return std::to_string( 0 );
}