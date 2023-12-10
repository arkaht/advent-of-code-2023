#pragma once

#include "common.h"
#include <windows.h>

/**
* common
*/
enum class Cardinal : uint8_t
{
	None	=	0,
	North	=	1 << 0,
	East	=	1 << 1,
	South	=	1 << 2,
	West	=	1 << 3,
};
DEFINE_ENUM_FLAG_OPERATORS( Cardinal )  //  windows specific

using pipes_t = std::vector<std::vector<Cardinal>>;

Int2 get_cardinal_dir( Cardinal cardinal )
{
	switch ( cardinal )
	{
		case Cardinal::North:
			return Int2 { 0, -1 };
		case Cardinal::South:
			return Int2 { 0, 1 };
		case Cardinal::East:
			return Int2 { 1, 0 };
		case Cardinal::West:
			return Int2 { -1, 0 };
	}

	assert( false );
	return Int2 { 0, 0 };
}

Cardinal get_opposite_cardinal( Cardinal cardinal )
{
	switch ( cardinal )
	{
		case Cardinal::North:
			return Cardinal::South;
		case Cardinal::South:
			return Cardinal::North;
		case Cardinal::East:
			return Cardinal::West;
		case Cardinal::West:
			return Cardinal::East;
	}
}

bool can_travel_pipe( 
	const pipes_t& pipes, 
	const Int2& pos, 
	Cardinal cardinal
)
{
	//  check current position has a link to desired cardinal
	Cardinal current_cardinals = pipes[pos.y][pos.x];
	if ( ( current_cardinals & cardinal ) == Cardinal::None ) return false;

	//  check next position is valid
	Int2 dir = get_cardinal_dir( cardinal );
	if ( pos.y + dir.y < 0 || pos.y + dir.y >= pipes.size()
	  || pos.x + dir.x < 0 || pos.x + dir.x >= pipes[0].size() ) return false;

	//  check next position has a link from the current position
	Cardinal next_cardinals = pipes[pos.y + dir.y][pos.x + dir.x];
	if ( ( next_cardinals & get_opposite_cardinal( cardinal ) ) == Cardinal::None ) return false;

	return true;
}

/**
* part one
*/
namespace part1
{
	struct Walker
	{
		Int2 pos;
		Cardinal heading;
	};
}

std::string part_one()
{
	using namespace part1;

	//  open input
	std::ifstream file( "day10.txt" );

	pipes_t pipes;
	Int2 start_pos {};

	//  parse file
	int y = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::vector<Cardinal> row;
		row.resize( line.size() );

		for ( int x = 0; x < line.size(); x++ )
		{
			char letter = line[x];

			Cardinal cardinal = Cardinal::None;
			switch ( letter )
			{
				case '.':
					break;
				case '|':
					cardinal = Cardinal::North | Cardinal::South;
					break;
				case '-':
					cardinal = Cardinal::East | Cardinal::West;
					break;
				case 'L':
					cardinal = Cardinal::North | Cardinal::East;
					break;
				case 'J':
					cardinal = Cardinal::North | Cardinal::West;
					break;
				case '7':
					cardinal = Cardinal::South | Cardinal::West;
					break;
				case 'F':
					cardinal = Cardinal::South | Cardinal::East;
					break;
				case 'S':
					cardinal = Cardinal::North | Cardinal::South 
							 | Cardinal::East | Cardinal::West;
					start_pos.x = x, start_pos.y = y;
					break;
			}

			row[x] = cardinal;
		}

		pipes.push_back( row );
		y++;
	}

	std::cout << "Starting position: x=" << start_pos.x << "; y=" << start_pos.y << std::endl;

	std::vector<Cardinal> CARDINALS 
	{ 
		Cardinal::North, 
		Cardinal::South, 
		Cardinal::East, 
		Cardinal::West 
	};

	std::vector<Walker> walkers;
	for ( Cardinal cardinal : CARDINALS )
	{
		if ( !can_travel_pipe( pipes, start_pos, cardinal ) ) continue;

		Walker walker {};
		walker.pos = start_pos;
		walker.heading = cardinal;
		walkers.push_back( walker );
	}

	//  navigate and establish distances
	uint64_t steps = 0;
	while ( true )
	{
		//  update position & direction
		for ( auto& walker : walkers )
		{
			//  move
			Int2 dir = get_cardinal_dir( walker.heading );
			walker.pos.x += dir.x, walker.pos.y += dir.y;

			//  find next direction
			for ( Cardinal cardinal : CARDINALS )
			{
				if ( cardinal == get_opposite_cardinal( walker.heading ) ) continue;
				if ( !can_travel_pipe( pipes, walker.pos, cardinal ) ) continue;

				walker.heading = cardinal;
				break;
			}
		}

		//  check arrivals
		bool is_same_pos = true;
		for ( auto& walker : walkers )
		{
			if ( walker.pos == walkers[0].pos ) continue;
			
			is_same_pos = false;
			break;
		}

		steps++;
		if ( is_same_pos ) break;
	}

	return std::to_string( steps );
}

/**
* part two
*/
namespace part2
{
	struct Walker
	{
		Int2 pos;
		Cardinal heading;
		std::vector<Int2> vertices;
	};
}

std::string part_two()
{
	using namespace part2;

	//  open input
	std::ifstream file( "day10.txt" );

	pipes_t pipes;
	Int2 start_pos {};

	//  parse file
	int y = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::vector<Cardinal> row;
		row.resize( line.size() );

		for ( int x = 0; x < line.size(); x++ )
		{
			char letter = line[x];

			Cardinal cardinal = Cardinal::None;
			switch ( letter )
			{
				case '.':
					break;
				case '|':
					cardinal = Cardinal::North | Cardinal::South;
					break;
				case '-':
					cardinal = Cardinal::East | Cardinal::West;
					break;
				case 'L':
					cardinal = Cardinal::North | Cardinal::East;
					break;
				case 'J':
					cardinal = Cardinal::North | Cardinal::West;
					break;
				case '7':
					cardinal = Cardinal::South | Cardinal::West;
					break;
				case 'F':
					cardinal = Cardinal::South | Cardinal::East;
					break;
				case 'S':
					cardinal = Cardinal::North | Cardinal::South 
							 | Cardinal::East | Cardinal::West;
					start_pos.x = x, start_pos.y = y;
					break;
			}

			row[x] = cardinal;
		}

		pipes.push_back( row );
		y++;
	}

	std::cout << "Starting position: x=" << start_pos.x << "; y=" << start_pos.y << std::endl;

	std::vector<Cardinal> CARDINALS 
	{ 
		Cardinal::North, 
		Cardinal::South, 
		Cardinal::East, 
		Cardinal::West 
	};

	//  setup walkers
	Cardinal start_cardinal = Cardinal::None;
	std::vector<Walker> walkers;
	for ( Cardinal cardinal : CARDINALS )
	{
		if ( !can_travel_pipe( pipes, start_pos, cardinal ) ) continue;

		Walker walker {};
		walker.pos = start_pos;
		walker.heading = cardinal;
		walkers.push_back( walker );

		start_cardinal |= cardinal;
	}
	pipes[start_pos.y][start_pos.x] = start_cardinal;

	//  navigate and establish main-loop
	int steps = 0;
	while ( true )
	{
		//  update position & direction
		for ( auto& walker : walkers )
		{
			//filtered_pipes[walker.pos.y][walker.pos.y] = pipes[walker.pos.y][walker.pos.x];

			//  move
			Int2 dir = get_cardinal_dir( walker.heading );
			walker.pos.x += dir.x, walker.pos.y += dir.y;

			//  find next direction
			for ( Cardinal cardinal : CARDINALS )
			{
				if ( cardinal == get_opposite_cardinal( walker.heading ) ) continue;
				if ( !can_travel_pipe( pipes, walker.pos, cardinal ) ) continue;

				if ( cardinal != walker.heading )
				{
					walker.vertices.push_back( walker.pos );
					walker.heading = cardinal;
				}
				break;
			}
		}

		//  check arrivals
		bool is_same_pos = true;
		for ( auto& walker : walkers )
		{
			if ( walker.pos == walkers[0].pos ) continue;
			
			is_same_pos = false;
			break;
		}

		steps++;
		if ( is_same_pos ) break;
	}

	//  algorithm from (haven't looked into his code, only the theory):
	//  https://www.reddit.com/r/adventofcode/comments/18ey1s7/comment/kcrl36f/?utm_source=share&utm_medium=web3x&utm_name=web3xcss&utm_term=1&utm_content=share_button

	//  establish polygon
	std::vector<Int2> polygon;
	polygon.push_back( start_pos );

	//  insert first walker vertices
	polygon.insert( polygon.end(), walkers[0].vertices.begin(), walkers[0].vertices.end() );

	//  insert second walker vertices in reversed order
	const auto& vertices = walkers[1].vertices;
	for ( int i = vertices.size() - 1; i >= 0; i-- )
	{
		const auto& vertex = vertices[i];
		polygon.push_back( vertex );
	}

	//  shoelace algorithm
	int area = shoelace_area( polygon );
	//std::cout << shoelace_area( { { 2, 7 }, { 10, 1 }, { 8, 6 }, { 11, 7 }, { 7, 10 } } ) << std::endl;

	return std::to_string( area - steps + 1 );
}