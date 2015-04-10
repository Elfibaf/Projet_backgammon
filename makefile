# app name
app = main

# extension to compile
srcExt = c

# directories
srcDir = src
objDir = obj
binDir = .

# debug option (=1 for debugging ; =0 no include debug information)
debug = 0

# compiler options
CFlags = -Wall
# linker options
LDFlags = -rdynamic
# library names
libs = SDL2
# additionnal library directories
libDir = lib/
# additionnal include directories
incDir =


#************************ DO NOT EDIT BELOW THIS LINE! ************************

ifeq ($(debug),1)
	debug=-g
else
	debug=
endif
incDir := $(addprefix -I,$(incDir))
libs := $(addprefix -l,$(libs))
libDir := $(addprefix -L,$(libDir))
CFlags += -c $(debug) $(incDir)
LDFlags += $(libDir) $(libs)
sources := $(shell find $(srcDir) -name '*.$(srcExt)')
srcDirs := $(shell find . -name '*.$(srcExt)' -exec dirname {} \; | sort | uniq)
objects := $(patsubst %.$(srcExt),$(objDir)/%.o,$(sources))

ifeq ($(srcExt),cpp)
	CC = $(CXX)
else
	CFlags += -std=gnu99
endif

.phony: all clean distclean


all: $(binDir)/$(app)

$(binDir)/$(app): $(objects)
	@mkdir -p `dirname $@`
	@echo "Linking $@..."
	@$(CC) $(objects) $(LDFlags) -o $@
	cd lib; make;

$(objDir)/%.o: %.$(srcExt)
	@echo "Generating dependencies for $<..."
	@mkdir -p `dirname $(objDir)/$<`
	@$(call make-depend,$<,$@,$(subst .o,.d,$@))
	@echo "Compiling $<..."
	@$(CC) $(CFlags) $< -o $@

clean:
	@echo "Deleting object and dependencies files"
	@$(RM) -r $(objDir)

distclean: clean
	@echo "Deleting binary file $(binDir)/$(app)"
	@$(RM) -r $(binDir)/$(app)

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(srcDirs); \
   do \
	mkdir -p $(objDir)/$$dir; \
   done
endef

ifneq "$(MAKECMDGOALS)" "clean"
-include $(objects:.o=.d)
endif

# usage: $(call make-depend,source-file,object-file,depend-file)
define make-depend
  $(CC) -MM       \
        -MF $3    \
        -MP       \
        -MT $2    \
        $(CFlags) \
        $1
endef
