CC		= gcc
CXX		= g++
FLAGS   = -lgmp -lpaillier
TARGETS = key_factory encrypt decrypt

all: $(TARGETS)

%.o: %.c
	$(CC) $< $(FLAGS) -c -o $@

%: %.o
	$(CC) $< $(FLAGS) -o $@

sum_he.so: sum_he.c
	$(CC) $< $(FLAGS) -shared -o $@


clean:
	rm -f $(foreach v, $(TARGETS), $(v).o) $(foreach v, $(TARGETS), $(v))