# File updated with AI / AI Assisted

ifeq ($(origin CXX),default)
CXX := c++
endif

DOCTEST_VERSION ?= 2.5.3
DOCTEST := vendor/doctest.h

KERNEL := ../kernel
BUILD  := build

##########
# Not addable: keymap paging args commands pci pit timer idt pic keyboard terminal filesystem storage screenBuffer print clear info_text print_helper heap pmm
KERNEL_MODULES := string char memory math bit integer utf16 pci_class_names mbr bitmap
##########

KERNEL_SRC := $(foreach m,$(KERNEL_MODULES),$(shell find $(KERNEL) -name '$(m).cpp'))

TEST_SRC := $(shell find . -name 'test_*.cpp' -not -path './$(BUILD)/*')

KERNEL_OBJ := $(patsubst $(KERNEL)/%.cpp,$(BUILD)/kobj/%.o,$(KERNEL_SRC))
TEST_OBJ   := $(patsubst ./%.cpp,$(BUILD)/tobj/%.o,$(TEST_SRC))
TEST_BIN   := $(patsubst ./%.cpp,$(BUILD)/bin/%,$(TEST_SRC))
COV_BIN    := $(patsubst ./%.cpp,$(BUILD)/cov/%,$(TEST_SRC))

INC := -iquote vendor -iquote support $(addprefix -iquote ,$(shell find $(KERNEL) -type d))

CXXFLAGS ?= -std=c++20 -Wall -Wextra -g -O0
COVFLAGS := -fprofile-instr-generate -fcoverage-mapping
DEPFLAGS := -MMD -MP

.PHONY: test coverage compile_commands list clean distclean

test: $(TEST_BIN)
	@fail=0; \
	for b in $(TEST_BIN); do \
	    echo "===== $$b"; \
	    ./$$b || fail=1; \
	done; \
	exit $$fail

$(BUILD)/bin/%: $(BUILD)/tobj/%.o $(KERNEL_OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/tobj/%.o: %.cpp $(DOCTEST)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INC) -c $< -o $@

$(BUILD)/kobj/%.o: $(KERNEL)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) $(INC) -c $< -o $@

$(DOCTEST):
	@mkdir -p $(dir $@)
	curl -L -o $@ https://raw.githubusercontent.com/doctest/doctest/v$(DOCTEST_VERSION)/doctest/doctest.h

coverage: $(DOCTEST)
	@mkdir -p $(BUILD)/cov
	@rm -f $(BUILD)/cov/*.profraw
	@for src in $(TEST_SRC); do \
	    bin=$(BUILD)/cov/$${src#./}; bin=$${bin%.cpp}; \
	    mkdir -p $$(dirname $$bin); \
	    $(CXX) $(CXXFLAGS) $(COVFLAGS) $(INC) $$src $(KERNEL_SRC) -o $$bin || exit 1; \
	    LLVM_PROFILE_FILE=$$bin.profraw ./$$bin || true; \
	done
	xcrun llvm-profdata merge -sparse $$(find $(BUILD)/cov -name '*.profraw') -o $(BUILD)/tests.profdata
	xcrun llvm-cov export $(firstword $(COV_BIN)) \
	    $(addprefix -object ,$(wordlist 2,999,$(COV_BIN))) \
	    -instr-profile=$(BUILD)/tests.profdata -format=lcov \
	    $(KERNEL_SRC) > $(BUILD)/lcov.info

compile_commands:
	@echo '[' > ../compile_commands.json
	@sep="%s"; for f in $(TEST_SRC) $(KERNEL_SRC); do \
	    printf "$$sep" "" >> ../compile_commands.json; \
	    printf '  {\n    "directory": "%s",\n    "file": "%s",\n    "command": "%s"\n  }' \
	        "$(CURDIR)" "$$f" "$(CXX) $(CXXFLAGS) $(INC) -c $$f" >> ../compile_commands.json; \
	    sep=",\n%s"; \
	done
	@echo '' >> ../compile_commands.json
	@echo ']' >> ../compile_commands.json
	@echo "wrote ../compile_commands.json"

list:
	@echo "tests:"; for t in $(TEST_SRC); do echo "  $$t"; done
	@echo "kernel sources:"; for s in $(KERNEL_SRC); do echo "  $$s"; done

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf vendor

-include $(TEST_OBJ:.o=.d) $(KERNEL_OBJ:.o=.d)
