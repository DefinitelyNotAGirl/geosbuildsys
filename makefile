CC = gcc
CXX = g++-12
ld = ld
as = as

OUT=OUTPUT_NOT_CONFIGURED
build=build
OPT=-O0
DEPFLAGS_GPP=-MP -MD
DEPFLAGS_ASM=-M -MD

INCDIRS=include
BINARY=buildExec

LD_ARGS=
NOWARN=-Wno-literal-suffix
GPP_ARGS=$(NOWARN) $(OPT) $(DEPFLAGS_GPP) $(foreach D,$(INCDIRS),-I$(D))
AS_ARGS=$(DEPFLAGS_ASM)

SOURCE_CPP=$(wildcard src/*.cpp)
SOURCE_ASM=$(wildcard src/*.s)
OBJECTS_CPP=$(patsubst src/%.cpp,$(build)/%.o,$(SOURCE_CPP))
OBJECTS_ASM=$(patsubst src/%.s,$(build)/%.o,$(SOURCE_ASM))
DEPFILES_CPP=$(patsubst src/%.cpp,$(build)/%.d,$(SOURCE_CPP))
DEPFILES_ASM=$(patsubst src/%.s,$(build)/%.d,$(SOURCE_ASM))

all: $(BINARY)

$(BINARY): $(OBJECTS_CPP) $(OBJECTS_ASM)
	$(LD) $(LD_ARGS) -o $@ $^
	$(info [BUILD] compiled module: $(shell basename $(CURDIR)))

bin/%.o: src/%.cpp
	$(GPP) $(GPP_ARGS) -c -o $@ $<

bin/%.o: src/%.s
	$(AS) $(AS_ARGS) -c -o $@ $<

# include dependencies
-include $(DEPFILES_CPP) $(DEPFILES_ASM)

clean:
	@-rm $(DEPFILES_CPP) $(DEPFILES_ASM) $(OBJECTS_CPP) $(OBJECTS_ASM) $(BINARY)