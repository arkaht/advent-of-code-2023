#pragma once

#include "common.h"

/**
* common
*/

/**
* part one
*/
std::string part_one()
{
	//  open input
	std::ifstream file( "day9.txt" );

	const std::regex REGEX_NUMBER( "(\\-?\\d+)" );

	using num_t = int;
	std::vector<std::vector<num_t>> histories;

	//  parse file
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::smatch match;

		std::vector<num_t> history;

		//  find numbers
		std::string::const_iterator start_search = line.cbegin();
		while ( std::regex_search( start_search, line.cend(), match, REGEX_NUMBER ) )
		{
			start_search += match.position() + match[0].str().length();
			//std::cout << match[1].str() << std::endl;

			num_t number = str_to_int( match[1].str() );
			history.push_back( number );
			std::cout << number << " ";
		}

		histories.push_back( history );
		std::cout << std::endl;
	}

	num_t sum = 0;
	for ( const auto& history : histories )
	{
		std::vector<std::vector<num_t>> sequences;
		sequences.push_back( history );

		std::vector<num_t> last_sequence = history;
		std::vector<num_t> sequence;
		while ( true )
		{
			sequence.resize( last_sequence.size() - 1 );

			bool is_zeroes = true;

			num_t last_value = last_sequence[0];
			for ( int i = 1; i < last_sequence.size(); i++ )
			{
				num_t value = last_sequence[i];
				num_t diff = value - last_value;
				sequence[(size_t)(i - 1)] = diff;

				if ( diff != 0 )
				{
					is_zeroes = false;
				}

				last_value = value;
			}

			if ( is_zeroes ) break;

			sequences.push_back( sequence );
			last_sequence = sequence;
		}

		//  predicting the next value
		num_t last_prediction = 0;
		int sequences_size = sequences.size();
		for ( int i = sequences_size - 1; i >= 0; i-- )
		{
			const auto& sequence = sequences[i];
			last_prediction += sequence[sequence.size() - 1];

			/*std::cout << "Sequence " << i << ": ";
			
			for ( num_t value : sequence )
			{
				std::cout << value << " ";
			}

			std::cout << std::endl;*/
		}

		std::cout << last_prediction << std::endl;
		sum += last_prediction;
		//break;
	}

	return std::to_string( sum );
}

/**
* part two
*/
std::string part_two()
{
	//  open input
	std::ifstream file( "day9.txt" );

	const std::regex REGEX_NUMBER( "(\\-?\\d+)" );

	using num_t = int;
	std::vector<std::vector<num_t>> histories;

	//  parse file
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::smatch match;

		std::vector<num_t> history;

		//  find numbers
		std::string::const_iterator start_search = line.cbegin();
		while ( std::regex_search( start_search, line.cend(), match, REGEX_NUMBER ) )
		{
			start_search += match.position() + match[0].str().length();
			//std::cout << match[1].str() << std::endl;

			num_t number = str_to_int( match[1].str() );
			history.push_back( number );
			//std::cout << number << " ";
		}

		histories.push_back( history );
		//std::cout << std::endl;
	}

	num_t sum = 0;
	for ( const auto& history : histories )
	{
		std::vector<std::vector<num_t>> sequences;
		sequences.push_back( history );

		std::vector<num_t> last_sequence = history;
		std::vector<num_t> sequence;
		while ( true )
		{
			sequence.resize( last_sequence.size() - 1 );

			bool is_zeroes = true;

			num_t last_value = last_sequence[0];
			for ( int i = 1; i < last_sequence.size(); i++ )
			{
				num_t value = last_sequence[i];
				num_t diff = value - last_value;
				sequence[(size_t)(i - 1)] = diff;

				if ( diff != 0 )
				{
					is_zeroes = false;
				}

				last_value = value;
			}

			if ( is_zeroes ) break;

			sequences.push_back( sequence );
			last_sequence = sequence;
		}

		//  predicting the last value
		num_t last_prediction = 0;
		int sequences_size = sequences.size();
		for ( int i = sequences_size - 1; i >= 0; i-- )
		{
			const auto& sequence = sequences[i];
			last_prediction = sequence[0] - last_prediction;

			std::cout << "Sequence " << i << ": ";
			
			std::cout << "(" << last_prediction << ") ";
			for ( num_t value : sequence )
			{
				std::cout << value << " ";
			}

			std::cout << std::endl;
		}

		std::cout << last_prediction << std::endl;
		sum += last_prediction;
		//break;
	}

	return std::to_string( sum );
}