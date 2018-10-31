NAME			=	PCG_assignment2

CXX				=	g++

WARNINGS		=	-Wall -Wshadow -Wswitch-enum -Wundef	\
					-Wredundant-decls -Wcast-align -Winline	\
					-Wzero-as-null-pointer-constant			\
					-Wnon-virtual-dtor -Wpedantic			\
					-Wmissing-include-dirs -Winvalid-pch

SRDIRS			=	.

DEFAULTFLAGS	=	-std=c++11 -fexceptions $(WARNINGS)

INCLUDEPFLAGS	=	

CXXFLAGS		=	$(DEFAULTFLAGS) $(INCLUDEPFLAGS)

SRC				:=	main.cpp allegpart.cpp

#$(wildcard $(addsuffix /*.cpp, $(SRDIRS)))

OBJ				:=	$(SRC:.cpp=.o)

LDLIBS			:=	-lallegro								\
					-lallegro_font							\
					-lallegro_primitives					\
					-lallegro_color							\
					-lallegro_image

RM				=	/bin/rm -f

.PHONY: all clean fclean re

$(NAME): $(OBJ)
	$(CXX) -o $(NAME) $(OBJ) $(LDLIBS)

all: $(NAME)

clean:
	$(RM) -r $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean $(NAME)
