#--------------------------------
# User Setting
#--------------------------------

PROGRAM = test.bin
SRCS = main.cpp poseKeyPoints.cpp

#--------------------------------
# Compile Option
#--------------------------------

CXX = g++
LDLIBS =
CXXFLAGS = -g -Wall -MMD -MP -std=gnu++11
CFLAGS = -g -Wall -Wextra -MMD -MP

#--------------------------------
# Directories
#--------------------------------

OBJDIR    = ./build
BINDIR    = .
INCLUDE   = -I .

#--------------------------------
# Don't change the following
#--------------------------------

ifeq "$(strip $(OBJDIR))" ""
	OBJDIR = .
endif
ifeq "$(strip $(BINDIR))" ""
	BINDIR = .
endif

TARGET   = $(addprefix $(BINDIR)/, $(PROGRAM))
OBJS     = $(addprefix $(OBJDIR)/, $(SRCS:.cpp=.o))
DEPENDS  = $(OBJS:.o=.d)

default:
	@[ -d $(OBJDIR)] || mkdir -p $(OBJDIR)
	@[ -d $(BINDIR)] || mkdir -p $(BINDIR)
	@make all
	@echo ""
	@echo "make $(TARGET)"
	@echo "success"

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ $^ $(LDLIBS)

$(OBJDIR)/%.o: %.cpp
		-mkdir -p $(dir $@)
		$(CXX) $(CXXFLAGS) $(INCLUDE) -o $@ -c $< $(LDLIBS)

clean:
	$(RM) $(OBJS) $(DEPENDS) $(TARGET)
	rm -R $(OBJDIR)

-include $(DEPENDS)

.PHONY: all clean
