#    Makefile for spy
#    Magne Guttormsen & Alexander Bürger
#    December 2008

# most settings like F95, CC, ... are in a common file 'Makeconf'
BASEDIR = ..
include $(BASEDIR)/Makeconf

TARGET = spy

MINIG_X = $(BASEDIR)/src/minig_x.a
ARCHIVES = main.a minig_x.a 
OBJ = main.o

all: $(TARGET)

main.o: FORCE
	$(MAKE) -C spylibs/spyfiles main.o

spy: main.o main.a minig_custom.o $(MINIG_X)
	@echo "  [LINK] $@"
	@$(F95) $(FFLAGS) -o $@ $^ $(LIB_PATHS) $(LIBS)

# archives 
#       force check of libraries....
main.a: FORCE
	$(MAKE) -C spylibs/spyfiles $@

MINIG_X_IF_NEEDED = $(shell make -q -C $(BASEDIR)/src minig_x.a > /dev/null || echo "FORCE")
$(MINIG_X): $(MINIG_X_IF_NEEDED)
	$(MAKE) -C $(BASEDIR)/src minig_x.a

.c.o:
	@echo "    [CC] $@"
	@$(CC) $(CFLAGS) -o $@ -c $*.c

FORCE:

clean: 
	$(RM) *.o core *[~%] 
	$(MAKE) -C spylibs/spyfiles clean

very-clean:
	$(RM) *.o core *[~%] $(TARGET) $(ARCHIVES)
	$(MAKE) -C spylibs/spyfiles very-clean
	
remove:
	for target in $(TARGET) ; \
		do rm -f $(INSTALL_DIR)/bin/$$target ; \
	done

install:
	for program in $(TARGET) ; \
		do $(INSTALL_BIN) $$program $(INSTALL_DIR)/bin/$$program; \
	done
