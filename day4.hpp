#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <regex>
#include <assert.h>

/**
* common
*/


/**
* part one
*/

std::string part_one()
{
	//  open input
	std::ifstream file( "day4.txt" );

	const std::regex REGEX_CARD_ID( "Card\\s*(\\d+): " );
	const std::regex REGEX_NUMBER( "([\\d|]+)" );

	int sum = 0;
	std::unordered_set<int> winning_numbers;

	//  gather symbols and numbers
	std::string line;
	while ( std::getline( file, line ) )
	{
		int result = 0;
		winning_numbers.clear();

		std::smatch match;
		std::regex_search( line, match, REGEX_CARD_ID );
		int card_id = std::atoi( match[1].str().c_str() );

		//  find sets
		bool is_winning_numbers = true;
		std::string line_sets = match.suffix();
		std::stringstream set_stream( line_sets );
		std::string set;
		while ( std::getline( set_stream, set, ' ' ) )
		{
			if ( auto number = std::atoi( set.c_str() ) ) 
			{
				if ( is_winning_numbers )
				{
					winning_numbers.insert( number );
					continue;
				}

				if ( winning_numbers.find( number ) == winning_numbers.end() ) continue;
				
				result = result == 0 ? 1 : result * 2;
				continue;
			}

			if ( set == "|" )
			{
				is_winning_numbers = false;
			}
		}

		sum += result;
		std::cout << card_id << ": " << match.suffix() << " => " << result << std::endl;
	}

	return std::to_string( sum );
}

/**
* part two
*/

std::string part_two()
{
	//  open input
	std::ifstream file( "day4.txt" );

	const std::regex REGEX_CARD_ID( "Card\\s*(\\d+): " );
	const std::regex REGEX_NUMBER( "([\\d|]+)" );

	int sum = 0;
	std::unordered_set<int> winning_numbers;
	std::unordered_map<int, int> cards_counts;

	//  reserve map
	for ( int i = 0; i < 300; i++ )
	{
		cards_counts[i] = 0;
	}

	//  gather symbols and numbers
	std::string line;
	while ( std::getline( file, line ) )
	{
		int matching_numbers = 0;
		winning_numbers.clear();

		//  get card id
		std::smatch match;
		std::regex_search( line, match, REGEX_CARD_ID );
		int card_id = std::atoi( match[1].str().c_str() );

		//  add original to count
		cards_counts[card_id]++;

		//  find numbers
		bool is_winning_numbers = true;
		std::string line_sets = match.suffix();
		std::stringstream set_stream( line_sets );
		std::string set;
		while ( std::getline( set_stream, set, ' ' ) )
		{
			if ( auto number = std::atoi( set.c_str() ) ) 
			{
				if ( is_winning_numbers )
				{
					winning_numbers.insert( number );
					continue;
				}

				if ( winning_numbers.find( number ) == winning_numbers.end() ) continue;
				
				matching_numbers++;
				continue;
			}

			if ( set == "|" )
			{
				is_winning_numbers = false;
			}
		}

		//  adding copies
		for ( int i = 0; i < matching_numbers; i++ )
		{
			int id = card_id + i + 1;
			cards_counts[id] += cards_counts[card_id];
			//std::cout << "+" << i << std::endl;
		}

		//  sum up
		sum += cards_counts[card_id];

		std::cout << card_id << ": " << match.suffix() << " => " << matching_numbers << std::endl;
	}

	return std::to_string( sum );
}