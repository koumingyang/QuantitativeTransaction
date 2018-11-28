RM		:= rm -f

CXX = g++

CXXFLAGS =    -O2 -g -Wall -fmessage-length=0 -Wc++11-extensions\

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

$(TARGET_MAIN):    $(OBJS_MAIN)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)
$(TARGET_MAIN_ANNUAL1):    $(OBJS_MAIN)
	$(CXX) -o $@ $^ $(LDFLAGS) $(CXXFLAGS)

all:
	$(TARGET_MAIN)
annual1:
	$(TARGET_MAIN_ANNUAL1)
clean:
	rm -f $(OBJS_ALL) $(TARGET_MAIN) $(TARGET_MAIN_ANNUAL1)

