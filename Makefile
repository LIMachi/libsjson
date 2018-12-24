NAME := libsjson.a

ARG_TEST +=

OBJ_DIR := .obj

TEST_SRCS := test.c

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

CFLAGS += -Iinc
LDLIBS +=
PRE_TEST :=
CLIB := ../swiss-table/swiss-table.a

include ../Makefiles/lib.mk