#
# Makefile for ShowGraph project
#

# Variables
CC = gcc
CXX = g++
PERL = perl
MOC = moc
MKDIR = mkdir
RM = rm
GREP = grep

#Directories
BIN_DIR := bin
OBJECT_DIR := objects
SOURCES := sources
DEBUG_OBJECTS_DIR := $(OBJECT_DIR)/debug
RELEASE_OBJECTS_DIR := $(OBJECT_DIR)/release

#ifeq ($(TOOLSET), mingw)
  QT_DEBUG_DIR = /c/QT/
  QT_RELEASE_DIR = /c/QtStatic/qt
#else
#  QT_DEBUG_DIR = 
#  QT_RELEASE_DIR = 
#endif

#Compiler configureations

#Includes
QT_INCLUDE_DIRS = include \
                  include/Qt \
                  include/QtGui \
                  include/QtXml \
                  include/QtCore

RELEASE_INCLUDE_FLAGS = $(addprefix -I$(QT_RELEASE_DIR)/, $(QT_INCLUDE_DIRS))
DEBUG_INCLUDE_FLAGS = $(addprefix -I$(QT_DEBUG_DIR)/, $(QT_INCLUDE_DIRS))

# Final debug and release flags
DEBUG_CPPFLAGS = -D_DEBUG
RELEASE_CPPFLAGS = $(RELEASE_INCLUDE_FLAGS)

# Library sets for debug and release
DEBUG_LIBS = qtmaind.lib QtGuid4.lib QtCored4.lib QtXmld4.lib
RELEASE_LIB_NAMES = qtmain QtGui QtCore QtXml imm32 winmm ws2_32 qico
RELEASE_LIBS = $(addprefix -l, $(RELEASE_LIB_NAMES))
RELEASE_LIB_FLAGS = -L$(QT_RELEASE_DIR)/lib -L$(QT_RELEASE_DIR)/plugins/imageformats

SOURCES_CPP:= $(wildcard $(SOURCES)/*/*.cpp $(SOURCES)/*/*.c)
HEADERS:= $(wildcard $(SOURCES)/*/*.h)
MOCS:= $(HEADERS:.h=.moc)
RELEASE_SRC_NAMES= $(patsubst $(SOURCES)/%,$(RELEASE_OBJECTS_DIR)/%,$(SOURCES_CPP))
RELEASE_OBJS = $(RELEASE_SRC_NAMES:.cpp=.o)
RELEASE_DEPS = $(RELEASE_SRC_NAMES:.cpp=.d)
RELEASE_DIRS = $(dir $RELEASE_OBJS)
RELEASE_OBJS_GUI= $(filter-out $(SOURCES)/console/%,$(RELEASE_OBJS))

all: showgraph
	
#showgraphcl showgraphd showgraphcld

showgraph: $(RELEASE_OBJS_GUI)
	$(CXX) -static $(RELEASE_LIB_FLAGS) -o $(BIN_DIR)/$@ $(RELEASE_OBJS_GUI) $(RELEASE_LIBS)

#
# This part generates new CPP files from headers that have Q_OBJECT usage
#
# run QT meta-object compiler on headers to generate additional .cpp files. 
# list of created files can be found in NEW_MOCS variable later
moc: $(MOCS)

# rule for moc run
%.moc: %.h
	(! $(GREP) -q Q_OBJECT $< || $(MOC) $< -o $*_moc.cpp)

#
# Rules that run CPP compiler
#

	
#Dependency generation
$(RELEASE_OBJECTS_DIR)/%.d: $(SOURCES)/%.cpp
	$(MKDIR) -p $(dir $@)
	$(CXX) -MM $(RELEASE_CPPFLAGS) $< -MF $@ -MT "$@ $(@:.d=.o)"
	
include $(RELEASE_DEPS)	

#Objects generation
$(RELEASE_OBJECTS_DIR)/%.o: $(SOURCES)/%.cpp
	$(MKDIR) -p $(dir $@)
	$(CXX) -c $(RELEASE_CPPFLAGS) $< -o $@

#
# Cleanup routines
#
.PHONY: clean

clean:
	$(eval EXISTING_MOCS = $(wildcard $(SOURCES)/*/*_moc.cpp))
	-$(RM) -f $(EXISTING_MOCS)
	-$(RM) -rf $(OBJECT_DIR)

	



 