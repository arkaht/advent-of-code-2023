#pragma once

#include "common.h"

/**
* common
*/
struct Node
{
	std::string left;
	std::string right;
};


/**
* part one
*/
std::string part_one()
{
	//  open input
	std::ifstream file = read_input( "day8.txt" );

	const std::regex REGEX_NODE( "(\\w+) = \\((\\w+), (\\w+)\\)" );

	std::unordered_map<std::string, Node> network;

	//  read instructions
	std::string instructions;
	std::getline( file, instructions );

	//  gather symbols and numbers
	std::string line;
	while ( std::getline( file, line ) )
	{
		if ( line.empty() ) continue;

		std::smatch match;
		std::regex_search( line, match, REGEX_NODE );

		std::string label = match[1].str();

		Node node {};
		node.left = match[2].str();
		node.right = match[3].str();
		network[label] = node;
	}

	//  compute solution
	std::string current_node_label = "AAA";
	int steps = 0;
	while ( true )
	{
		const Node& current_node = network[current_node_label];
		if ( current_node_label == "ZZZ" ) break;

		//  find next node
		char instruction = instructions[steps % instructions.size()];
		switch ( instruction )
		{
			case 'L':
				current_node_label = current_node.left;
				break;
			case 'R':
				current_node_label = current_node.right;
				break;
		}

		steps++;
	}

	return std::to_string( steps );
}

/**
* part two
*/
struct Path
{
	std::string current_node_label;
	int cycle_step = 0;
};

std::string part_two()
{
	//  open input
	std::ifstream file = read_input( "day8.txt" );

	const std::regex REGEX_NODE( "(\\w+) = \\((\\w+), (\\w+)\\)" );

	std::unordered_map<std::string, Node> network;
	std::vector<Path> paths;

	//  read instructions
	std::string instructions;
	std::getline( file, instructions );

	//  gather symbols and numbers
	std::string line;
	while ( std::getline( file, line ) )
	{
		if ( line.empty() ) continue;

		std::smatch match;
		std::regex_search( line, match, REGEX_NODE );

		std::string label = match[1].str();

		Node node {};
		node.left = match[2].str();
		node.right = match[3].str();
		network[label] = node;

		//  set as a starting pos if ending up with A
		if ( label[2] == 'A' )
		{
			Path path {};
			path.current_node_label = label;
			paths.push_back( path );
		}
	}

	std::cout << "Starting: " << std::endl;
	for ( const auto& path : paths )
	{
		std::cout << "- " << path.current_node_label << std::endl;
	}
	std::cout << std::endl;

	//std::cout << lcm( std::vector<uint64_t> { 2, 7, 3, 9, 4 } ) << std::endl;

	//  compute solution
	std::vector<uint64_t> cycles;
	int steps = 0;
	while ( true )
	{
		bool is_finished = true;

		//std::cout << "Step " << steps << ": ";
		char instruction = instructions[steps % instructions.size()];
		for ( int i = 0; i < paths.size(); i++ )
		{
			Path& path = paths[i];
			if ( path.current_node_label[2] != 'Z' ) 
			{
				is_finished = false;
			}
			else if ( path.cycle_step == 0 )
			{
				path.cycle_step = steps;
				cycles.push_back( steps );

				if ( cycles.size() == paths.size() )
				{
					is_finished = true;
					break;
				}
			}
			
			//std::cout << label << " ";
			const Node& current_node = network[path.current_node_label];

			//  find next node
			switch ( instruction )
			{
				case 'L':
					path.current_node_label = current_node.left;
					break;
				case 'R':
					path.current_node_label = current_node.right;
					break;
			}
		}
		//std::cout << std::endl;

		if ( is_finished ) break;
		steps++;
	}

	return std::to_string( lcm( cycles ) );
}