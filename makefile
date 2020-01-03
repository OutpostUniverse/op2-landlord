# To see a list of available make targets from bash, type:
# make <space> <tab> <tab>


include makefile-generic.mk


# Set compiler to mingw (can still override from command line)
#config := mingw
config := clang


CPPFLAGS := -Inas2d-core/include/
CXXFLAGS := -std=c++17 -g -Wall -Wno-unknown-pragmas $(shell sdl2-config --cflags)
LDFLAGS := -static-libgcc -static-libstdc++ -Lnas2d-core/lib/
LDLIBS := -lstdc++fs -lnas2d -lphysfs -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lGL -lGLEW

.PHONY: op2landlord nas2d clean-nas2d clean-all-nas2d

all: op2landlord

op2landlord.exe: | nas2d

nas2d:
	+make -C nas2d-core/ CXX="$(CXX)"
clean-nas2d:
	make -C nas2d-core/ CXX="$(CXX)" clean
clean-all-nas2d:
	make -C nas2d-core/ CXX="$(CXX)" clean-all

clean: clean-nas2d
clean-all: clean-all-nas2d

$(eval $(call DefineCppProject,op2landlord,op2landlord.exe,src/))

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

