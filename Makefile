CC = gcc
CC_OPTS=-O0 \
		-Wextra \
		-std=c99 \
		-Wall \
		-Wshadow \
		-Wpointer-arith \
		-Wcast-align \
		-fsanitize=address \
		-fsanitize=undefined \
		-fno-sanitize-recover=all \
		-fsanitize=float-divide-by-zero \
		-fsanitize=float-cast-overflow \
		-fno-sanitize=null \
		-fno-sanitize=alignment


SRCDIR=src
APPNAME = prefix
EX_TARGET_DIR=$(EX_DIR)/target
src=$(addprefix $(SRCDIR)/, *.c)

run: $(APPNAME)
	./$(APPNAME)

$(APPNAME): $(src)
	$(CC) $(CC_OPTS) $(src) -o $(APPNAME)
	

.PHONY: clean
clean:
	rm -f $(APPNAME)