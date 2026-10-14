.PHONY: gui terminal run-gui run-terminal debug-gui debug-terminal clean

build/Makefile:
	cmake -B build

build/debug/Makefile:
	cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug

gui: build/Makefile
	cmake --build build --target minesweeper

terminal: build/Makefile
	cmake --build build --target terminal

run-gui: gui
	cd build && ./minesweeper

run-terminal: terminal
	cd build && ./terminal

debug-gui: build/debug/Makefile
	cmake --build build/debug --target minesweeper
	cd build/debug && lldb ./minesweeper

debug-terminal: build/debug/Makefile
	cmake --build build/debug --target terminal
	cd build/debug && lldb ./terminal

clean:
	rm -rf build