# Executable names:
EXE = main
TEST = test

# Add all object files needed for compiling:
EXE_OBJ = main.o
OBJS = main.o Airport.o RouteMaker.o Plane.o parser.o

# CPP_FILES += $(wildcard data/*.cpp)
# OBJS += $(CPP_FILES:.cpp=.o)

# Generated files
#edit these later when we are generating images for the map
# CLEAN_RM = unsolved.png solved.png testdsets testsquaremaze \
# 						*.o *.d tests/*.o tests/*.d cs225/*.d cs225/*.o \
# 						cs225/lodepng/*.o cs225/lodepng/*.d \
# 						cs225/catch/*.o cs225/catch/*.d

OBJS_DIR = .objs

EXE_PARSER = parsertest
OBJS_PARSER = 


# Use the cs225 makefile template:
include cs225/make/cs225.mk

$(EXE_PARSER): $(OBJS_PARSER)
	$(LD) $^ $(LDFLAGS) -o $@