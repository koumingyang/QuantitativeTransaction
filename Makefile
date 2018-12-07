SOURCE  := $(wildcard *.c) $(wildcard *.cpp)
#OBJS    := $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCE)))

OBJS =      BootFunc.o	\
            BootFuncBMA.o	\
			GenBmaWeights.o \
			GenRollingErr.o \
			Regress.o

OBJS_MAIN = $(OBJS)	\
			Main.o	

OBJS_MAIN_ANNUAL1 = $(OBJS)	\
					MainAnnual1.o

OBJS_MAIN_MONTHLY1 = $(OBJS)	\
					MainMonthly1.o

OBJS_BOOTSTRAP_BMA = $(OBJS)	\
					BootStrapBMA.o

OBJS_ALL = $(OBJS)  \
            Main.o  \
            MainAnnual1.o	\
			BootStrapBMA.o

TARGET_MAIN =    main
TARGET_MAIN_ANNUAL1 = 	main_annual1
TARGET_MAIN_MONTHLY1 = 	main_monthly1
TARGET_BOOTSTRAP_BMA = 	bootstrap_bma
  
CC      := gcc
LIBS    += -lblas -llapack 
LDFLAGS := 
DEFINES :=
INCLUDE := -I .\include
CFLAGS  := -g -Wall -O3 -std=c++14 $(DEFINES) $(INCLUDE)
CXXFLAGS:= $(CFLAGS) -DHAVE_CONFIG_H
  
  
.PHONY : everything objs clean veryclean rebuild

everything : $(TARGET_MAIN)

all : $(TARGET_MAIN)

annual1 : $(TARGET_MAIN_ANNUAL1)

monthly1: $(TARGET_MAIN_MONTHLY1)

btstrpbma : $(TARGET_BOOTSTRAP_BMA)
  
objs : $(OBJS)
  
rebuild: veryclean everything
                
clean :
	del -fr *.so
	del -fr *.o
    
veryclean : clean
	del -fr $(TARGET_MAIN) $(TARGET_MAIN_ANNUAL1) $(TARGET_MAIN_MONTHLY1) $(TARGET_BOOTSTRAP_BMA)
  
$(TARGET_MAIN) : $(OBJS_MAIN)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_MAIN) $(LDFLAGS) $(LIBS)
$(TARGET_MAIN_ANNUAL1) : $(OBJS_MAIN_ANNUAL1)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_MAIN_ANNUAL1) $(LDFLAGS) $(LIBS)
$(TARGET_MAIN_MONTHLY1) : $(OBJS_MAIN_MONTHLY1)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_MAIN_MONTHLY1) $(LDFLAGS) $(LIBS)
$(TARGET_BOOTSTRAP_BMA) : $(OBJS_BOOTSTRAP_BMA)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_BOOTSTRAP_BMA) $(LDFLAGS) $(LIBS)
	

