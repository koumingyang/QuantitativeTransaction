RM		:= rm -f

CXX = g++

CXXFLAGS =    -O2 -g -Wall -fmessage-length=0\

OBJS =      BootFuncBMA.o	\
            BaseFunc.o    	\
            Matrix.o		

OBJS_MAIN = $(OBJS)	\
			main.o

OBJS_MAIN_ANNUAL1 = $(OBJS)	\
					MainAnnual1.o

OBJS_ALL = $(OBJS)  \
            main.o  \
            MainAnnual1.o

TARGET_MAIN =    main
TARGET_MAIN_ANNUAL1 = 	main_annual1

$(TARGET_MAIN):    $(OBJS_MAIN)
	$(CXX) -o $@ $^ $(LDFLAGS)
$(TARGET_MAIN_ANNUAL1):    $(OBJS_MAIN)
	$(CXX) -o $@ $^ $(LDFLAGS)

all:
	$(TARGET_MAIN)
annual1:
	$(TARGET_MAIN_ANNUAL1)
clean:
	rm -f $(OBJS_ALL) $(TARGET_MAIN) $(TARGET_MAIN_ANNUAL1)

