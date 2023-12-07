#pragma once

#include "common.h"


/**
* common
*/
enum class HandType
{
	None,
	FiveOfAKind,
	FourOfAKind,
	FullHouse,
	ThreeOfAKind,
	TwoPair,
	OnePair,
	HighCard,
};

template <typename CardT>
CardT str_to_card( char letter )
{
	switch ( letter )
	{
		case 'A':
			return CardT::A;
		case 'K':
			return CardT::K;
		case 'Q':
			return CardT::Q;
		case 'J':
			return CardT::J;
		case 'T':
			return CardT::T;
		case '9':
			return CardT::Nine;
		case '8':
			return CardT::Eight;
		case '7':
			return CardT::Seven;
		case '6':
			return CardT::Six;
		case '5':
			return CardT::Five;
		case '4':
			return CardT::Four;
		case '3':
			return CardT::Three;
		case '2':
			return CardT::Two;
	}

	return CardT::None;
}

template <typename HandT, typename CardT>
struct HandSorter
{
	bool operator()( const HandT& a, const HandT& b )
	{
		//  compare hand type
		if ( a.type != b.type )
		{
			return a.type > b.type;
		}

		//  compare cards
		for ( int i = 0; i < 5; i++ )
		{
			CardT card_a = a.cards[i];
			CardT card_b = b.cards[i];
			if ( card_a == card_b ) continue;
			
			return card_a < card_b;
		}

		return false;
	}
};

/**
* part one
*/
namespace part1
{
	enum class Card
	{
		None,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,
		T,
		J,
		Q,
		K,
		A,
	};

	struct Hand
	{
		int bid = 0;
		std::vector<Card> cards;

		HandType type = HandType::None;

		void identify_type()
		{
			//  count cards by type
			std::unordered_map<Card, int> cards_counter;
			for ( const auto& card : cards )
			{
				//  init counter
				if ( cards_counter.find( card ) == cards_counter.end() )
				{
					cards_counter[card] = 0;
				}

				//  add to counter
				cards_counter[card]++;
			}

			//  identify type
			switch ( cards_counter.size() )
			{
				case 1:
					type = HandType::FiveOfAKind;
					break;
				case 2:
					type = HandType::FullHouse;
					for ( const auto& pair : cards_counter )
					{
						if ( pair.second == 4 )
						{
							type = HandType::FourOfAKind;
							break;
						}
					}
					break;
				//  either ThreeOfAKind or TwoPair
				case 3:
					type = HandType::TwoPair;
					for ( const auto& pair : cards_counter )
					{
						if ( pair.second == 3 )
						{
							type = HandType::ThreeOfAKind;
							break;
						}
					}
					break;
				case 4:
					type = HandType::OnePair;
					break;
				case 5:
					type = HandType::HighCard;
					break;
				default:
					assert(false);
					break;
			}
		}
	};
}

std::string part_one()
{
	using namespace part1;

	//  open input
	std::ifstream file( "day7.txt" );

	const std::regex REGEX_HAND( "(\\w+) (\\d+)" );

	std::vector<Hand> hands;

	//  gather symbols and numbers
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::smatch match;
		std::regex_search( line, match, REGEX_HAND );

		Hand hand {};

		//  set cards
		std::string str_cards = match[1].str();
		for ( char letter : str_cards )
		{
			Card card = str_to_card<Card>( letter );
			hand.cards.push_back( card );
		}
		hand.identify_type();

		//  set bid
		hand.bid = str_to_int( match[2].str() );

		hands.push_back( hand );
	}

	using custom_int_t = uint32_t;
	
	//  rank hands
	std::sort( hands.begin(), hands.end(), HandSorter<Hand, Card>() );

	//  compute solution
	custom_int_t solution = 0;
	for ( int i = 0; i < hands.size(); i++ )
	{
		const auto& hand = hands[i];
		int rank = i + 1;
		solution += hand.bid * rank;

		std::cout << "Rank " << rank << ": " << hand.bid << std::endl;
	}

	return std::to_string( solution );
}

/**
* part two
*/
namespace part2
{
	enum class Card
	{
		None,
		J,
		Two,
		Three,
		Four,
		Five,
		Six,
		Seven,
		Eight,
		Nine,
		T,
		Q,
		K,
		A,
	};

	struct Hand
	{
		int bid = 0;
		std::vector<Card> cards;

		HandType type = HandType::None;

		void identify_type()
		{
			//  count cards by type
			std::unordered_map<Card, int> cards_counter;
			for ( const auto& card : cards )
			{
				//  init counter
				if ( cards_counter.find( card ) == cards_counter.end() )
				{
					cards_counter[card] = 0;
				}

				//  add to counter
				cards_counter[card]++;
			}

			//  alter counter w/ jokers
			if ( cards_counter.find( Card::J ) != cards_counter.end() )
			{
				//  get the best card to fake
				Card faking_card = Card::None;
				int faking_count = 0;
				for ( const auto& pair : cards_counter )
				{
					if ( pair.first == Card::J ) continue;
					if ( pair.second < faking_count ) continue;

					faking_card = pair.first;
					faking_count = pair.second;
				}

				//  alter counting
				int jokers_count = cards_counter[Card::J];
				cards_counter.erase( Card::J );
				cards_counter[faking_card] += jokers_count;
			}

			//  identify type
			switch ( cards_counter.size() )
			{
				case 1:
					type = HandType::FiveOfAKind;
					break;
				case 2:
					type = HandType::FullHouse;
					for ( const auto& pair : cards_counter )
					{
						if ( pair.second == 4 )
						{
							type = HandType::FourOfAKind;
							break;
						}
					}
					break;
				//  either ThreeOfAKind or TwoPair
				case 3:
					type = HandType::TwoPair;
					for ( const auto& pair : cards_counter )
					{
						if ( pair.second == 3 )
						{
							type = HandType::ThreeOfAKind;
							break;
						}
					}
					break;
				case 4:
					type = HandType::OnePair;
					break;
				case 5:
					type = HandType::HighCard;
					break;
				default:
					assert(false);
					break;
			}
		}
	};
}

std::string part_two()
{
	using namespace part2;

	//  open input
	std::ifstream file( "day7.txt" );

	const std::regex REGEX_HAND( "(\\w+) (\\d+)" );

	std::vector<Hand> hands;

	//  gather symbols and numbers
	std::string line;
	while ( std::getline( file, line ) )
	{
		std::smatch match;
		std::regex_search( line, match, REGEX_HAND );

		Hand hand {};

		//  set cards
		std::string str_cards = match[1].str();
		for ( char letter : str_cards )
		{
			Card card = str_to_card<Card>( letter );
			hand.cards.push_back( card );
		}
		hand.identify_type();

		//  set bid
		hand.bid = str_to_int( match[2].str() );

		hands.push_back( hand );
	}

	using custom_int_t = uint32_t;
	
	//  rank hands
	std::sort( hands.begin(), hands.end(), HandSorter<Hand, Card>() );

	//  compute solution
	custom_int_t solution = 0;
	for ( int i = 0; i < hands.size(); i++ )
	{
		const auto& hand = hands[i];
		int rank = i + 1;
		solution += hand.bid * rank;

		std::cout << "Rank " << rank << ": " << hand.bid << std::endl;
	}

	return std::to_string( solution );
}