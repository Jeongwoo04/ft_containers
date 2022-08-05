NAME_FT		= ft_container.out
NAME_STD	= std_container.out
SRCS		= main.cpp
HEADERS		= vector.hpp \
			stack.hpp \
			utils/random_access_iterator.hpp \
			utils/iterators_traits.hpp \
			utils/reverse_iterator.hpp \
			utils/is_integral.hpp \
			utils/enable_if.hpp \
			utils/pair.hpp \
			utils/lexicographical_compare.hpp

OBJS_A		= $(SRCS:.cpp=.ft)
OBJS_B		= $(SRCS:.cpp=.std)
OBJS		= $(OBJS_A) $(OBJS_B)

CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -std=c++98
RM			= rm -rf

CCBLUE		= \033[34m
CCBLUE_BOLD	= \033[1;34m
CCEND		= \033[0m

all: $(NAME_FT) $(NAME_STD)

ft: $(NAME_FT)

std: $(NAME_STD)

$(NAME_FT): $(OBJS_A)
	$(CXX) $(OBJS_A) $(CXXFLAGS) -o $(NAME_FT)
	@echo "$(CCBLUE_BOLD) >>> make $(NAME_FT)  <<< $(CCEND)"

$(NAME_STD): $(OBJS_B)
	$(CXX) $(OBJS_B) $(CXXFLAGS) -o $(NAME_STD)
	@echo "$(CCBLUE_BOLD) >>> make $(NAME_STD)  <<< $(CCEND)"

%.ft: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -D FT -c $< -o $@

%.std: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -D STD -std=c++11 -c $< -o $@

clean:
	@echo "$(CCBLUE) >>> make clean <<< $(CCEND)"
	$(RM) $(OBJS)

fclean: clean
	@echo "$(CCBLUE) >>> make fclean <<< $(CCEND)"
	$(RM) $(NAME_FT) $(NAME_STD)

re: fclean all

.PHONY: all clean fclean re ft std
