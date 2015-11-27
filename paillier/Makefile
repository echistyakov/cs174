CC		= gcc
CXX		= g++
FLAGS   = -lgmp -lpaillier
TARGETS = key_factory encrypt decrypt

all: $(TARGETS)

%.o: %.c
	$(CC) $< $(FLAGS) -c -o $@

%: %.o
	$(CC) $< $(FLAGS) -o $@

clean:
	rm -f $(foreach v, $(TARGETS), $(v).o) $(foreach v, $(TARGETS), $(v))