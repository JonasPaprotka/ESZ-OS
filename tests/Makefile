# File updated with AI / AI Assisted

CXX ?= c++

DOCTEST_VERSION ?= 2.5.3
DOCTEST := vendor/doctest.h

KERNEL := ../kernel
BUILD  := build
BIN    := $(BUILD)/tests

INC := -iquote vendor -iquote support -iquote $(KERNEL)/types/string -iquote $(KERNEL)/types/char

CXXFLAGS ?= -std=c++17 -Wall -Wextra -g -O0

SRC := libraries/test_string.cpp \
       $(KERNEL)/types/string/string.cpp \
       $(KERNEL)/types/char/char.cpp

.PHONY: test coverage clean

test: $(BIN)
	./$(BIN)

$(BIN): $(SRC) $(DOCTEST)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $(INC) $(SRC) -o $@

$(DOCTEST):
	@mkdir -p $(dir $@)
	curl -L -o $@ https://raw.githubusercontent.com/doctest/doctest/v$(DOCTEST_VERSION)/doctest/doctest.h

coverage: $(SRC) $(DOCTEST)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) -fprofile-instr-generate -fcoverage-mapping $(INC) $(SRC) -o $(BIN)
	-LLVM_PROFILE_FILE=$(BUILD)/tests.profraw ./$(BIN)
	xcrun llvm-profdata merge -sparse $(BUILD)/tests.profraw -o $(BUILD)/tests.profdata
	xcrun llvm-cov export ./$(BIN) -instr-profile=$(BUILD)/tests.profdata -format=lcov \
	    $(KERNEL)/types/string/string.cpp $(KERNEL)/types/char/char.cpp > $(BUILD)/lcov.info

clean:
	rm -rf $(BUILD) vendor
