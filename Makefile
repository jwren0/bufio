## User configuration ##
AR ?= ar
CC ?= gcc

ARFLAGS :=
CCFLAGS ?= -march=native -O2 -pipe

INC_DIR ?= /usr/include
LIB_DIR ?= /usr/lib

LIB := bufio


## Developer configuration ##
ARFLAGS := $(ARFLAGS)
CCFLAGS := $(CCFLAGS) -Werror -Wall -Wextra -Wformat-security \
			-Wpedantic -pedantic-errors -std=c18

INC_DIR := $(INC_DIR)/$(LIB)

LIB := lib$(LIB)

SRC_FILES := src/buf_reader.c
HEAD_FILES := src/buf_reader.h
OBJ_FILES := ${SRC_FILES:.c=.o}

SRC_TESTS := tests/test_br.c
OBJ_TESTS := ${SRC_TESTS:%.c=%}


## User targets ##
build: $(LIB).so
build_debug: $(LIB)_debug.so
build_static: $(LIB).a
build_tests: build $(OBJ_TESTS)

test: build_tests
	@for f in $(OBJ_TESTS); do \
		printf "TEST    $$f\n"; \
		LD_LIBRARY_PATH=. ./$$f; \
	done

install: build
	@[ -d $(INC_DIR) ] || mkdir -v -m 755 $(INC_DIR)
	@install -v -m 755 $(LIB).so $(LIB_DIR)/
	@for f in ${HEAD_FILES:src/%=%}; do \
		[ -d $(INC_DIR)/`dirname $$f` ] \
			|| mkdir -v -m 755 $(INC_DIR)/`dirname $$f`; \
		install -v -m 644 src/$$f $(INC_DIR)/$$f; \
	done
	@ldconfig

uninstall:
	@rm -rf $(INC_DIR) $(LIB_DIR)/$(LIB).so
	@ldconfig

clean:
	@rm -rf $(LIB).so $(LIB).a $(LIB)_debug.so \
		$(OBJ_TESTS) $(OBJ_FILES)


## Developer targets ##
$(LIB).so: $(OBJ_FILES)
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -Xlinker --strip-all -shared -o $@ $^

$(LIB)_debug.so: $(OBJ_FILES)
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -shared -g -o $@ $^

$(LIB).a: $(OBJ_FILES)
	@printf "AR      $@\n"
	@$(AR) $(ARFLAGS) -rcs -o $@ $^
	@printf "STRIP   $@\n"
	@strip --strip-unneeded $@

src/%.o: src/%.c src/%.h
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -c -g -o $@ $<

src/%.o: src/%.c
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -c -g -o $@ $<

tests/%: tests/%.c
	@printf "CC      $@\n"
	@$(CC) $(CCFLAGS) -L. -l${LIB:lib%=%} -o $@ $<
