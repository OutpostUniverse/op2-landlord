# To see a list of available make targets from bash, type:
# make <space> <tab> <tab>


include makefile-generic.mk


# Set compiler toolchain (default, gcc, clang, mingw)
config := default


PkgConfig := pkg-config
ImguiIncludeFlags = $(shell type $(PkgConfig) >/dev/null 2>&1 && $(PkgConfig) --cflags-only-I imgui)

CPPFLAGS := $(ImguiIncludeFlags) -I OP2Utility/include
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

# Docker and CircleCI commands
$(eval $(call DefineDockerImage,.circleci/,outpostuniverse/gcc-mingw-wine-googletest-circleci,1.2))
$(eval $(call DefineCircleCi))


ifdef Outpost2Path

.PHONY: install run install-testModule run-testModule

install: $(Outpost2Path)op2landlord.exe

$(Outpost2Path)op2landlord.exe: op2landlord.exe
	cp op2landlord.exe "$(Outpost2Path)"

run: install
	pushd "$(Outpost2Path)" && wine op2landlord.exe ; popd

endif

