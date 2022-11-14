// Louis : fichier pour faire des tests
// c++ -Wall -Wextra -Werror -std=c++98 tmp.cpp && ./a.out "CMD mes arguments"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

typedef std::vector<std::string>			str_vec;

class Server
{
	public:
		str_vec		getTest() const
		{
			str_vec		test;
			test.push_back("hello");
			test.push_back(", ");
			test.push_back("world");
			return test;
		};
};


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
		Server server;
		for (str_vec::const_iterator i = args.begin(); i != args.end(); ++i)
			std::cout << *i << std::endl;
		std::cout << "]" << std::endl;
		std::cout << "mon test: [" << args[0] << "]" << std::endl;
		str_vec		tmp = server.getTest();
		for (str_vec::const_iterator it = tmp.begin();
			it != tmp.end();
			it++)
			std::cout << *it << std::endl;
	}
	else
		std::cout << "Wrong number of arguments" << std::endl;
	return (0);
}
