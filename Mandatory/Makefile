# NAME = irc

# CC = ++
# CFLAGS = -Wall -Wextra -Werror -std=c++98

# SRCS = src/main.cpp src/Server.cpp
# OBJS = $(SRCS:.cpp=.o)

# all: $(NAME)

# $(NAME): $(OBJS)
#     $(CC) $(CFLAGS) -o $(NAME) $(OBJS)

# clean:
#     rm -f $(OBJS)

# fclean: clean
#     rm -f $(NAME)

# re: fclean all

# .PHONY: all clean fclean re

NAME = irc

CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = src/main.cpp src/Server.cpp src/Client.cpp
OBJS = $(SRCS:.cpp=.o)

HEADERS = src/Server.hpp src/Client.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
