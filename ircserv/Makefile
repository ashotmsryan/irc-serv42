NAME = ircserv

CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = $(wildcard src/*.cpp)

OBJFILE = objs

OBJS = $(patsubst %.cpp, %.o, $(SRCS))

RM = rm -rf

MD = mkdir -p

all: $(NAME)

$(NAME): $(OBJS)
	@$(MD) $(OBJFILE)
	@mv $(OBJS) $(OBJFILE)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJFILE)/*.o

clean:
	$(RM) $(OBJFILE)

fclean: clean
	$(RM) $(NAME)

bonus: $(NAME)

re: fclean all

.PHONY: all clean fclean re
