ROOT=..
include $(ROOT)/Make.defines

PROGS = hole_file lseek_stdin get_fl

all: $(PROGS)

%: %.cc
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f temp.* file.hole file.nohole $(PROGS)
