NAME := libsjson.a

ARG_TEST +=

OBJ_DIR := .obj
DEP_DIR := .dep

TEST_SRC := test.c

SRCS := src/explorer/sjson_explorer.c              \
        src/explorer/sjson_search_index_in_array.c \
        src/explorer/sjson_search_pair_in_object.c \
        src/explorer/sjson_test_type.c             \
        src/parser/in_set.c                        \
        src/parser/jump_blanks.c                   \
        src/parser/new_array.c                     \
        src/parser/new_object.c                    \
        src/parser/new_pair.c                      \
        src/parser/new_real.c                      \
        src/parser/new_value.c                     \
        src/parser/sjson_parse_file.c              \
        src/parser/sjson_parse_str.c               \
        src/parser/string_extractor.c              \
        src/printer/sjson_error.c                  \
        src/printer/sjson_print.c                  \
        src/sjson_free.c                           \
        src/ft_clear.c

OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRCS))
TEST_OBJ := $(patsubst %.c, $(OBJ_DIR)/%.o, $(TEST_SRC))

CFLAGS += -Iinc -Wall -Wextra -Werror
LDLIBS +=

ifneq ($(DEBUG), )
PRE_TEST += valgrind
CFLAGS += -g
endif

ifneq ($(SANITIZE), )
CFLAGS += -g -fsanitize=address
LDLIBS += -fsanitize=address
endif

#if neither DEBUG nor SANITIZE is set
ifneq ($(DEBUG), )
ifneq ($(SANITIZE), )
CFLAGS += -O3
endif
endif

ifneq ($(SWT_KEY_TYPE), )
CFLAGS += -DSWT_KEY_TYPE=$(SWT_KEY_TYPE)
endif

ifneq ($(SWT_VALUE_TYPE), )
CFLAGS += -DSWT_VALUE_TYPE=$(SWT_VALUE_TYPE)
endif

.PHONY: all clean fclean re test
.PRECIOUS: $(OBJ_DIR)/. $(OBJ_DIR)%/.

all: $(NAME)

test: test.bin
	$(PRE_TEST) ./test.bin $(TEST_ARG)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $@ $?

test.bin: $(TEST_OBJ) | $(NAME)
	$(CC) $(LDFLAGS) $^ $| $(LDLIBS) -o $@

$(OBJ_DIR)/.:
	mkdir -p $@

$(OBJ_DIR)%/.:
	mkdir -p $@

clean:
	$(RM) -rf $(OBJ_DIR)
	$(RM) -f test.bin

fclean: clean
	$(RM) -rf $(NAME)

re: | fclean all

.SECONDEXPANSION:
$(OBJ_DIR)/%.o: %.c | $$(@D)/.
	$(CC) $(CFLAGS) -c $< -o $@
