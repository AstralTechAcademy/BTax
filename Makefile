# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/gabridc/clion-2021.1.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/gabridc/clion-2021.1.1/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/gabridc/Repositorio/Broker

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/gabridc/Repositorio/Broker

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/home/gabridc/clion-2021.1.1/bin/cmake/linux/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/home/gabridc/clion-2021.1.1/bin/cmake/linux/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/gabridc/Repositorio/Broker/CMakeFiles /home/gabridc/Repositorio/Broker//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/gabridc/Repositorio/Broker/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named Broker

# Build rule for target.
Broker: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 Broker
.PHONY : Broker

# fast build rule for target.
Broker/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/build
.PHONY : Broker/fast

#=============================================================================
# Target rules for targets named Broker_autogen

# Build rule for target.
Broker_autogen: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 Broker_autogen
.PHONY : Broker_autogen

# fast build rule for target.
Broker_autogen/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker_autogen.dir/build.make CMakeFiles/Broker_autogen.dir/build
.PHONY : Broker_autogen/fast

Broker.o: Broker.cpp.o

.PHONY : Broker.o

# target to build an object file
Broker.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Broker.cpp.o
.PHONY : Broker.cpp.o

Broker.i: Broker.cpp.i

.PHONY : Broker.i

# target to preprocess a source file
Broker.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Broker.cpp.i
.PHONY : Broker.cpp.i

Broker.s: Broker.cpp.s

.PHONY : Broker.s

# target to generate assembly for a file
Broker.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Broker.cpp.s
.PHONY : Broker.cpp.s

Broker_autogen/mocs_compilation.o: Broker_autogen/mocs_compilation.cpp.o

.PHONY : Broker_autogen/mocs_compilation.o

# target to build an object file
Broker_autogen/mocs_compilation.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Broker_autogen/mocs_compilation.cpp.o
.PHONY : Broker_autogen/mocs_compilation.cpp.o

Broker_autogen/mocs_compilation.i: Broker_autogen/mocs_compilation.cpp.i

.PHONY : Broker_autogen/mocs_compilation.i

# target to preprocess a source file
Broker_autogen/mocs_compilation.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Broker_autogen/mocs_compilation.cpp.i
.PHONY : Broker_autogen/mocs_compilation.cpp.i

Broker_autogen/mocs_compilation.s: Broker_autogen/mocs_compilation.cpp.s

.PHONY : Broker_autogen/mocs_compilation.s

# target to generate assembly for a file
Broker_autogen/mocs_compilation.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Broker_autogen/mocs_compilation.cpp.s
.PHONY : Broker_autogen/mocs_compilation.cpp.s

DBLocal.o: DBLocal.cpp.o

.PHONY : DBLocal.o

# target to build an object file
DBLocal.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/DBLocal.cpp.o
.PHONY : DBLocal.cpp.o

DBLocal.i: DBLocal.cpp.i

.PHONY : DBLocal.i

# target to preprocess a source file
DBLocal.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/DBLocal.cpp.i
.PHONY : DBLocal.cpp.i

DBLocal.s: DBLocal.cpp.s

.PHONY : DBLocal.s

# target to generate assembly for a file
DBLocal.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/DBLocal.cpp.s
.PHONY : DBLocal.cpp.s

Deposit.o: Deposit.cpp.o

.PHONY : Deposit.o

# target to build an object file
Deposit.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Deposit.cpp.o
.PHONY : Deposit.cpp.o

Deposit.i: Deposit.cpp.i

.PHONY : Deposit.i

# target to preprocess a source file
Deposit.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Deposit.cpp.i
.PHONY : Deposit.cpp.i

Deposit.s: Deposit.cpp.s

.PHONY : Deposit.s

# target to generate assembly for a file
Deposit.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Deposit.cpp.s
.PHONY : Deposit.cpp.s

Operation.o: Operation.cpp.o

.PHONY : Operation.o

# target to build an object file
Operation.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Operation.cpp.o
.PHONY : Operation.cpp.o

Operation.i: Operation.cpp.i

.PHONY : Operation.i

# target to preprocess a source file
Operation.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Operation.cpp.i
.PHONY : Operation.cpp.i

Operation.s: Operation.cpp.s

.PHONY : Operation.s

# target to generate assembly for a file
Operation.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Operation.cpp.s
.PHONY : Operation.cpp.s

OperationsModel.o: OperationsModel.cpp.o

.PHONY : OperationsModel.o

# target to build an object file
OperationsModel.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/OperationsModel.cpp.o
.PHONY : OperationsModel.cpp.o

OperationsModel.i: OperationsModel.cpp.i

.PHONY : OperationsModel.i

# target to preprocess a source file
OperationsModel.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/OperationsModel.cpp.i
.PHONY : OperationsModel.cpp.i

OperationsModel.s: OperationsModel.cpp.s

.PHONY : OperationsModel.s

# target to generate assembly for a file
OperationsModel.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/OperationsModel.cpp.s
.PHONY : OperationsModel.cpp.s

SQLManager.o: SQLManager.cpp.o

.PHONY : SQLManager.o

# target to build an object file
SQLManager.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/SQLManager.cpp.o
.PHONY : SQLManager.cpp.o

SQLManager.i: SQLManager.cpp.i

.PHONY : SQLManager.i

# target to preprocess a source file
SQLManager.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/SQLManager.cpp.i
.PHONY : SQLManager.cpp.i

SQLManager.s: SQLManager.cpp.s

.PHONY : SQLManager.s

# target to generate assembly for a file
SQLManager.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/SQLManager.cpp.s
.PHONY : SQLManager.cpp.s

Statistics.o: Statistics.cpp.o

.PHONY : Statistics.o

# target to build an object file
Statistics.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Statistics.cpp.o
.PHONY : Statistics.cpp.o

Statistics.i: Statistics.cpp.i

.PHONY : Statistics.i

# target to preprocess a source file
Statistics.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Statistics.cpp.i
.PHONY : Statistics.cpp.i

Statistics.s: Statistics.cpp.s

.PHONY : Statistics.s

# target to generate assembly for a file
Statistics.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Statistics.cpp.s
.PHONY : Statistics.cpp.s

Wallet.o: Wallet.cpp.o

.PHONY : Wallet.o

# target to build an object file
Wallet.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Wallet.cpp.o
.PHONY : Wallet.cpp.o

Wallet.i: Wallet.cpp.i

.PHONY : Wallet.i

# target to preprocess a source file
Wallet.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Wallet.cpp.i
.PHONY : Wallet.cpp.i

Wallet.s: Wallet.cpp.s

.PHONY : Wallet.s

# target to generate assembly for a file
Wallet.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/Wallet.cpp.s
.PHONY : Wallet.cpp.s

WalletsModel.o: WalletsModel.cpp.o

.PHONY : WalletsModel.o

# target to build an object file
WalletsModel.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/WalletsModel.cpp.o
.PHONY : WalletsModel.cpp.o

WalletsModel.i: WalletsModel.cpp.i

.PHONY : WalletsModel.i

# target to preprocess a source file
WalletsModel.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/WalletsModel.cpp.i
.PHONY : WalletsModel.cpp.i

WalletsModel.s: WalletsModel.cpp.s

.PHONY : WalletsModel.s

# target to generate assembly for a file
WalletsModel.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/WalletsModel.cpp.s
.PHONY : WalletsModel.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/main.cpp.s
.PHONY : main.cpp.s

qrc_qml.o: qrc_qml.cpp.o

.PHONY : qrc_qml.o

# target to build an object file
qrc_qml.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/qrc_qml.cpp.o
.PHONY : qrc_qml.cpp.o

qrc_qml.i: qrc_qml.cpp.i

.PHONY : qrc_qml.i

# target to preprocess a source file
qrc_qml.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/qrc_qml.cpp.i
.PHONY : qrc_qml.cpp.i

qrc_qml.s: qrc_qml.cpp.s

.PHONY : qrc_qml.s

# target to generate assembly for a file
qrc_qml.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Broker.dir/build.make CMakeFiles/Broker.dir/qrc_qml.cpp.s
.PHONY : qrc_qml.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... Broker_autogen"
	@echo "... Broker"
	@echo "... Broker.o"
	@echo "... Broker.i"
	@echo "... Broker.s"
	@echo "... Broker_autogen/mocs_compilation.o"
	@echo "... Broker_autogen/mocs_compilation.i"
	@echo "... Broker_autogen/mocs_compilation.s"
	@echo "... DBLocal.o"
	@echo "... DBLocal.i"
	@echo "... DBLocal.s"
	@echo "... Deposit.o"
	@echo "... Deposit.i"
	@echo "... Deposit.s"
	@echo "... Operation.o"
	@echo "... Operation.i"
	@echo "... Operation.s"
	@echo "... OperationsModel.o"
	@echo "... OperationsModel.i"
	@echo "... OperationsModel.s"
	@echo "... SQLManager.o"
	@echo "... SQLManager.i"
	@echo "... SQLManager.s"
	@echo "... Statistics.o"
	@echo "... Statistics.i"
	@echo "... Statistics.s"
	@echo "... Wallet.o"
	@echo "... Wallet.i"
	@echo "... Wallet.s"
	@echo "... WalletsModel.o"
	@echo "... WalletsModel.i"
	@echo "... WalletsModel.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
	@echo "... qrc_qml.o"
	@echo "... qrc_qml.i"
	@echo "... qrc_qml.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

