
#ifndef COMMON_HPP
#define COMMON_HPP

/* Contains handy functions for testing stuff when debugging */

// TODO: All of these are currently defined as inline
// to make compiler not whine about multiple definitions.
// Turn this into the appropriate Common.hpp Common.cpp duo

#define TEST_ASSERTS
#define TEST_PRINTS

#include <iostream>
#include <string>
#include <cstdlib>  // abort()

/////////////////////////////////////////////////////////
inline                                                 //
void program_assert(const bool conditional_statement)  //
{                                                      //
    #ifdef TEST_ASSERTS                                //
    if (!conditional_statement)                        //
        abort();                                       //
    #endif                                             //
}                                                      //
inline                                                 //
void program_assert(const bool conditional_statement,  //
                    const std::string& failure_msg)    //
{                                                      //
    #ifdef TEST_ASSERTS                                //
    if (!conditional_statement)                        //
    {                                                  //
        std::cout << failure_msg << std::endl;         //
        abort();                                       //
    }                                                  //
    #endif                                             //
}                                                      //
/////////////////////////////////////////////////////////
// have a variadic function?
// http://stackoverflow.com/questions/15240/how-do-you-create-a-debug-only-function-that-takes-a-variable-argument-list-lik
inline
void debug_print(const char* statement)
{
    #ifdef TEST_PRINTS
    std::cout << statement << std::endl;
    #endif
}
inline
void debug_print(const char* statement, int n)
{
    #ifdef TEST_PRINTS
    std::cout << statement << " " << n << std::endl;
    #endif
}

#include <iostream>
#include <limits>

#define PressEnterToContinue wait
inline
void PressEnterToContinue(const char* description = "")
{
    std::cout << description << ".." << std::endl;
    std::cout << "Press Enter to continue... " << std::flush;
    std::cin.ignore( std::numeric_limits <std::streamsize> ::max(), '\n' );
    std::cout << std::endl;
}

#endif // COMMON_HPP
