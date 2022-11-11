# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llalba <llalba@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/19 13:33:48 by llalba            #+#    #+#              #
#    Updated: 2022/11/11 13:07:37 by llalba           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CXX				= c++

NAME			= ircserv

SRC_DIR			= src

OBJ_DIR			= obj

INC_DIR			= include

SRC				= main.cpp \
				Server.cpp \
				Channel.cpp \
				commands.cpp \
				User.cpp

OBJ				= $(SRC:%.cpp=$(OBJ_DIR)/%.o)

DEPS			= $(SRC:%.cpp=$(OBJ_DIR)/%.d)

CXXFLAGS		= -Wall -Wextra -Werror -std=c++98

all:			$(NAME)

$(NAME):		$(OBJ)
				$(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR):
				mkdir $@

-include $(DEPS)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp | $(OBJ_DIR)
				$(CXX) $(CXXFLAGS) -MMD -MP $(INC_DIR:%=-I %) -c $< -o $@

clean:
				rm -rf $(OBJ_DIR)


fclean:			clean
				rm -f $(NAME)

re:				fclean all

.PHONY: all clean fclean re
