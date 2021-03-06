CC = gcc
src = src
inc = inc
test = test
lib = lib
objdir = obj
CFLAGS = -Wall -Werror -Wextra
LDFLAGS = -lm -L$(lib) -lpelf
doxygenFile = doxyfile

updateLib = 0

RELEASE = assembler.exe
DEBUG = assembler.dbg

incFiles = $(wildcard $(inc)/*.h)
obj = $(patsubst $(src)%.c, $(objdir)%.o, $(wildcard $(src)/*.c))
objWithoutMain = $(subst $(objdir)/main.o,,$(obj))
testObj = $(patsubst $(test)%.c, $(objdir)%.o, $(wildcard $(test)/*.c))
testExe = $(patsubst $(test)%.c, $(test)%.exe, $(wildcard $(test)/*.c))

help:
	@echo ""
	@echo "Informatique project: MIPS assembler"
	@echo ""
	@echo "Usage:"
	@echo ""
	@echo "make release         => build RELEASE version"
	@echo "make test/test%.exe  => build the test/test%.c file"
	@echo "make allTest         => build build all tests from test directory"
	@echo "make clean           => clean object file from obj directory"
	@echo "make cleanAll        => clean everything"
	@echo "make documentation   => create a documentation"
	@echo ""


$(objdir)/%.o : $(src)/%.c
	$(CC) -c -g3 -I$(inc) -o $@ $<

$(objdir)/%.o : $(test)/%.c
	$(CC) -c -g3 -I$(inc) -o $@ $<

debug: $(obj)
	$(CC) -o $(DEBUG) $^ $(LDFLAGS)

# $(objdir)/%.o : $(src)/%.c
# 	$(CC) -c -I$(inc) -o $@ $<

# $(objdir)/%.o : $(test)/%.c
# 	$(CC) -c -I$(inc) -o $@ $<

release: $(obj)
	$(CC) $(CFLAGS) -o $(RELEASE) $^ $(LDFLAGS)

$(test)/test%.exe: $(objWithoutMain) $(objdir)/test%.o
	$(CC) -o $@ $^ $(LDFLAGS)

allTest: $(testExe)

exportLib:
	@export LD_LIBRARY_PATH=/home/raphael/Project/AssemblerMIPS/lib/:/usr/local/lib/
	
documentation : 
	doxygen $(doxygenFile)

clean:
	rm $(objdir)/*.o

cleanAll:
	$(MAKE) clean
	rm -rf *.exe
	rm -rf test/*.exe