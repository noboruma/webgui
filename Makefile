ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

ifeq ($(OS),Windows_NT)
	CXXFLAGS+=-DWEBVIEW_WINAPI=1 -lole32 -lcomctl32 -loleaut32 -luuid -mwindows
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
		CXXFLAGS+=-DWEBVIEW_GTK=1 `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.0`
    endif
    ifeq ($(UNAME_S),Darwin)
		CXXFLAGS+=-DWEBVIEW_COCOA=1 -x objective-c -framework Cocoa -framework WebKit
    endif
endif

APPNAME=webgui

# User defined
#HEADERS=
SRCS= $(ROOT_DIR)/src/main.cc
RES= $(ROOT_DIR)/resources/html/index.html

# Auto defined
OBJS= $(notdir $(SRCS:.cc=.o))
RAWS= $(notdir $(RES))
#PCH=$(HEADERS).gch
CXXFLAGS+=-I$(ROOT_DIR)/deps/pistache/include -I$(ROOT_DIR)/deps -I.
LDFLAGS+=-lpistache -pthread -L$(ROOT_DIR)/deps/pistache/build/src

CXXFLAGS+=-O3 -DNDEBUG -std=c++14

all: app

debug: CXXFLAGS+=-DDEBUG -g
debug: app

app: $(RAWS) $(OBJS) $(PCH)
	$(CXX) $^ $(CXXFLAGS) $(LDFLAGS) -o $(APPNAME)

#$(PCH): $(HEADERS)
    #$(CXX) $(CXX_CFLAGS) -x c++-header $(PCH)

$(RAWS): $(RES)
	cat $< > $@
	sed -i '1s/^/R"(=====/' $@
	sed -i '$$s/$$/=====)"/' $@

%.o: $(ROOT_DIR)/src/%.cc
	$(CXX) -c $< -O3 $(CXXFLAGS) -o $@

clean:
	rm ./*.o
	rm ./$(APPNAME)
	rm ./html.h
