CC		= gcc
CXX		= g++
FLAGS   = -lgmp -lpaillier
TARGETS = key_factory encrypt decrypt

all: $(TARGETS)

%.o: %.c
	$(CC) $< -o $@ $(FLAGS) -c

%: %.c
	$(CC) $< -o $@ $(FLAGS)

sum_he.so: sum_he.c
	$(CC) $< -o $@ $(FLAGS) -shared


clean:
	rm -f $(foreach v, $(TARGETS), $(v).o) $(foreach v, $(TARGETS), $(v))
