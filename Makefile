.PHONY: front wasm build dev clean format

FORMATTER = clang-format

BACK_DIR = back
FRONT_DIR = front
BUILD_DIR = back/build

all: wasm front

wasm:
	cd $(BACK_DIR) && emcmake cmake -B build
	cd $(BACK_DIR) && cmake --build build

front:
	cd $(FRONT_DIR) && npm install && npm run build

clean:
	rm -rf $(BACK_DIR)/build
	rm -rf $(FRONT_DIR)/build

build: wasm front

dev:
	cd $(FRONT_DIR) && npm run dev

format:
	find $(BACK_DIR) -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) -exec $(FORMATTER) -i {} \;