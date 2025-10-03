# Компилятор и флаги
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic -Wno-self-assign-overloaded
GTEST_FLAGS := $(shell pkg-config --cflags --libs gtest)
# Фолбэк для macOS/сред, где pkg-config gtest недоступен
ifeq ($(strip $(GTEST_FLAGS)),)
  GTEST_FLAGS := -lgtest -lgtest_main -pthread
endif

TEST_SRCS := tests/test_containers.cpp tests/test_containersplus.cpp $(wildcard tests/cases_*.cpp)
BIN_DIR  := bin
TEST_TARGET := $(BIN_DIR)/tests

REPORT_DIR := report_gcovr
REPORT_FILE = $(REPORT_DIR)/report.html
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    OPEN_REPORT = xdg-open $(REPORT_FILE)
		LEAKS_CMD := CK_FORK=no valgrind --tool=memcheck --leak-check=full --track-origins=yes -s $(TEST_TARGET)
else
    OPEN_REPORT = open $(REPORT_FILE)
		LEAKS_CMD := CK_FORK=no leaks --atExit -- $(TEST_TARGET)
endif

REPORT_BINARY = $(REPORT_DIR)/report.out
COVERAGE_FLAGS = -fprofile-arcs -ftest-coverage -O0

# Цели
.PHONY: all test smoke clean leaks

all: test

rebuild: clean all

test: $(TEST_TARGET)
	@echo "== Running unit tests =="
	@$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(GTEST_FLAGS) -o $@

clean:
	@rm -rf $(BIN_DIR) $(REPORT_DIR)

# Coverage report (gcovr)
clean_coverage_files:
	@find . -name "*.gcno" -type f -delete
	@find . -name "*.gcda" -type f -delete
	@rm -rf $(REPORT_DIR)

compile_coverage: clean_coverage_files
	@mkdir -p $(REPORT_DIR)
	# Компиляция всех файлов проекта и тестов с флагами COVERAGE_FLAGS
	$(CXX) $(CXXFLAGS) $(COVERAGE_FLAGS) $(TEST_SRCS) -o $(REPORT_BINARY) $(GTEST_FLAGS)

gcov_report: compile_coverage
	@echo "== Running tests for coverage report =="
	@$(REPORT_BINARY)
	
	@gcovr -r . --exclude '.*tests/.*' --html --html-details -o $(REPORT_FILE)
	@$(OPEN_REPORT)
	

# Misc
clang:
	find . -name "*.cpp" -exec clang-format -i --style=Google {} \;
	find . -name "*.h" -exec clang-format -i --style=Google {} \;

nclang:
	find . -name "*.cpp" -exec clang-format -n --style=Google {} \;
	find . -name "*.h" -exec clang-format -n --style=Google {} \;

devinstall:
	$(INSTALL_CMD) install build-essential libgtest-dev gcovr pkg-config

devuninstall:
	$(INSTALL_CMD) remove build-essential libgtest-dev gcovr pkg-config

leaks: $(TEST_TARGET)
	@$(LEAKS_CMD)
