#
# 实用库的Makefile
#
ROOT=..
include $(ROOT)/Make.defines

OBJS = flag_op.o func_wrapper.o pr_exit.o
LIBMISC = libutil.a

all: $(LIBMISC)

$(LIBMISC): $(OBJS)
	ar rv $(LIBMISC) $^

%.o: %.cc
	$(CC) $(FLAGS) $^ -c

clean:
	rm -f *.o a.out core temp.* $(LIBMISC)

include $(ROOT)/Make.libutil.inc
