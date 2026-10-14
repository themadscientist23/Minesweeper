.PHONY: gui terminal run-gui run-terminal clean

build/Makefile:
	cmake -B build

gui: build/Makefile
	cmake --build build --target minesweeper

terminal: build/Makefile
	cmake --build build --target terminal

run-gui: gui
	cd build && ./minesweeper

run-terminal: terminal
	cd build && ./terminal

clean:
	rm -rf build