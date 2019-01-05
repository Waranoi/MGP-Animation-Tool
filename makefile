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

# Constants
EXE := MPG-Animation-Tool
BIN := build
SRCROOT = source

# Store path to source files, in a convenient format
SRCDIRS := $(patsubst %/,%,$(dir $(CSRCS)) $(dir $(CXXSRCS)))
# Add all folders below source root as source directories, in a convenient format. Folders to and below the ones specified in the EXCLUDE variable are ignored
SRCDIRS += $(patsubst ./%,%,$(shell find ./$(SRCROOT) $(patsubst %,-not \( -path ./% -prune \),$(EXCLUDE)) -type d))

# Find all source files in all source directories
CSRCS += $(wildcard $(SRCDIRS:%=%/*.c))
CXXSRCS += $(wildcard $(SRCDIRS:%=%/*.cpp))
# Set all source directories as include folders, in a valid format
INCLUDE += $(SRCDIRS:%=-I%) -MMD -MP

# TODO Remember to check for duplicates and abort if found
# Create object and dependency files from source files below the binary folder
OBJS := $(CSRCS:%.c=$(BIN)/%.o) $(CXXSRCS:%.cpp=$(BIN)/%.o)
DEPS := $(OBJS:.o=.d)

# List of binary directories
BINDIRS := $(BIN) $(SRCDIRS:%=build/%)

# Dummy variable that's never used. Evaluated immidiately which creates all the needed binary directories
create-output-directories := 				\
	$(shell for f in $(BINDIRS); 			\
		do 									\
			MKDIR -p $$f; 					\
		done)

$(BIN)/$(EXE): $(OBJS)
	g++ -o $@ $^ $(LDIR) $(LIBS)

$(BIN)/%.o: %.cpp
	g++ -c -o $@ $< $(INCLUDE)

$(BIN)/%.o: %.c
	g++ -c -o $@ $< $(INCLUDE)

.PHONY: clean
clean:
	rm -r $(BIN)

-include $(DEPS)

# Resources for future improvement of this makefile:
# http://aegis.sourceforge.net/auug97.pdf
# http://uploads.mitechie.com/books/Managing_Projects_with_GNU_Make_Third_Edition.pdf