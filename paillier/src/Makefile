CC		= gcc
CXX		= g++
FLAGS   = -lgmp -lpaillier -w
TARGETS = key_factory encrypt decrypt sum_he.so

all: $(TARGETS)

%: %.c
	$(CC) paillier_helpers.c $< -o $@ $(FLAGS)

sum_he.so: sum_he.c
	$(CC) paillier_helpers.c $< -o $@ $(FLAGS) -shared -fPIC -I/usr/include/mysql

clean:
	rm -f $(foreach v, $(TARGETS), $(v).o) $(foreach v, $(TARGETS), $(v))
