// Louis : fichier pour faire des tests
// c++ -Wall -Wextra -Werror -std=c++98 tmp.cpp && ./a.out "CMD mes arguments"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

typedef std::vector<std::string>			str_vec;

str_vec		split_str(std::string initial, std::string separator)
{
	str_vec			output;
	size_t			pos = 0;
	std::string		tmp;
	while ((pos = initial.find(separator)) != std::string::npos) {
		tmp = initial.substr(0, pos);
		output.push_back(tmp);
		initial.erase(0, pos + separator.length());
	}
	output.push_back(initial);
	return output;
}

std::string		ft_to_string(size_t	nb)
{
	std::stringstream	ss;
	ss << nb;
	return (ss.str());
}


size_t			ft_to_size_t(std::string my_str)
{
	std::stringstream	sstream(my_str);
	size_t				result;
	sstream >> result;
	return (result);
}



int main(int argc, char **argv)
{
	if (argc == 2)
	{
		std::string input = argv[1];
        // input.append("\r\n");
		std::string::size_type pos = input.find(' '); // might be string::npos
		std::string cmd_str = input.substr(0, pos);
		std::string test = input.substr(pos + 1, std::string::npos);
		size_t	result = ft_to_size_t(test);
		std::cout << result << std::endl;
		std::string test2 = ft_to_string(result);
		std::cout << test2 << std::endl;
		// std::string arg;
		// str_vec		args;
		// if (input.length() != cmd_str.length())
		// {
		// 	arg = input.substr(pos + 1, std::string::npos);
		// }
		// 	args = split_str(arg, " ");
		// std::cout << "command: [" << cmd_str << "]" << std::endl;
		// std::cout << "args: [";
		// for (str_vec::const_iterator i = args.begin(); i != args.end(); ++i)
		// 	std::cout << "{" << *i << "}" << std::endl;
		// std::cout << "]" << std::endl;
	}
	else
		std::cout << "Wrong number of arguments" << std::endl;
	return (0);
}
