CC      = gcc
CXX     = g++
CFLAGS  = -w
LIBS    = -lpaillier -lgmp
TARGETS = key_factory encrypt decrypt sum_he.so

all: $(TARGETS)

key_factory: key_factory.c
	$(CC) paillier_helpers.c $< -o $@ $(CFLAGS) $(LIBS)

encrypt: encrypt.c
	$(CC) paillier_helpers.c $< -o $@ $(CFLAGS) $(LIBS)

decrypt: decrypt.c
	$(CC) $< -o $@ $(CFLAGS) $(LIBS)

sum_he.so: sum_he.c
	$(CC) paillier_helpers.c $< -o $@ $(CFLAGS) -shared -fPIC $(LIBS) -I/usr/include/mysql

clean:
	rm -f $(foreach v, $(TARGETS), $(v).o) $(foreach v, $(TARGETS), $(v))
