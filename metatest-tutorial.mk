# This is used as a prefix for generated files
# e.g. <SUITE_NAME>_validity_check.exe
SUITE_NAME = tutorial

# A list of all #defines that trigger buggy implementations
ALL_BUGS = \
  SLIDE_RIGHT_BUG_1 \
  SLIDE_RIGHT_BUG_2 \
  SLIDE_RIGHT_BUG_3 \
  FLIP_BUG_1 \
  FLIP_BUG_2 \
  FLIP_BUG_3

# metatest-common.mk needs to come after the definition of these variables:
#   SUITE_NAME
#   ALL_BUGS
include metatest-common.mk

# Below are specific executables referred to by the targets in metatest-common.mk.
# Note the suite name (i.e. what is in the SUITE_NAME variable) must appear
# in the names of the executables below.

tutorial_validity_check.exe: tutorial_buggy_impls.o tutorial_tests.o
	$(LD) $(LDFLAGS) $^ -o $@

# Note that we want *_buggy_impls.cpp and not *_buggy_impls.o, since
# only with the former do we recompile with the preprocessor flag for the bug.
tutorial_bug_check_%.exe: tutorial_buggy_impls.cpp tutorial_tests.o
	$(CXX) $(CXXFLAGS) -D$* $^ -o $@
