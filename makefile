#Directories
SRCDIR = src
TSGLSRCDIR = src/TSGL
VISPRJDIR = visualizations
TESTDIR = tests
BINDIR = bin
BLDDIR = build

#Header Files
HEADERS := $(wildcard $(SRCDIR)/*.h) $(wildcard $(TSGLSRCDIR)/*.h)
# TODO depend on headers!!!

#Library Files
TSGLSRC := $(wildcard $(TSGLSRCDIR)/*.cpp)  $(SRCDIR)/glad.cpp
TSGLOBJ := $(addprefix build/, $(TSGLSRC:.cpp=.o))
TSGLDEP := $(TSGLOBJ:.o=.d)

#Tester Program Files
PRGMSRC := $(wildcard $(SRCDIR)/main.cpp)
PRGMOBJ := $(addprefix build/, $(PRGMSRC:.cpp=.o))
PRGMDEP := $(PRGMOBJ:.o=.d)

#Reader/Writer Program Files
RWSRC := $(wildcard $(VISPRJDIR)/ReaderWriter/*.cpp)
RWOBJ := $(addprefix build/, $(RWSRC:.cpp=.o))
RWDEP := $(RWOBJ:.o=.d)

#Dining Philosophers Program Files
DPHSRC := $(wildcard $(VISPRJDIR)/DiningPhilosophers/*.cpp)
DPHOBJ := $(addprefix build/, $(DPHSRC:.cpp=.o))
DPHDEP := $(DPHOBJ:.o=.d)

#Producer/Consumer Program Files
PCSRC := $(wildcard $(VISPRJDIR)/ProducerConsumer/*.cpp)
PCOBJ := $(addprefix build/, $(PCSRC:.cpp=.o))
PCDEP := $(PCOBJ:.o=.d)

#Tests subfolders
TESTFOLDERS := $(notdir $(shell find $(TESTDIR)/ -maxdepth 1 -type d))

#Tests
TESTPRGMS := $(basename $(notdir $(wildcard $(TESTDIR)/*.cpp)))
TESTSRC := $(wildcard $(TESTDIR)/*.cpp)
TESTOBJ := $(addprefix build/, $(TESTSRC:.cpp=.o))
TESTDEP := $(TESTOBJ:.o=.d)


# OS Check
UNAME := $(shell uname)



# Commands
ifeq ($(UNAME), Linux)
CC = g++
RM = rm -f
endif

ifeq ($(UNAME), Darwin)
CC = g++-7
RM = rm -f
endif



#Linker flags
# Linux
ifeq ($(UNAME), Linux)
#Flags for linking programs built with TSGL
LFLAGS = \
	-L/usr/local/lib \
	-L/usr/lib \
	-fopenmp -lfreetype -ldl -lm -lglfw -ltsgl

#Flags for linking the TSGL shared library
LIBLFLAGS = \
	-L/usr/local/lib \
	-L/usr/lib \
	-shared -fopenmp -lfreetype -ldl -lm -lglfw
endif


# MacOS
ifeq ($(UNAME), Darwin)
LFLAGS = \
	-Llib/ -L/usr/local/lib  -L/usr/X11/lib/ \
	-lfreetype -lGLEW -lglfw -lX11  -lXrandr -fopenmp -ltsgl\
	-framework Cocoa -framework OpenGl -framework IOKit -framework Corevideo
endif

#Compiler flags
CFLAGS = \
	-std=c++0x \
	-D__GXX_EXPERIMENTAL_CXX0X__ \
	-I/usr/local/include/Cellar/glfw3/3.1.1/include/ \
	-Iinclude/ \
	-Isrc/ \
	-Isrc/TSGL/ \
	-I/usr/include/ \
	-I/usr/local/include/ \
	-I/usr/local/include/freetype2 \
	-I/usr/local/include/freetype2/freetype \
	-I/opt/AMDAPP/include/ \
	-I/usr/include/c++/4.6/ \
	-I/usr/include/c++/4.6/x86_64-linux-gnu/ \
	-I/usr/lib/gcc/x86_64-linux-gnu/4.6/include/ \
	-I/usr/include/freetype2 \
	-I/usr/include/freetype2/freetype \
	-I./ \
	-I../include/TSGL/ \
	-O0 -g3 -Wall -Wextra -c -fmessage-length=0 -fopenmp \
	-Wno-unused-parameter -Wno-unused-function -Wno-narrowing -fpermissive -fPIC -pthread




all: lib tester ReaderWriter DiningPhilosophers ProducerConsumer sftests $(TESTFOLDERS)

vis: ReaderWriter DiningPhilosophers ProducerConsumer






#Shared Object Library
lib: lib/libtsgl.so

install: lib
	@echo ""
	@tput setaf 6;
	@echo "//////////////////// Installing TSGL Library ////////////////////"
	@echo ""
	@echo "Removing old versions of TSGL..."
	@tput setaf 2;
	@sudo rm -f /usr/local/lib/libtsgl.so
	@sudo install lib/libtsgl.so /usr/local/lib/
	@echo "Library installed at /usr/local/lib/libtsgl.so"
	@sudo cp -r src/TSGL /usr/local/include
	@sudo rm -rf /usr/local/include/TSGL/*.cpp
	@sudo ldconfig
	@tput sgr0;

lib/libtsgl.so: $(TSGLOBJ) $(HEADERS)
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking TSGL Library ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(TSGLOBJ) $(LIBLFLAGS) -o lib/libtsgl.so










#Test Program
tester: $(PRGMOBJ) $(HEADERS) install
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Tester Program ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(PRGMOBJ) -o $(BINDIR)/$(notdir $(@)) $(LFLAGS)

#Reader Writer
ReaderWriter: $(RWOBJ) install
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Reader/Writer Visualization ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(RWOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

#Dining Philosophers
DiningPhilosophers: $(DPHOBJ) install
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Dining Philosophers Visualization ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(DPHOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))

#Producer Consumer
ProducerConsumer: $(PCOBJ) install
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Producer/Consumer Visualization ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(PCOBJ) $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))



#Build all single-file tests
sftests: $(TESTPRGMS)

#Single-file test programs
$(TESTPRGMS): %: $(BLDDIR)/tests/%.o install
	@echo ""
	@tput setaf 3;
	@echo "//////////////////// Linking Test Program: $(@) ////////////////////"
	@tput sgr0;
	@echo ""
	$(CC) $(BLDDIR)/tests/$(@).o $(TSGLOBJ) $(LFLAGS) -o $(BINDIR)/$(notdir $(@))


#Tests with multiple files
$(TESTFOLDERS): install
	cd tests/$@ && make


# Include header dependencies
-include $(TSGLDEP)
-include $(PRGMDEP)
-include $(RWDEP)
-include $(DPHDEP)
-include $(PCDEP)
-include $(TESTDEP)

#Building rule for cpp to o
$(BLDDIR)/%.o: %.cpp
	@echo ""
	@tput setaf 3;
	@echo "+++++++++++++++++++ Building $@ +++++++++++++++++++"
	@tput sgr0;
	@echo ""
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -MF $(patsubst %.o,%.d,$@) $< -o $@


# $(EXECUTABLE): $(OBJECTS)
# 	echo "--------------Linking Files--------------"
# 	$(CC) $(OBJECTS) $(LFLAGS)
#
# .cpp.o:
# 	echo $@
# 	$(CC) $(CFLAGS) $< -o build/$(notdir $@)
#

clean:
		$(RM) -r bin/* build/* lib/* tutorial/docs/html/* *~ *# *.tmp


#Linking
