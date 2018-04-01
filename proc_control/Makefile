ROOT=..
include $(ROOT)/Make.defines

PROGS = fork vfork tellwait exec
MOREPROGS = pr_exit tsys

all: $(PROGS) $(MOREPROGS) system.o test_system.cc

test_system: test_system.cc system.o $(LIBUTIL) system.h
	$(CC) $(FLAGS) test_system.cc system.o $(LIBUTIL) -o $@

system.o: system.cc
	$(CC) $(FLAGS) $^ -c

pr_exit: pr_exit.cc $(LIBUTIL)
	$(CC) $(FLAGS) $^ -o $@

tsys: tsys.cc $(LIBUTIL)
	$(CC) $(FLAGS) $^ -o $@

%: %.cc
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f a.out temp.* system.o test_system $(PROGS) $(MOREPROGS)

include $(ROOT)/Make.libutil.inc
