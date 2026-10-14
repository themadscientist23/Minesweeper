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
	./build/minesweeper

run-terminal: terminal
	./build/terminal

debug-gui: build/debug/Makefile
	cmake --build build/debug --target minesweeper
	lldb ./build/debug/minesweeper

debug-terminal: build/debug/Makefile
	cmake --build build/debug --target terminal
	lldb ./build/debug/terminal

clean:
	rm -rf build