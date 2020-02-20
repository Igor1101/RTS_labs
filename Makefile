PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))
LIBS=-lSDL2 -lgdrive -ljconer -lcurl
OBJS = main.o \
	statistics.o\
	RNG.o \
	app.o \
	file_insert.o

INCFILES = -I ~/libgdrive/include
LIBPATH =  -L ~/libgdrive/ -L ~/libgdrive/lib
CFLAGS += $(INCFILES) 
ifeq ($(BUILD_MODE),debug)
	CFLAGS += -g
else ifeq ($(BUILD_MODE),run)
	CFLAGS += -O2
else
	$(error Build mode $(BUILD_MODE) not supported by this Makefile)
endif

all:	RTS_labs

RTS_labs:	$(OBJS)
	$(CXX) -o $@ $^ $(LIBS) $(LIBPATH)

%.o:	$(PROJECT_ROOT)%.cpp
	$(CXX) -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

%.o:	$(PROJECT_ROOT)%.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr RTS_labs $(OBJS)
