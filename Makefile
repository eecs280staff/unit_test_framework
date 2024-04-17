CXX ?= g++
CPPFLAGS ?= -MMD -MP -I.
CXXFLAGS ?= -Wall -Wextra -Werror -pedantic --std=c++11
CXXFLAGS_NO_WARNINGS ?= $(filter-out -W%,$(CXXFLAGS))

test_dir = test

test_exes = $(test_dir)/test_special_asserts.exe \
			$(test_dir)/end_to_end_test.exe \
			$(test_dir)/test_early_exit.exe \
			$(test_dir)/pairs_and_sequences.exe \
			$(test_dir)/sequence_equal.exe \
			$(test_dir)/size_t_and_int.exe \
			$(test_dir)/string_equal.exe \
			$(test_dir)/char_array.exe \
			$(test_dir)/multiple_files.exe

end_to_end_corrects = $(wildcard $(test_dir)/*.correct.txt)
end_to_end_targets = $(end_to_end_corrects:.correct.txt=.end_to_end)

all: test

test: $(test_exes:.exe=.run) $(test_dir)/end_to_end_test.exe
	./$(test_dir)/test_special_asserts.exe
	! ./$(test_dir)/test_early_exit.exe

	$(CXX) $(CXXFLAGS) -I. \
					   $(test_dir)/test_name_conflict_file_with_non_static_func.cpp \
					   $(test_dir)/test_name_conflict_file_with_tests.cpp \
					   -o $(test_dir)/no_conflict.exe

	! $(CXX) $(CXXFLAGS_NO_WARNINGS) -I. $(test_dir)/array_compare_error.cpp
	! $(CXX) $(CXXFLAGS_NO_WARNINGS) -I. $(test_dir)/missing_overload_error.cpp

	$(call run_and_diff,$(test_dir)/end_to_end_test.exe,end_to_end_test)
	$(call run_and_diff,$(test_dir)/end_to_end_test.exe --show_test_names,show_test_names_test)

	$(call run_and_diff,$(test_dir)/end_to_end_test.exe --quiet,quiet_test)
	$(call run_and_diff,$(test_dir)/end_to_end_test.exe -q,quiet_test)

	$(call run_and_diff,$(test_dir)/end_to_end_test.exe should_pass,run_subset_of_tests_test)
	$(call run_and_diff,$(test_dir)/end_to_end_test.exe should_pass -q should_fail,quiet_run_subset_of_tests)
	$(call run_and_diff,$(test_dir)/pairs_and_sequences.exe,pairs_and_sequences)
	$(call run_and_diff,$(test_dir)/sequence_equal.exe,sequence_equal)
	$(call run_and_diff,$(test_dir)/string_equal.exe,string_equal)
	./$(test_dir)/size_t_and_int.exe
	$(call run_and_diff,$(test_dir)/char_array.exe,char_array)

	$(CXX) $(CXXFLAGS) -DUNIT_TEST_ENABLE_REGEXP -I. \
					   $(test_dir)/end_to_end_test.cpp \
					   -o $(test_dir)/end_to_end_test_regexp.exe
	$(call run_and_diff,$(test_dir)/end_to_end_test_regexp.exe -e ".*almost.*",regexp_test1)
	$(call run_and_diff,$(test_dir)/end_to_end_test_regexp.exe --regexp ".*almost.*" "should.*",regexp_test2)

	@echo TESTS PASSED

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -c -o $@

$(test_dir)/%.exe: $(test_dir)/%.o
	$(CXX) $(CXXFLAGS) $< -o $@

$(test_dir)/multiple_files.exe: $(test_dir)/end_to_end_test.o $(test_dir)/no_main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(test_dir)/%.run: $(test_dir)/%.exe
	$(call run_and_diff,$<,$*)

# Params:
# 1. executable plus args
# 2. base for .correct.txt and .out.txt files
define run_and_diff
	-./$1 | sed "s/0x[0-9a-fA-F]*/<HEX VALUE>/g" > $(test_dir)/$2.out.txt 2>&1;
	diff -q $(test_dir)/$2.correct.txt $(test_dir)/$2.out.txt || sdiff $(test_dir)/$2.correct.txt $(test_dir)/$2.out.txt;
endef

.PHONY: clean clean_all test
clean:
	rm -vrf *.exe *.o *.ii */*.exe *.ii */*.o *.o *.dSYM */*.out.txt

clean_all: clean
	rm -vf *.d */*.d

.SECONDARY:

-include *.d
-include */*.d
