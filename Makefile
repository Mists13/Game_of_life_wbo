THIS_MAKEFILE   = $(lastword $(MAKEFILE_LIST))
BASE_DIR        = $(dir $(realpath $(THIS_MAKEFILE)))
BUILD_DIR       = $(BASE_DIR)build

.PHONY: all clean config format

all: $(BUILD_DIR)
	$(MAKE) -C $(BUILD_DIR) $@ 2> build_errors.log

clean:
	@echo "-- Cleaning up"
	rm -rf $(BUILD_DIR) bin
	rm -rf $$(find $(BASE_DIR) -name "*~")

config: $(BUILD_DIR)
	$(MAKE) edit_cache

format:
	clang-format -i src/*

$(BUILD_DIR):
	@echo "-- Creating build directory: $@"
	mkdir -p $@
	(cd $@; cmake -DCMAKE_CXX_FLAGS="-std=c++17 -g" $(BASE_DIR))
