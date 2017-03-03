CFLAGS=-Wall -Wpedantic -Werror -std=gnu++14 -Iinc/
HEADERS=$(wildcard inc/*.hpp)
TESTSRC=$(wildcard tests/*.cpp)
TESTS=$(TESTSRC:.cpp=)
TESTRUN=$(addprefix run_, $(notdir $(TESTS)))
TESTVAL=$(addprefix val_, $(notdir $(TESTS)))

.PHONY: run_tests all clean docs

all: run_tests

run_tests: $(TESTS)
	@echo "** Running tests..."
	@$(MAKE) --no-print-directory $(TESTRUN)

valgrind_tests: $(TESTS)
	@echo "** Running tests w/ valgrind..."
	@$(MAKE) --no-print-directory $(TESTVAL)

run_%: tests/%
	@$^ > /dev/null && echo \ * $@: pass || echo \ * $@: fail

val_%: tests/%
	@valgrind -q --error-exitcode=1 --leak-check=full $^ > /dev/null && echo \ * $@: pass || echo \ * $@: fail

tests/%: tests/%.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -o $@ $<

docs:
	mkdir -p docs
	doxygen Doxyfile

clean:
	rm -rf $(TESTS) docs

