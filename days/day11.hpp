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
	std::ifstream file = read_input( "day11.txt" );

	std::vector<Int2> galaxies;
	std::vector<int> to_expand_rows;
	std::unordered_set<int> to_expand_columns_set;

	//  parse file
	int y = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		bool found_a_galaxy = false;

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

		//  set row as expandable if no galaxy was found
		if ( !found_a_galaxy )
		{
			to_expand_rows.push_back( y );
		}

		y++;
	}

	//  expand vertically
	for ( int i = 0; i < to_expand_rows.size(); i++ )
	{
		for ( auto& galaxy : galaxies )
		{
			if ( galaxy.y < to_expand_rows[i] + i ) continue;
			galaxy.y++;
		}
	}

	//  build expanding columns vector
	std::vector<int> to_expand_columns;
	to_expand_columns.insert( 
		to_expand_columns.begin(), 
		to_expand_columns_set.begin(), 
		to_expand_columns_set.end() 
	);
	std::sort( to_expand_columns.begin(), to_expand_columns.end() );

	//  expand horizontally
	for ( int i = 0; i < to_expand_columns.size(); i++ )
	{
		for ( auto& galaxy : galaxies )
		{
			if ( galaxy.x < to_expand_columns[i] + i ) continue;
			galaxy.x++;
		}
	}

	int pairs_count = 0;
	int sum = 0;
	int galaxies_size = galaxies.size();
	for ( int a = 0; a < galaxies_size; a++ )
	{
		const Int2& galaxy_a = galaxies[a];
		for ( int b = a + 1; b < galaxies_size; b++ )
		{
			const Int2& galaxy_b = galaxies[b];
			sum += manhattan_distance( galaxy_a, galaxy_b );
			pairs_count++;
		}
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
using custom_int = int64_t;

struct Int64_2D
{
	custom_int x;
	custom_int y;
};

struct Galaxy
{
	Int2 original_pos;
	Int64_2D pos;
};

custom_int manhattan_distance( const Int64_2D& a, const Int64_2D& b )
{
	return abs( a.x - b.x ) + abs( a.y - b.y );
}

std::string part_two()
{
	//  open input
	std::ifstream file = read_input( "day11.txt" );

	std::vector<Galaxy> galaxies;
	std::vector<int> to_expand_rows;
	std::unordered_set<int> to_expand_columns_set;

	//  parse file
	int y = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		bool found_a_galaxy = false;

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
					galaxies.push_back( 
						Galaxy {
							Int2 { x, y },
							Int64_2D { x, y } 
						}
					);
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

		//  set row as expandable if no galaxy was found
		if ( !found_a_galaxy )
		{
			to_expand_rows.push_back( y );
		}

		y++;
	}

	const int GAP_SPACE = 999999;  //  x1.000.000; yea.. so it's actually the number minus one for some reasons..

	//  expand vertically
	for ( custom_int i = 0; i < to_expand_rows.size(); i++ )
	{
		for ( auto& galaxy : galaxies )
		{
			if ( galaxy.original_pos.y < to_expand_rows[i] ) continue;
			galaxy.pos.y += GAP_SPACE;
		}
	}

	//  expand horizontally
	for ( int column : to_expand_columns_set )
	{
		for ( auto& galaxy : galaxies )
		{
			if ( galaxy.original_pos.x < column ) continue;
			galaxy.pos.x += GAP_SPACE;
		}
	}

	int pairs_count = 0;
	custom_int sum = 0;
	int galaxies_size = galaxies.size();
	for ( int a = 0; a < galaxies_size; a++ )
	{
		const auto& galaxy_a = galaxies[a];
		for ( int b = a + 1; b < galaxies_size; b++ )
		{
			const auto& galaxy_b = galaxies[b];
			sum += manhattan_distance( galaxy_a.pos, galaxy_b.pos );
			pairs_count++;
		}
	}

	/*std::cout << manhattan_distance( galaxies[0], galaxies[6] ) << std::endl;
	std::cout << manhattan_distance( galaxies[2], galaxies[5] ) << std::endl;
	std::cout << manhattan_distance( galaxies[7], galaxies[8] ) << std::endl;
	std::cout << manhattan_distance( galaxies[4], galaxies[8] ) << std::endl;*/

	printf( "Pairs count: %d\n", pairs_count );
	return std::to_string( sum );
}