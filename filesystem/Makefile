ROOT=..
include $(ROOT)/Make.defines

PROGS = filetype access_test umask_test chmod_test unlink_test trunk_not_change_time myftw myftw_chdir

all: $(PROGS)

%: %.cc
	$(CC) $(FLAGS) $^ -o $@

clean:
	rm -f a.out foo bar tempfile $(PROGS)
