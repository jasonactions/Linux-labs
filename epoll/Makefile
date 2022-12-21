

PROGS = ${patsubst %.c, %, ${wildcard *.c}} 

all: $(PROGS)
	

% : %.c
	$(CC)  $(CFLAGS)  $< -o $@ 
	
.PHONY:clean
clean:
	- rm -f $(PROGS) core *.gz
