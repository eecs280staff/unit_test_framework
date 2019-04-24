CXX = g++
CPPFLAGS = -MMD -MP -I..
CXXFLAGS = -Wall -Wextra -pedantic --std=c++11

diff_test_cpps = $(wildcard test_*.diff.cpp)
diff_test_targets = $(diff_test_cpps:.cpp=)

run_test_cpps = $(wildcard test_*.run.cpp)
run_test_targets = $(run_test_cpps:.cpp=)

run_error_test_cpps = $(wildcard test_*.run_error.cpp)
run_error_test_targets = $(run_error_test_cpps:.cpp=)

compile_test_cpps = $(wildcard test_*.compile.cpp)
compile_test_targets = $(compile_test_cpps:.cpp=)

compile_error_test_cpps = $(wildcard test_*.compile_error.cpp)
compile_error_test_targets = $(compile_error_test_cpps:.cpp=)

all: $(diff_test_targets) \
	 $(run_test_targets) \
	 $(run_error_test_targets) \
	 $(compile_test_targets) \
	 $(compile_error_test_targets)

	# This test verifies that functions generated with TEST() are static.
	$(CXX) $(CXXFLAGS) -I.. \
					   test_name_conflict_file_with_non_static_func.cpp \
					   test_name_conflict_file_with_tests.cpp \
					   ../unit_test_framework.cpp \
					   -o no_conflict.exe

test_%.diff: test_%.diff.exe
	@test -f test_$*.diff.args || (echo "*** Missing .args file for test" $@ "***" && false)
	-./$< $$(cat test_$*.diff.args) > $@.out
	@diff -q $@.correct $@.out || sdiff $@.correct $@.out

test_%.diff.exe: test_%.diff.o ../unit_test_framework.o
	$(CXX) $(CXXFLAGS) $< ../unit_test_framework.o -o $@

test_%.run: test_%.run.exe
	./$<

test_%.run.exe: test_%.run.o ../unit_test_framework.o
	$(CXX) $(CXXFLAGS) $< ../unit_test_framework.o -o $@

test_%.run_error: test_%.run_error.exe
	! ./$<

test_%.run_error.exe: test_%.run_error.o ../unit_test_framework.o
	$(CXX) $(CXXFLAGS) $< ../unit_test_framework.o -o $@

test_%.compile: test_%.compile.o
	echo $@ "PASS"

test_%.compile_error:
	! $(CXX) $(CXXFLAGS) -c $@.cpp

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

.PHONY: all clean clean_all
clean:
	rm -vrf *.exe *.o *.ii *.dSYM *.out

clean_all: clean
	rm -vf *.d

.SECONDARY:

-include *.d
