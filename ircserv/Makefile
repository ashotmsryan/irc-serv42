NAME = ircserv

CXXFLAGS = -Wall -Wextra -Werror

SRCS = $(wildcard src/*.cpp)

OBJFILE = objs

OBJS = $(patsubst src/%.cpp, $(OBJFILE)/%.o, $(SRCS))

RM = rm -rf

MD = mkdir -p

all: $(OBJFILE) $(NAME)

$(OBJFILE) :
	$(MD) $(OBJFILE)

$(OBJFILE)/%.o : src/%.cpp Makefile includes/*.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@stty eof ^V

$(NAME): $(OBJS) Makefile
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJFILE)

fclean: clean
	$(RM) $(NAME)
	@stty eof '^D'

bonus: $(NAME)

re: fclean all

.PHONY: all clean fclean re
