CC = clang

CLANGINC = /usr/local/lib/clang/9.0.0/include/

CFLAGS =-w -c -I$(CLANGINC)

SOURCE = 18822364
OBJECT = $(join $(addsuffix $(BUILDDIR)/, $(dir $(SOURCE))), $(notdir $(SOURCE:=.o)))

all: $(OBJECT)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o 
