
EXEC        = rule_net
BUILDDIR    = build
TRUNKROOT   = ./ src src/Nodes

INC  = . /usr/local/include include/ include/Nodes include/Strategies include/Rules include/WME ./

######################### 3 P P #########################

################ A L L   T O G E T H E R ################
INCLUDE     = $(addprefix -I, $(INC))

LDFLAGS		= -L/usr/local/lib
LIBS		= -lnsl -lpthread -lboost_system -lboost_filesystem -lboost_thread

C_FLAGS     = -std=c++17 -g -Wall -ftemplate-backtrace-limit=0 $(INCLUDE) $(DEFINES)
CC		    = g++

TRUNKMODLIST := ./ src src/Nodes src/Rules

SRC := . src src/Nodes src/Rules $(foreach srcdir,$(TRUNKROOT)/src,$(addprefix $(strip $(srcdir))/, $(TRUNKMODLIST)))

s := $(addsuffix /*.cpp, $(SRC))

OBJS := $(addprefix $(BUILDDIR)/,$(notdir $(patsubst %.cpp,%.o,$(wildcard $(s)))))

all: |pre $(EXEC)

$(EXEC): $(OBJS)
	@echo "Building $@"
	@$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)

pre:
	@echo
	@echo "================== Compile flags ======================"
	@echo
	@echo "$(C_FLAGS)"
	@echo
	@echo "==================== Link flags ======================="
	@echo
	@echo "$(LDFLAGS) $(LIBS)"
	@echo
	@mkdir -p $(BUILDDIR)

VPATH   := $(SRC)

$(BUILDDIR)/%.o: %.cpp
	@echo "Compiling $<"
	@$(CC) $(C_FLAGS) -c -MD $< -o $@

include $(wildcard build/*.d)

clean:
	@rm -rf $(EXEC) $(BUILDDIR)/
