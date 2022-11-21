// Louis : fichier pour faire des tests
// c++ -Wall -Wextra -Werror -std=c++98 tmp.cpp && ./a.out "CMD mes arguments"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

typedef std::vector<std::string>			str_vec;

# define ALLOWED_CHAR_IN_NICK					"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_[]{}\\`|"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		std::string input = argv[1];
        // input.append("\r\n");
		std::string::size_type pos = input.find(' '); // might be string::npos
		std::string cmd_str = input.substr(0, pos);
		std::string arg;
		str_vec		args;
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
		for (str_vec::const_iterator i = args.begin(); i != args.end(); ++i)
			std::cout << *i << std::endl;
		std::cout << "]" << std::endl;
		if (arg.find_first_not_of(ALLOWED_CHAR_IN_NICK) != std::string::npos)
			std::cout << "Invalid char found!" << std::endl;
	}
	else
		std::cout << "Wrong number of arguments" << std::endl;
	return (0);
}
