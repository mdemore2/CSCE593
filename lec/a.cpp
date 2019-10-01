#include <iostream>

int main( int argc, char* argv[] )
{
    std::cout<<"Hello World! \n";

    std::cout<<"argc count is "<<argc<<"\n";

    std::vector<std::string>args( argv, argv + argc );

    for(auto& s : args)
    {
        std::cout << s<<"\n";
    }

    std::cout<<"\n";
    for( int i = 0; i <args.size(); ++i)
    {
        std::cout<<args[i]<<"\n";
    }
}