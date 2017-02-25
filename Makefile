CFLAGS=-Wall -Wpedantic -Werror -std=gnu++14 -Iinc/
HEADERS=$(wildcard inc/*.hpp)
TESTSRC=$(wildcard tests/*.cpp)
TESTS=$(TESTSRC:.cpp=)
TESTRUN=$(addprefix run_, $(notdir $(TESTS)))

.PHONY: run_tests all clean

all: run_tests

run_tests: $(TESTS)
	@echo "** Running tests..."
	@$(MAKE) --no-print-directory $(TESTRUN)

run_%: tests/%
	@$^ > /dev/null && echo \ * $@: pass || echo \ * $@: fail

tests/%: tests/%.cpp $(HEADERS)
	$(CXX) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(TESTS)

