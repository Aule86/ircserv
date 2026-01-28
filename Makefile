NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98

#SRCS = main.cpp SRC/Authenti.cpp SRC/Channel.cpp SRC/Client.cpp SRC/Server.cpp \
	CMD/INVITE.cpp CMD/KICK.cpp CMD/MODE.cpp CMD/TOPIC.cpp 
SRCS = src/Server.cpp  main.cpp src/Client.cpp src/Aunth.cpp src/Channel.cpp \
	cmd/JOIN.cpp cmd/KICK.cpp cmd/PRIVMSG.cpp cmd/QUIT.cpp


OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
