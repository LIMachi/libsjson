AUTHOR = hmartzol

NAME = libsjson.a

SRCDIR = src
INCDIRS = inc
OBJDIR = .obj
MAIN = src/test.c

EXEARGS := '{"a": [1, true], "b": "ok"}'

CFLAGS = -Wall -Wextra -Werror -g

PINC := ../libft/inc
CLIB := ../libft
LIB := ../libft/libft.a

include ../Makefiles/Makefile.gen
