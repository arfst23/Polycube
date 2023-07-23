################################################################################

CXX		= g++
CFLAGS		= -march=native -O3 -funroll-loops -fpic
REASON		= @if [ -f $@ ]; then echo "[$@: $?]"; else echo "[$@]"; fi

all: poly

################################################################################

poly: polycube.cc
	$(REASON)
	$(CXX) -DTHREADS -fopenmp $(CFLAGS) -o $@ $^ 

################################################################################

tags:
	ctags --format=2 -o $@ *.h *.cc

deps depend: *.h *.c
	$(REASON)
	$(CC) -MM $(CPPFLAGS) *.c > deps

clean:
	$(RM) *.o *.gcda 

distclean: clean
	$(RM) core log deps tags *~ poly

-include deps

################################################################################
