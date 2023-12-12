#pragma once

#include "common.h"

std::string part_one()
{
    //  open input
    std::ifstream file = read_input( "day1.txt" );

    int sum = 0;
    while ( file )
    {
        //  get input line
        std::string line;
        std::getline( file, line );
        if ( line.empty() ) continue;

        //  find digits
        std::string first_digit, last_digit;
        for ( auto letter : line )
        {
            if ( !std::isdigit( letter ) ) continue;

            //  append number
            if ( first_digit.empty() )
            {
                first_digit = last_digit = letter;
            }
            else
            {
                last_digit = letter;
            }
        }

        //  get calibration value
        std::string str_value = first_digit + last_digit;
        int value = std::atoi( str_value.c_str() );
        
        //  sum up
        sum += value;

        std::cout << line << " => " << value << std::endl;
        //break;
    }

    return std::to_string( sum );
}

std::string part_two()
{
    std::unordered_map<std::string, std::string> words_to_digits 
    {
        { "one",    "1" },
        { "two",    "2" },
        { "three",  "3" },
        { "four",   "4" },
        { "five",   "5" },
        { "six",    "6" },
        { "seven",  "7" },
        { "eight",  "8" },
        { "nine",   "9" },
    };

    std::string word_digit, first_digit, last_digit;
    auto append_digit = [&]( std::string digit ) 
    {
        if ( first_digit.empty() )
        {
            first_digit = last_digit = digit;
        }
        else
        {
            last_digit = digit;
        }
    };

    //  open input
    std::ifstream file = read_input( "day1.txt" );

    int sum = 0;
    while ( file )
    {
        //  get input line
        std::string line;
        std::getline( file, line );
        if ( line.empty() ) continue;

        //  clear digits
        word_digit.clear();
        first_digit.clear();
        last_digit.clear();

        //  find digits
        for ( auto letter : line )
        {
            if ( !std::isdigit( letter ) )
            {
                //  append letter to word
                word_digit += letter;
                //std::cout << word_digit << std::endl;

                //  search for equivalents
                for ( auto& pair : words_to_digits )
                {
                    auto position = word_digit.find( pair.first );
                    if ( position == std::string::npos ) continue;

                    append_digit( pair.second );
                    word_digit.erase( 0, position + pair.first.length() - 1 );

                    //std::cout << "found " << pair.first << " at " << position << std::endl;
                }

                //  search for equivalent
                /*auto itr = words_to_digits.find( word_digit );
                if ( itr != words_to_digits.end() )
                {
                    std::cout << word_digit << " => " << itr->second << std::endl;
                    append_digit( itr->second );
                    word_digit.clear();
                }*/

                continue;
            }

            //  append number
            append_digit( std::string( 1, letter ) );
            word_digit.clear();
        }

        //  get calibration value
        std::string str_value = first_digit + last_digit;
        int value = std::atoi( str_value.c_str() );
        
        //  sum up
        sum += value;

        std::cout << line << " => " << value << std::endl;
        //break;
    }

    return std::to_string( sum );
}