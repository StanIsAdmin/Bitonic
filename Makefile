# GNU Makefile adapted from:
# https://sites.google.com/site/michaelsafyan/software-engineering/how-to-write-a-makefile
# Help for cross-platform usage from:
# http://skramm.blogspot.be/2013/04/writing-portable-makefiles.html
# More on implicit variables:
# https://www.gnu.org/software/make/manual/html_node/Implicit-Variables.html
# More on automatic variables:
# https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html


# - - - - - VARIABLES - - - - -

# Name of the program
program_NAME := Bitonic

# Source files will be looked for in this directory
program_SOURCE_DIR := src

# Headers in these directories will be found with #include <>
program_INCLUDE_DIRS := include

# Libraries in these directories will be found by the linker
program_LIBRARY_DIRS :=

# Libraries with these names will be linked in
program_LIBRARIES :=

# Binary files will be written to this directory
program_BIN_DIR := bin

# If true, will output all used variables
debugging := false

# CPPFLAGS are extra flags to give to the C preprocessor and programs that use it (like the C compiler)
CPPFLAGS += -std=c++11 -pthread -Wpedantic -Wall -Wextra -Winit-self -Winline -Wconversion -Weffc++ -Wstrict-null-sentinel -Wold-style-cast -Wnoexcept -Wctor-dtor-privacy -Woverloaded-virtual -Wconversion -Wsign-promo -Wzero-as-null-pointer-constant 

# CXXFLAGS are extra flags to give to the C++ compiler
CXXFLAGS +=

# LDFLAGS are used when linking
LDFLAGS += 


# - - - - - CROSS PLATFORM - - - - -

# PATHSEP is the path separator
# RM is them remove command used by the clean target
# END is the file extension of the executable file
ifdef ComSpec
	PATHSEP=\\
	RM=del /F /Q
	END=.exe
	MKDIR1=if not exist
	MKDIR2=mkdir
else
	PATHSEP=/
	RM=rm -f
	END=
	MKDIR1=mkdir -p
	MKDIR2=&& mkdir -p
endif

# - - - - - FILE LOOKUP - - - - -

# List of all files in the src/ directory ending in ".c"
program_C_SRCS := $(wildcard $(program_SOURCE_DIR)/*.c)

# List of all files in the src/ directory ending in ".cpp"
program_CXX_SRCS := $(wildcard $(program_SOURCE_DIR)/*.cpp)

# Names all C object files that we are going to build (substitution expression)
program_C_OBJS := $(patsubst $(program_SOURCE_DIR)/%.c,$(program_SOURCE_DIR)$(PATHSEP)%.o,$(program_C_SRCS))

# Names all C++ object files that we are going to build
program_CXX_OBJS := $(patsubst $(program_SOURCE_DIR)/%.cpp,$(program_SOURCE_DIR)$(PATHSEP)%.o,$(program_CXX_SRCS))

# List of all the ".o" files, both from C and C++ source files
program_OBJS := $(program_C_OBJS) $(program_CXX_OBJS)

# Add I$(includedir) for every include directory given in $(program_INCLUDE_DIRS)
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))

# Add -L$(librarydir) for every library directory given in $(program_LIBRARY_DIRS)
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))

# Add -l$(library) for every library given in $(program_LIBRARIES)
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))


# - - - - - DEBUGGING - - - - -
ifeq ($(debugging),true)
	$(info $$program_C_SRCS   is [${program_C_SRCS}])
	$(info $$program_CXX_SRCS is [${program_CXX_SRCS}])
	$(info $$program_CXX_OBJS is [${program_CXX_OBJS}])
	$(info $$program_C_OBJS   is [${program_C_OBJS}])
	$(info $$program_OBJS     is [${program_OBJS}])
	$(info $$CPPFLAGS is [${CPPFLAGS}])
	$(info $$LDFLAGS  is [${LDFLAGS}])
endif


# - - - - - COMPILATION - - - - -

# Phony targets execute their build rules, even if a file with the same name exists
.PHONY: all clean distclean


# First build rule in the makefile is the default (when executing "make")
all: $(program_NAME)


# In debug versions, use the -g option to provide debugging information
debug: CPPFLAGS += -g
debug: $(program_NAME)


# In released versions, disable asserts and optimize as much as possible
release: CPPFLAGS += -DNDEBUG -O3
release: $(program_NAME)

# The program depends on the object files
# The build rule $(LINK.cc) is used to link the object files and output a file with the same name as the program. LINK.cc makes use of CXX,CXXFLAGS,CPPFLAGS,LDFLAGS,TARGET_ARCH.
# For more info on LINK, do 'make -p | grep LINK'
$(program_NAME): makedirs $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_BIN_DIR)/$(program_NAME)

makedirs: 
	$(MKDIR1) "$(program_BIN_DIR)" $(MKDIR2) "$(program_BIN_DIR)"

# This target removes the built program and the generated object files. 
# The @ symbol indicates that the line should be run silently, and the - symbol indicates that errors should be ignored
clean:
	@- $(RM) $(program_BIN_DIR)$(PATHSEP)$(program_NAME)$(END)
	@- $(RM) $(program_OBJS)


# The distclean target depends on the clean target (so executing distclean will cause clean to be executed), but it also removes configuration files
distclean: clean