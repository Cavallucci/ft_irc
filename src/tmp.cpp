// Louis : fichier pour faire des tests
// c++ -Wall -Wextra -Werror -std=c++98 tmp.cpp && ./a.out "CMD mes arguments"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

typedef std::vector<std::string>			str_vec;


std::string		join_vec(str_vec vec, std::string glue)
{
	std::string		output;
	size_t			i = vec.size() - 1;
	for (str_vec::iterator it = vec.begin(); it != vec.end(); ++it, --i)
	{
		output.append(*it);
		if (i)
			output.append(glue);
	}
	return (output);
}


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
		std::string		test_join = join_vec(args, "_");
		std::cout << "test join_vec: [" << test_join << "]" << std::endl;
	}
	else
		std::cout << "Wrong number of arguments" << std::endl;
	return (0);
}
