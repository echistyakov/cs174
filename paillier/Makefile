CC		= gcc
CXX		= g++
FLAGS   = -lgmp -lpaillier
TARGETS = key_factory encrypt decrypt

all: $(TARGETS)

%: %.c
	$(CC) paillier_helpers.c $< -o $@ $(FLAGS)

sum_he.so: sum_he.c
	$(CC) paillier_helpers.c $< -o $@ $(FLAGS) -shared


clean:
	rm -f $(foreach v, $(TARGETS), $(v).o) $(foreach v, $(TARGETS), $(v))
