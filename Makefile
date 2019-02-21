include ../Makefiles/default_var.mk

NAME := libsjson.a

CLIB := ../swiss-table/swiss_table.a
CFLAGS += -I../swiss-table/inc

include ../Makefiles/lib.mk
