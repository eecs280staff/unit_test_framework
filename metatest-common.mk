# The intent is for this makefile to provide common functionality that can be
# included in a specific makefile for each lab/project that provides additional
# targets.

# Compiler and flags. Need to be provided here as the metatest makefiles
# are not always used via recursive calls to make from the AG makefile.
export CXX ?= g++
export CXXFLAGS ?= --std=c++17 -Wall -pedantic -g -Wno-sign-compare -Wno-comment

# Compiler and flags. Need to be provided here as the metatest makefiles
# are not always used via recursive calls to make from the AG makefile.
# Note that we expliclty set LD here so that it doesn't default to ld rather than g++.
export LD := $(CXX)
export LDFLAGS ?= -Wall

# Set student_test at the command line when running the validity_check
# target. If this value is not set, all the student tests will be run.  Set
# student_test and bug_name at the command line when running the bug_check
# target.  If student_test is not set, all the student tests will be run.
student_test =

# Test student unit tests against correct instructor solution.  This tests for
# the absence of false positives, which we sometimes call a validity check.
# Test student unit tests against buggy implementations, AKA true positives.
# The recipe will only succeed if ALL buggy implemenations are caught by at
# least one of the student unit tests.
metatest: validity_check verify_all_bugs_caught

# This intentionally has no dependencies
get_test_names:
	-@test -f $(SUITE_NAME)_validity_check.exe && ./$(SUITE_NAME)_validity_check.exe --show_test_names

validity_check: $(SUITE_NAME)_validity_check.exe
	@./$(SUITE_NAME)_validity_check.exe $(student_test)
	@echo "****************************************"
	@echo "VALIDITY CHECK PASS ($(SUITE_NAME)): student unit tests pass on instructor solution"

bug_check_%: $(SUITE_NAME)_bug_check_%.exe
	./$< $(student_test)

verify_all_bugs_caught: $(patsubst %, verify_caught_%, $(ALL_BUGS))
	@echo "****************************************"
	@echo "BUG CATCHING PASS ($(SUITE_NAME)): student unit tests caught ALL instructor bugs"

verify_caught_%: $(SUITE_NAME)_bug_check_%.exe
	! ./$< $(student_test)

.SECONDARY:

.PHONY: clean metatest get_test_names validity_check bug_check verify_all_bugs_caught

# Compile one object
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@

# Verify variables set by metatest-*.mk
ifndef SUITE_NAME
  $(error SUITE_NAME is not set)
endif
ifndef ALL_BUGS
  $(error ALL_BUGS is not set)
endif
