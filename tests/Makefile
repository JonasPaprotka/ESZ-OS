# File updated with AI / AI Assisted

ifeq ($(origin CXX),default)
CXX := c++
endif

include ../versions.env

DOCTEST := vendor/doctest.h

KERNEL := ../kernel
BUILD  := build

MAKEFLAGS += --no-print-directory

CXXFLAGS ?= -std=c++20 -Wall -Wextra -g -O0
COVFLAGS := -fprofile-instr-generate -fcoverage-mapping

LLVM_PROFDATA ?= $(shell command -v llvm-profdata >/dev/null 2>&1 && echo llvm-profdata || echo 'xcrun llvm-profdata')
LLVM_COV ?= $(shell command -v llvm-cov >/dev/null 2>&1 && echo llvm-cov || echo 'xcrun llvm-cov')
DEPFLAGS := -MMD -MP

ifdef SAN
override CXXFLAGS += -fsanitize=address,undefined -fno-omit-frame-pointer
BUILD := $(BUILD)/san
endif

##########
# Not addable: keymap paging args commands pci pit timer idt pic keyboard terminal filesystem storage screenBuffer print clear info_text print_helper heap pmm
KERNEL_MODULES := string char memory math bit integer utf16 pci_class_names mbr bitmap
##########

KERNEL_SRC := $(sort $(foreach m,$(KERNEL_MODULES),$(shell find $(KERNEL) -name '$(m).cpp')))
KERNEL_HDR := $(sort $(foreach m,$(KERNEL_MODULES),$(shell find $(KERNEL) -name '$(m).h')))
TEST_SRC   := $(sort $(shell find . -name 'test_*.cpp' -not -path './$(BUILD)/*'))

KERNEL_OBJ := $(patsubst $(KERNEL)/%.cpp,$(BUILD)/kobj/%.o,$(KERNEL_SRC))
TEST_OBJ   := $(patsubst ./%.cpp,$(BUILD)/tobj/%.o,$(TEST_SRC))
TEST_BIN   := $(patsubst ./%.cpp,$(BUILD)/bin/%,$(TEST_SRC))
COV_BIN    := $(patsubst ./%.cpp,$(BUILD)/cov/%,$(TEST_SRC))

NAME_OF = $(subst /test_,/,$(patsubst $(BUILD)/bin/%,%,$(1)))

ifdef TEST
RUN_BIN := $(strip $(foreach b,$(TEST_BIN),$(if $(findstring $(TEST),$(call NAME_OF,$(b))),$(b))))
else
RUN_BIN := $(TEST_BIN)
endif

RUN_ARGS := $(if $(CASE),-tc=*$(CASE)*) $(ARGS)

RUN_PAIRS := $(foreach b,$(RUN_BIN),$(b):$(call NAME_OF,$(b)))

INC := -iquote vendor -iquote support $(addprefix -iquote ,$(shell find $(KERNEL) -type d))

.PHONY: test coverage compile_commands list list-cases help clean distclean

test: $(RUN_BIN)
	@if [ -z "$(RUN_BIN)" ]; then echo "no test binary matches TEST=$(TEST)"; exit 1; fi; \
	if [ -t 1 ]; then \
	    col=--force-colors; \
	    dim=`printf '\033[2m'`;    grn=`printf '\033[32;1m'`; \
	    red=`printf '\033[31;1m'`; rst=`printf '\033[0m'`; \
	fi; \
	w=0; for p in $(RUN_PAIRS); do n=$${p#*:}; if [ $${#n} -gt $$w ]; then w=$${#n}; fi; done; \
	failed=; files=0; cases=0; checks=0; bad_cases=0; bad_checks=0; \
	for p in $(RUN_PAIRS); do \
	    b=$${p%%:*}; n=$${p#*:}; files=$$((files + 1)); \
	    out=`./$$b $$col --no-intro --no-version $(RUN_ARGS) 2>&1`; rc=$$?; \
	    set -- `printf '%s\n' "$$out" | awk -F'[:|]' \
	        '/test cases:/ { c = $$2; cf = $$4 } \
	         /assertions:/ { a = $$2; af = $$4 } \
	         END { print c + 0, cf + 0, a + 0, af + 0 }'`; \
	    cases=$$((cases + $$1)); bad_cases=$$((bad_cases + $$2)); \
	    checks=$$((checks + $$3)); bad_checks=$$((bad_checks + $$4)); \
	    if [ $$rc -eq 0 ]; then \
	        printf '  %sPASS%s %-*s  %s%4d cases  %6d checks%s\n' \
	            "$$grn" "$$rst" "$$w" "$$n" "$$dim" "$$1" "$$3" "$$rst"; \
	    else \
	        printf '  %sFAIL%s %-*s  %s%4d cases  %6d checks%s\n' \
	            "$$red" "$$rst" "$$w" "$$n" "$$red" "$$1" "$$3" "$$rst"; \
	        printf '%s\n' "$$out" | sed 's/^/       /'; \
	        failed="$$failed $$n"; \
	    fi; \
	done; \
	printf '  %s------------------------------------------------------------%s\n' "$$dim" "$$rst"; \
	if [ -z "$$failed" ]; then \
	    printf '  %sPASS%s %s%d files, %d cases, %d checks%s\n' \
	        "$$grn" "$$rst" "$$dim" "$$files" "$$cases" "$$checks" "$$rst"; \
	else \
	    printf '  %sFAIL%s %s%d of %d cases, %d of %d checks%s\n' \
	        "$$red" "$$rst" "$$dim" "$$bad_cases" "$$cases" "$$bad_checks" "$$checks" "$$rst"; \
	    for n in $$failed; do printf '       %s%s%s\n' "$$red" "$$n" "$$rst"; done; \
	fi; \
	[ -z "$$failed" ]

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
	@$(CXX) --version 2>/dev/null | grep -qi clang || \
	    { echo "coverage needs a clang based CXX (got: $(CXX))"; exit 1; }
	@mkdir -p $(BUILD)/cov
	@rm -f $(BUILD)/cov/*.profraw
	@for src in $(TEST_SRC); do \
	    bin=$(BUILD)/cov/$${src#./}; bin=$${bin%.cpp}; \
	    mkdir -p $$(dirname $$bin); \
	    $(CXX) $(CXXFLAGS) $(COVFLAGS) $(INC) $$src $(KERNEL_SRC) -o $$bin || exit 1; \
	    LLVM_PROFILE_FILE=$$bin.profraw ./$$bin -q || true; \
	done
	$(LLVM_PROFDATA) merge -sparse $$(find $(BUILD)/cov -name '*.profraw') -o $(BUILD)/tests.profdata
	$(LLVM_COV) export $(firstword $(COV_BIN)) \
	    $(addprefix -object ,$(wordlist 2,999,$(COV_BIN))) \
	    -instr-profile=$(BUILD)/tests.profdata -format=lcov \
	    $(KERNEL_SRC) $(KERNEL_HDR) > $(BUILD)/lcov.info

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

list-cases: $(RUN_BIN)
	@for p in $(RUN_PAIRS); do \
	    echo "$${p#*:}:"; \
	    ./$${p%%:*} -ltc --no-intro --no-version \
	        | grep -v '^\[doctest\]' | sed '/^$$/d; /^=*$$/d; s/^/  /'; \
	done

help:
	@echo "make test                  run every test binary"
	@echo "make test TEST=string      only binaries whose path contains 'string'"
	@echo "make test CASE=str_length  only test cases matching '*str_length*'"
	@echo "make test ARGS='-s -d'     raw doctest flags (ARGS=--help lists them)"
	@echo "make list / list-cases     show test files / test case names"
	@echo "make coverage              write build/lcov.info"
	@echo "make clean / distclean     drop build/ (and vendor/)"

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -rf vendor

-include $(TEST_OBJ:.o=.d) $(KERNEL_OBJ:.o=.d)
