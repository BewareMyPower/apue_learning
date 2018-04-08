ROOT=..
include $(ROOT)/Make.defines

PROGS = signal_example non_entrant read_timeout read_timeout_jmp suspend signal_ipc

all: $(PROGS)

%: %.cc
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f a.out temp.* $(PROGS) 

include $(ROOT)/Make.libutil.inc
