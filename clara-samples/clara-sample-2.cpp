// --------------------------------------------------------------------------
// Copyright  (c) Dipl.-Ing. Joerg Desch <github@jdesch.de>
// --------------------------------------------------------------------------
// AUTHOR.: Joerg Desch
// LICENSE: GPL 3.0
// --------------------------------------------------------------------------
//
// [Clara](https://github.com/catchorg/Clara) example with `struct` and build method
//
// Save this snippet as `clara-sample-2.cpp` together with the simgle-header
// verion of Clara and call `g++` as shown below.
//
// g++ -Wall -std=c++11 -o clara-sample-2 clara-sample-2.cpp

#include <string>
#include <iostream>

#include "clara.hpp"


// "Hide" all settings and the creation of the Clara parser.
struct MyOptions
{
    // dont show the help by default. Use `-h or `-?` to enable it.
    bool showHelp = false;

    // this block of variables are changed by Clara with the corresponding
    // options passed to the program.
    std::string processName;
    int width = 0;
    std::string name;
    bool doIt = false;
    std::string command;

    // special value which is set indirectly by a lamba function!
    int index = 0;

    // create a Clara parser with all the options available.
    auto makeCli() -> clara::detail::Parser
    {
        return clara::detail::ExeName( processName )
             | clara::detail::Help(showHelp)
             | clara::detail::Opt( width, "width" )["-w"]["--width"]("How wide should it be?")
             | clara::detail::Opt( name, "name" )["-n"]["--name"]("By what name should I be known")
             | clara::detail::Opt( doIt )["-d"]["--doit"]("Do the thing" )
             | clara::detail::Opt( [&]( int i )
                  {
                    if (i < 0 || i > 10)
                        return clara::detail::ParserResult::runtimeError("index must be between 0 and 10");
                    else {
                        index = i;
                        return clara::detail::ParserResult::ok( clara::detail::ParseResultType::Matched );
                    }
                  }, "index" )
                  ["-i"]( "An index, which is an integer between 0 and 10, inclusive" )
             | clara::detail::Arg( command, "command" )("which command to run").required();
    }
};



MyOptions options;


int main ( int argc, char** argv )
{
    auto cli = options.makeCli();
    auto result = cli.parse( clara::detail::Args( argc, argv ) );
    if( !result )
    {
	std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
	return 1;
    }

    if ( options.showHelp )
    {
	std::cerr << cli << std::endl;
        return 0;
    }

    // show the results!
    std::cerr << "Show Help:" << options.showHelp << std::endl;
    std::cerr << "Process:" << options.processName << std::endl;
    std::cerr << "Index:" << options.index << std::endl;
    std::cerr << "Width:" << options.width << std::endl;
    std::cerr << "Name:" << options.name << std::endl;
    std::cerr << "Doit:" << options.doIt << std::endl;
    std::cerr << "Command:" << options.command << std::endl;

    return 0;
}
