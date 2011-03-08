#
# @file Makefile
# Makefile for ShowGraph project
# @author Boris Shurygin
#
# Main targets are:
#   all - build gui and console versinos of Showgraph in debug and release modes
#   debug (not yet implemented) - build debug vesion of ShowGraph (both gui and console)
#   release - build release version of ShowGraph
#
#   targets are buit in two steps:
#     1. Generate additional .cpp files by QT tools ( MOC and RCC)
#     2. Compile all .cpp to .o and link them with Qt libs
#


#Makefile that implements main building functionality
MAKEFILE_IMPL = Makefile-impl


# Tools
export CC = gcc
export CXX = g++
exportPERL = perl
export MKDIR = mkdir
export RM = rm
export GREP = grep
export TOUCH = touch

#Directories
export BIN_DIR := bin
export OBJECT_DIR := objects
export SOURCES := sources
export DEBUG_OBJECTS_DIR := $(OBJECT_DIR)/debug
export RELEASE_OBJECTS_DIR := $(OBJECT_DIR)/release
export QT_DIR = /usr/local/Trolltech/Qt-4.7.0

#QT tools
export MOC = $(QT_DIR)/bin/moc
export RCC = $(QT_DIR)/bin/rcc
	
HEADERS:= $(wildcard $(SOURCES)/*/*.h)
RELEASE_MOC_HEADERS:= $(patsubst $(SOURCES)/%,$(RELEASE_OBJECTS_DIR)/%,$(HEADERS))
RELEASE_MOCS= $(RELEASE_MOC_HEADERS:.h=.moc)
DEBUG_MOC_HEADERS:= $(patsubst $(SOURCES)/%,$(DEBUG_OBJECTS_DIR)/%,$(HEADERS))
DEBUG_MOCS= $(DEBUG_MOC_HEADERS:.h=.moc)


RESOURCES:=$(wildcard $(SOURCES)/*/*.qrc)
RELEASE_QRC:= $(patsubst $(SOURCES)/%,$(RELEASE_OBJECTS_DIR)/%,$(RESOURCES))
RELEASE_RCCS=$(RELEASE_QRC:.qrc=.rcc)
DEBUG_QRC:= $(patsubst $(SOURCES)/%,$(DEBUG_OBJECTS_DIR)/%,$(RESOURCES))
DEBUG_RCCS=$(RELEASE_QRC:.qrc=.rcc)
	
#
# All targets
#
all: release debug unittest
	
#showgraphcl showgraphd showgraphcld

#Debug targets
debug:

# Release targets
release: showgraph showgraphcl

# Unit Test
unittest:

showgraph: release_moc showgraph_impl

showgraphcl: release_moc showgraphcl_impl

showgraph_impl showgraphcl_impl:
	$(MAKE) -f $(MAKEFILE_IMPL) $@

#
# This part generates new CPP files from headers that have Q_OBJECT usage
#
# run QT meta-object compiler on headers to generate additional .cpp files.
# list of created files can be found in NEW_MOCS variable later
release_moc: $(RELEASE_MOCS) $(RELEASE_RCCS)

# rule for moc run
$(RELEASE_OBJECTS_DIR)/%.moc: $(SOURCES)/%.h
	$(MKDIR) -p $(dir $@)
	$(TOUCH) $@
	(! $(GREP) -q Q_OBJECT $< || $(MOC) $< -o $(SOURCES)/$*_moc.cpp)
# rule for resource compiler
$(RELEASE_OBJECTS_DIR)/%.rcc: $(SOURCES)/%.qrc
	$(MKDIR) -p $(dir $@)
	$(TOUCH) $@
	$(RCC) $< -o $(@:.rcc=.cpp)
	
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

	



 
