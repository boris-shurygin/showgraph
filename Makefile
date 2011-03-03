#
# Makefile for ShowGraph project
#

# Variables
CC = gcc
CXX = g++
PERL = perl
MKDIR = mkdir
RM = rm
GREP = grep

#Directories
BIN_DIR := bin
OBJECT_DIR := objects
SOURCES := sources
DEBUG_OBJECTS_DIR := $(OBJECT_DIR)/debug
RELEASE_OBJECTS_DIR := $(OBJECT_DIR)/release

QT_DEBUG_DIR = /usr/local/Trolltech/Qt-4.7.0
QT_RELEASE_DIR = /usr/local/Trolltech/Qt-4.7.0

MOC = $(QT_RELEASE_DIR)/bin/moc
RCC = $(QT_RELEASE_DIR)/bin/rcc


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
DEBUG_LIB_NAMES = QtGuid QtCored QtXmld
RELEASE_LIB_NAMES = QtGui QtCore QtXml

DEBUG_LIBS = $(addprefix -l, $(DEBUG_LIB_NAMES))
DEBUG_LIB_FLAGS = -L$(QT_DEBUG_DIR)/lib
RELEASE_LIBS = $(addprefix -l, $(RELEASE_LIB_NAMES))
RELEASE_LIB_FLAGS = -L$(QT_RELEASE_DIR)/lib

SOURCES_CPP:= $(wildcard $(SOURCES)/*/*.cpp $(SOURCES)/*/*.c)
HEADERS:= $(wildcard $(SOURCES)/*/*.h)
MOCS:= $(HEADERS:.h=.moc)
RESOURCES:=$(wildcard $(SOURCES)/*/*.qrc)
RCCS:=$(RESOURCES:.qrc=.rcc)



RELEASE_SRC_NAMES= $(patsubst $(SOURCES)/%,$(RELEASE_OBJECTS_DIR)/%,$(SOURCES_CPP))
RELEASE_OBJS = $(RELEASE_SRC_NAMES:.cpp=.o)
RELEASE_DEPS = $(RELEASE_SRC_NAMES:.cpp=.d)
RELEASE_OBJS_GUI = $(filter-out $(RELEASE_OBJECTS_DIR)/console/% $(RELEASE_OBJECTS_DIR)/UnitTest/%,$(RELEASE_OBJS))
RELEASE_OBJS_CL = $(filter-out $(RELEASE_OBJECTS_DIR)/Gui/% $(RELEASE_OBJECTS_DIR)/UnitTest/%,$(RELEASE_OBJS))


#
# All targets
#
all: release debug unittest
	
#showgraphcl showgraphd showgraphcld

#Debug targets
debug:

# Release targets
release: showgraph showgraphcl

showgraph: $(RELEASE_OBJS_GUI)
	$(MKDIR) -p $(BIN_DIR)
	$(CXX) $(RELEASE_LIB_FLAGS) -o $(BIN_DIR)/$@ $(RELEASE_OBJS_GUI) $(RELEASE_LIBS)

showgraphcl: $(RELEASE_OBJS_CL)
	$(MKDIR) -p $(BIN_DIR)
	$(CXX) $(RELEASE_LIB_FLAGS) -o $(BIN_DIR)/$@ $(RELEASE_OBJS_CL) $(RELEASE_LIBS)

# Unit Test
unittest:

#
# This part generates new CPP files from headers that have Q_OBJECT usage
#
# run QT meta-object compiler on headers to generate additional .cpp files. 
# list of created files can be found in NEW_MOCS variable later
moc: $(MOCS) $(RCCS)

# rule for moc run
%.moc: %.h
	(! $(GREP) -q Q_OBJECT $< || $(MOC) $< -o $*_moc.cpp)
# rule for resource compiler
%.rcc: %.qrc
	$(RCC) $< -o $(@:.rcc=.cpp)

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
	$(eval EXISTING_RESOURCES = $(RESOURCES:.qrc=.cpp))
	-$(RM) -f $(EXISTING_RESOURCES)
	-$(RM) -f $(EXISTING_MOCS)
	-$(RM) -rf $(OBJECT_DIR)
	-$(RM) -rf $(BIN_DIR)

	



 
