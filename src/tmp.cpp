// Louis : fichier pour faire des tests
// c++ -Wall -Wextra -Werror -std=c++98 tmp.cpp && ./a.out "CMD mes arguments"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		std::string input = argv[1];
        // input.append("\r\n");
		std::string::size_type pos = input.find(' '); // might be string::npos
		std::string cmd_str = input.substr(0, pos);
		std::string arg;
		std::vector<std::string>	args;
		if (input.length() != cmd_str.length())
		{
			arg = input.substr(pos + 1, std::string::npos);
			std::stringstream			arg_stream(arg);
			std::string					tmp;
			while (getline(arg_stream, tmp, ' '))
				args.push_back(tmp);
		}
		std::cout << "command: [" << cmd_str << "]" << std::endl;
		std::cout << "args: [";

		for (std::vector<std::string>::const_iterator i = args.begin(); i != args.end(); ++i)
			std::cout << *i << std::endl;
		std::cout << "]" << std::endl;
		std::cout << "mon test: [" << args[0] << "]" << std::endl;
	}
	else
		std::cout << "Wrong number of arguments" << std::endl;
	return (0);
}
