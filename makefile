# To see a list of available make targets from bash, type:
# make <space> <tab> <tab>


include makefile-generic.mk


# Set compiler toolchain (default, gcc, clang, mingw)
config := default


ImguiIncludeFlags = -I/usr/include/imgui -I/usr/include/imgui/backends

CPPFLAGS := $(ImguiIncludeFlags) -I OP2Utility/OP2Utility/include
CXXFLAGS_WARN := -Wall -Wno-unknown-pragmas -Wfloat-conversion
CXXFLAGS := -std=c++20 -g $(CXXFLAGS_WARN) $(shell sdl2-config --cflags)
LDFLAGS := -static-libgcc -static-libstdc++ -LOP2Utility/
LDLIBS := -lstdc++fs -lOP2Utility -limgui -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lGL -lGLEW

OP2UtilityLib := OP2Utility/libOP2Utility.a

.PHONY: op2landlord op2utility clean-op2utility clean-all-op2utility

all: op2landlord

op2landlord.exe: | $(OP2UtilityLib)

$(OP2UtilityLib): | op2utility

op2utility:
	+make -C OP2Utility/ CXX="$(CXX)"
clean-op2utility:
	+make -C OP2Utility/ CXX="$(CXX)" clean
clean-all-op2utility:
	+make -C OP2Utility/ CXX="$(CXX)" clean-all

clean: clean-op2utility
clean-all: clean-all-op2utility

$(eval $(call DefineCppProject,op2landlord,op2landlord.exe,OP2-Landlord/))


ifdef Outpost2Path

.PHONY: install run install-testModule run-testModule

install: $(Outpost2Path)op2landlord.exe

$(Outpost2Path)op2landlord.exe: op2landlord.exe
	cp op2landlord.exe "$(Outpost2Path)"

run: install
	pushd "$(Outpost2Path)" && wine op2landlord.exe ; popd

endif

