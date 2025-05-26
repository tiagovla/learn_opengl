all:
	cmake \
		-DFETCHCONTENT_QUIET=OFF \
		-DFETCHCONTENT_BASE_DIR=.cache/cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-B build -G Ninja .
	cmake --build build
	./build/main
