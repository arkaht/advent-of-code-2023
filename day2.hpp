#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <regex>
#include <assert.h>

std::string part_one()
{
	std::unordered_map<std::string, int> max_objects_per_color
	{
		{ "red", 12 },
		{ "green", 13 },
		{ "blue", 14 },
	};

	const std::regex REGEX_GAME_ID( "Game\\s*(\\d+):\\s*" );
	const std::regex REGEX_SUBSET( "(\\d+) (\\w+)" );

	//  open input
	std::ifstream file( "day2.txt" );

	int sum = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		bool is_valid = true;
		std::smatch match;

		//  find game id
		std::regex_search( line, match, REGEX_GAME_ID );
		int game_id = std::atoi( match[1].str().c_str() );	

		//  find sets
		std::string line_sets = match.suffix();
		std::stringstream set_stream( line_sets );
		std::string set;
		while ( std::getline( set_stream, set, ';' ) )
		{
			//std::cout << " - " << set << std::endl;

			std::stringstream subset_stream( set );
			std::string subset;
			while ( std::getline( subset_stream, subset, ',' ) )
			{
				//std::cout << "   - " << subset << std::endl;

				std::regex_search( subset, match, REGEX_SUBSET );
				
				int count = std::atoi( match[1].str().c_str() );
				std::string color = match[2].str();

				if ( count > max_objects_per_color[color] )
				{
					is_valid = false;
					break;
				}
			}

			if ( !is_valid ) break;
		}

		//  sum up
		if ( is_valid )
		{
			sum += game_id;
		}

		std::cout << line << " => " << game_id << " | " << is_valid << std::endl;
		//break;
	}

	return std::to_string( sum );
}

std::string part_two()
{
	const std::regex REGEX_GAME_ID( "Game\\s*(\\d+):\\s*" );
	const std::regex REGEX_SUBSET( "(\\d+) (\\w+)" );

	//  open input
	std::ifstream file( "day2.txt" );

	int sum = 0;
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::smatch match;

		//  find game id
		std::regex_search( line, match, REGEX_GAME_ID );
		int game_id = std::atoi( match[1].str().c_str() );

		std::unordered_map<std::string, int> max_colors
		{
			{ "red", 0 },
			{ "green", 0 },
			{ "blue", 0 },
		};

		//  find sets
		std::string line_sets = match.suffix();
		std::stringstream set_stream( line_sets );
		std::string set;
		while ( std::getline( set_stream, set, ';' ) )
		{
			//std::cout << " - " << set << std::endl;

			std::stringstream subset_stream( set );
			std::string subset;
			while ( std::getline( subset_stream, subset, ',' ) )
			{
				//std::cout << "   - " << subset << std::endl;

				std::regex_search( subset, match, REGEX_SUBSET );
				
				int count = std::atoi( match[1].str().c_str() );
				std::string color = match[2].str();

				if ( count > max_colors[color] )
				{
					max_colors[color] = count;
				}
			}
		}

		//  sum up
		int power = max_colors["red"] * max_colors["green"] * max_colors["blue"];
		sum += power;

		std::cout << line << " => " << power << std::endl;
		//break;
	}

	return std::to_string( sum );
}