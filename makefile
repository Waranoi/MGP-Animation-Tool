# This makefile automatically adds all directories and sub-directories under the 'source' directory as c-source folders, cpp-source folders and include folders.
# If you want to exclude one or more folders from this automated process then add the folders to the EXCLUDE variable.
# You can manually add source and include files below, libraries and library folders have to be added manually.

EXCLUDE := source/core/Window source/vmath-0.13

CSRCS := source/core/Window/glad/src/glad.c
CXXSRCS := source/vmath-0.13/src/vmath.cpp
INCLUDE := -Isource/core/Window/glfw-3.2.1.bin.WIN64/include -Isource/core/Window/glad/include -Isource/vmath-0.13/src

LDIR := -Lsource/core/Window/glfw-3.2.1.bin.WIN64/lib-mingw-w64
LIBS := -lglfw3 -lopengl32 -lgdi32

########## DO NOT EDIT ANYTHING BELOW THIS ##########
########## DO NOT EDIT ANYTHING BELOW THIS ##########
########## DO NOT EDIT ANYTHING BELOW THIS ##########

EXE := MPG-Animation-Tool
BIN := build

SRCDIRS := $(patsubst ./%,%,$(shell find ./source $(patsubst %,-not \( -path ./% -prune \),$(EXCLUDE)) -type d))

CSRCS += $(wildcard $(SRCDIRS:%=%/*.c))
CXXSRCS += $(wildcard $(SRCDIRS:%=%/*.cpp))
INCLUDE += $(SRCDIRS:%=-I%) -MMD -MP

OBJS := $(CSRCS:.c=.o) $(CXXSRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

$(BIN)/$(EXE): $(OBJS) | $(BIN)/
	g++ -o $@ $^ $(LDIR) $(LIBS)

%.o: %.cpp
	g++ -c -o $@ $< $(INCLUDE)

%.o: %.c
	g++ -c -o $@ $< $(INCLUDE)

$(BIN)/:
	mkdir -p $@

-include $(DEPS)

# Resource for future improvement of this makefile http://aegis.sourceforge.net/auug97.pdf