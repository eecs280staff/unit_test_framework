CXX = g++
CPPFLAGS = -MMD -MP -I.
CXXFLAGS = -Wall -Wextra -pedantic --std=c++11

test_dir = test

test_exes = $(test_dir)/test_special_asserts.exe \
			$(test_dir)/end_to_end_test.exe

end_to_end_corrects = $(wildcard $(test_dir)/*.correct.txt)
end_to_end_targets = $(end_to_end_corrects:.correct.txt=.end_to_end)

all: $(test_exes)

test: $(test_exes) $(test_dir)/end_to_end_test.exe
	./$(test_dir)/test_special_asserts.exe

	$(CXX) $(CXXFLAGS) -I. \
					   $(test_dir)/test_name_conflict_file_with_non_static_func.cpp \
					   $(test_dir)/test_name_conflict_file_with_tests.cpp \
					   unit_test_framework.cpp \
					   -o no_conflict.exe

	$(call run_and_diff,$(test_dir)/end_to_end_test.exe,end_to_end_test)
	$(call run_and_diff,$(test_dir)/end_to_end_test.exe --show_test_names,show_test_names_test)

	$(call run_and_diff,$(test_dir)/end_to_end_test.exe --quiet,quiet_test)
	$(call run_and_diff,$(test_dir)/end_to_end_test.exe -q,quiet_test)

	$(call run_and_diff,$(test_dir)/end_to_end_test.exe should_pass,run_subset_of_tests_test)
	$(call run_and_diff,$(test_dir)/end_to_end_test.exe should_pass -q should_fail,quiet_run_subset_of_tests)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

$(test_dir)/%.exe: $(test_dir)/%.o unit_test_framework.o
	$(CXX) $(CXXFLAGS) $< unit_test_framework.o -o $@

# Params:
# 1. executable plus args
# 2. base for .correct.txt and .out.txt files
define run_and_diff
	-./$1 > $(test_dir)/$2.out.txt 2>&1;
	diff -q $(test_dir)/$2.correct.txt $(test_dir)/$2.out.txt || sdiff $(test_dir)/$2.correct.txt $(test_dir)/$2.out.txt;
endef

.PHONY: clean clean_all test
clean:
	rm -vrf */*.exe *.ii */*.o *.o *.dSYM */*.out.txt

clean_all: clean
	rm -vf *.d */*.d

.SECONDARY:

-include *.d
-include */*.d
