.PHONY: help
help:
	@grep -E '^[a-z.A-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-30s\033[0m %s\n", $$1, $$2}'

.PHONY: build
build: ## Build firmware on Debug mode
	cmake -GNinja -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-arm-none-eabi.cmake" -DCMAKE_BUILD_TYPE=debug -Bbuild/debug --fresh
	cmake --build build/debug

.PHONY: flash
flash: ## Flash firmware on Debug mode
	cmake --build build/debug --target flash

.PHONY: console
console: ## Launch console for firmware
	picocom --echo -b 115200 /dev/ttyACM0

.PHONY: test_suite
test_suite: ## Build and run the test suite
	cmake -GNinja -DCMAKE_TOOLCHAIN_FILE="cmake/toolchain-native.cmake" -DCMAKE_BUILD_TYPE=test -Bbuild/test --fresh
	cmake --build build/test
	ctest --test-dir build/test

.PHONY: coverage
coverage: ## Launch code coverage on test suite
	gcovr -r . --filter "src" --html-details -o build/test/code_coverage.html

.PHONY: format
format: ## Format source code only (no third-party or test)
	find src -type f \( -name "*.c" -o -name "*.h" \) -exec sh -c 'echo "Formatting file: $$1"; clang-format -i "$$1"' sh {} \;

.PHONY: static_analysis
static_analysis: ## Run static analysis on source code only
	cppcheck --project=build/debug/compile_commands.json --enable=all --addon=misra --inconclusive --suppress=missingIncludeSystem -i test -i third_party

.PHONY: documentation
documentation: ## Build the documentation for code base
	doxygen Doxyfile

.PHONY: mrproper
mrproper: ## Remove all workspace
	rm -rf build
