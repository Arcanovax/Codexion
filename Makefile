NAME = codexion

CC = cc
CFLAGS += -Werror -Wextra -Wall -pthread -MMD -MP -g3

SRCS = \
	codexion.c	\
	ft_init.c	\
	time.c		\
	dongles.c	\
	coders.c	\
	monitoring.c\
	queue.c

OBJDIR = .obj

OBJS = $(SRCS:%.c=$(OBJDIR)/%.o)

DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(OBJS) -o $@


$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(DEPS)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)

.PHONY: all clean fclean re
