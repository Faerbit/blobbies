all: linux web

linux:
	mkdir -p build-linux
	cd build-linux; [ -f CMakeCache.txt ] || cmake -DCMAKE_BUILD_TYPE=Release -G "Ninja" ..
	cd build-linux; ninja

web:
	mkdir -p build-web
	cd build-web; [ -f CMakeCache.txt ] || cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake -G "Ninja" ..
	cd build-web; ninja

clean:
	rm -rf build-linux
	rm -rf build-web
