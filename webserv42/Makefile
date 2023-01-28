NAME	:= webserv
CC		:= g++ -g
INCLUDE	:= ./inc
CFLAGS	:= -ggdb -I $(INCLUDE) -I ./libft
FLAGS	= -Wall -Wextra -Werror
SRCDIR	:= src
SRC		:= $(shell find $(SRCDIR) -name '*.cpp')
OBJDIR	:= ./obj
OBJ		:= $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -g $(FLAGS) $(CFLAGS) $(OBJ) -o $@


$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(@D)
	$(CC) -g $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)

fclean:
	@rm -rf $(OBJDIR)
	@rm -f $(NAME)

re:	fclean all

.PHONY: clean fclean re all