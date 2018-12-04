SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
#OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))
TOP_PATH 	:= $(shell pwd)
LIB_PATH 	:= $(TOP_PATH)/lib/

OBJS =      Vec.o	\
            Matrix.o	

OBJS_MAIN = $(OBJS)	\
			Main.o

OBJS_MAIN_ANNUAL1 = $(OBJS)	\
					MainAnnual1.o

OBJS_ALL = $(OBJS)  \
            Main.o  \
            MainAnnual1.o

TARGET_MAIN =    main
TARGET_MAIN_ANNUAL1 = 	main_annual1
  
CC      := gcc
LIBS    += -larmadillo
LDFLAGS := 
DEFINES :=
INCLUDE := -I.
CFLAGS  := -g -Wall -O3 -std=c++14 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H
  
  
.PHONY : everything objs clean veryclean rebuild
  
everything : $(TARGET)
  
all : $(TARGET_MAIN)

annual1 : $(TARGET_MAIN_ANNUAL1)
  
objs : $(OBJS)
  
rebuild: veryclean everything
                
clean :
	rm -fr *.so
	rm -fr *.o
    
veryclean : clean
	rm -fr $(TARGET_MAIN) $(TARGET_MAIN_ANNUAL1)
  
$(TARGET) : $(OBJS_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
$(TARGET_MAIN_ANNUAL1) : $(OBJS_MAIN_ANNUAL1)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS) $(LIBS)
	

