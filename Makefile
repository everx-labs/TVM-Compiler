PROJECTS_SOURCE_DIR := llvm/projects/ton-compiler
TVM_LINKER_DIR := TVM-linker
TVM_LINKER_RELEASE_DIR := target/release
BUILD_DIR := build
# INSTALL_PREFIX := ../install

all: tvm-linker llvm-compiler

clean: clean-tvm-linker clean-llvm-compiler
	
create-install-path:
	mkdir -p $(INSTALL_PREFIX)
	mkdir -p $(INSTALL_PREFIX)/bin

clean-tvm-linker:
	if [ -d "$(PROJECTS_SOURCE_DIR)/$(TVM_LINKER_DIR)" ]; then \
	  rm -rf $(PROJECTS_SOURCE_DIR)/$(TVM_LINKER_DIR); \
	fi;

tvm-linker: create-install-path
	cd $(PROJECTS_SOURCE_DIR) && \
	git clone http://github.com/tonlabs/$(TVM_LINKER_DIR)/
	cd $(PROJECTS_SOURCE_DIR)/$(TVM_LINKER_DIR) && \
	cargo update && cargo build --release -j8
	cp $(PROJECTS_SOURCE_DIR)/$(TVM_LINKER_DIR)/$(TVM_LINKER_RELEASE_DIR)/tvm_linker $(INSTALL_PREFIX)/bin

#install: $(PROJECTS_SOURCE_DIR)/$(TVM_LINKER_DIR)/$(TVM_LINKER_RELEASE_DIR)
#	@true

#.PHONY: install

clean-llvm-compiler:
	if [ -d "$(BUILD_DIR)" ]; then \
	  rm -rf $(BUILD_DIR); \
	fi;

llvm-compiler: llvm-configure llvm-build-install

llvm-configure:
	mkdir $(BUILD_DIR)
	cd $(BUILD_DIR) && \
	cmake -DCMAKE_INSTALL_PREFIX=../$(INSTALL_PREFIX) -DLLVM_ENABLE_PROJECTS=clang -C ../cmake/Cache/ton-compiler.cmake ../llvm

llvm-build-install: create-install-path
	cd $(BUILD_DIR) && \
	cmake --build . --target install -- -j8
