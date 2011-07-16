#
# @file Makefile
# Makefile for ShowGraph project
# @author Boris Shurygin
#
# Main targets are:
#   all - build gui and console versinos of Showgraph in debug and release modes
#   debug (not yet implemented) - build debug vesion of ShowGraph (both gui and console)
#   release - build release version of ShowGraph
#   doc - run doxygen to generate documentation from source code
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
export DOXYGEN = doxygen

#Directories
export BIN_DIR := bin
export OBJECT_DIR := objects
export SOURCES := sources
export MOC_DIR = $(OBJECT_DIR)/mocs
export DEBUG_OBJECTS_DIR := $(OBJECT_DIR)/debug
export RELEASE_OBJECTS_DIR := $(OBJECT_DIR)/release
export QT_DIR = /usr/local/Trolltech/Qt-4.7.0

#QT tools
export MOC = $(QT_DIR)/bin/moc
export RCC = $(QT_DIR)/bin/rcc
	
HEADERS:= $(wildcard $(SOURCES)/*/*.h $(SOURCES)/Core/*/*.h)
MOC_HEADERS:= $(patsubst $(SOURCES)/%,$(MOC_DIR)/%,$(HEADERS))
MOCS= $(MOC_HEADERS:.h=.moc)

RESOURCES:=$(wildcard $(SOURCES)/*/*.qrc)
QRC:= $(patsubst $(SOURCES)/%,$(MOC_DIR)/%,$(RESOURCES))
RCCS=$(QRC:.qrc=.rcc)
	
#
# All targets
#
all: release debug unittest
	
#showgraphcl showgraphd showgraphcld

#Debug targets
debug: showgraphd showgraphcld

showgraphd: gen showgraphd_impl

showgraphcld: gen showgraphcld_impl

showgraphd_impl showgraphcld_impl:
	$(MAKE) -f $(MAKEFILE_IMPL) $@

# Release targets
release: showgraph showgraphcl

# Unit Test
unittest:

showgraph: gen showgraph_impl

showgraphcl: gen showgraphcl_impl

showgraph_impl showgraphcl_impl:
	$(MAKE) -f $(MAKEFILE_IMPL) $@

#
# This part generates new CPP files from headers that have Q_OBJECT usage
#
# run QT meta-object compiler on headers to generate additional .cpp files.
# list of created files can be found in NEW_MOCS variable later
gen: $(MOCS) $(RCCS)

# rule for moc run
$(MOC_DIR)/%.moc: $(SOURCES)/%.h
	@echo [moc] $*.h
	@$(MKDIR) -p $(dir $@)
	@$(TOUCH) $@
	@(! $(GREP) -q Q_OBJECT $< || $(MOC) $< -o $(SOURCES)/$*_moc.cpp)
# rule for resource compiler
$(MOC_DIR)/%.rcc: $(SOURCES)/%.qrc
	@echo [rcc] $*.qrc
	@$(MKDIR) -p $(dir $@)
	@$(TOUCH) $@
	@$(RCC) $< -o $(<:.qrc=.cpp)
	
#
# Documentation
#
doc: 
	@echo [doxygen]
	@cd autodoc;$(DOXYGEN) Doxyfile
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

	



 
