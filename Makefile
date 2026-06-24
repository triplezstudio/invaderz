
# Keep in sync with the CI workflow
CPP_VERSION=20

NB_PROCS=8

debug:
	mkdir -p cmake-build/Debug && \
	cd cmake-build/Debug && \
	cmake \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_C_COMPILER_LAUNCHER=ccache \
		-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
		-S ../.. \
		-B . \
	&& \
	make -j ${NB_PROCS}

release:
	mkdir -p cmake-build/Release && \
	cd cmake-build/Release && \
	cmake \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_C_COMPILER_LAUNCHER=ccache \
		-DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
		-S ../.. \
		-B . \
	&& \
	make -j ${NB_PROCS}

clean:
	rm -rf cmake-build

cleanSandbox:
	rm -rf sandbox

copyData:
	mkdir -p sandbox/
	rsync -avH scripts/ sandbox/

copyDebug: debug copyData
	rsync -avH cmake-build/Debug/lib cmake-build/Debug/bin sandbox/

copyRelease: release copyData
	rsync -avH cmake-build/Release/lib cmake-build/Release/bin sandbox/

run: copyRelease
	cd sandbox && ./run.sh invaderz

drun: copyDebug
	cd sandbox && ./debug.sh invaderz

format:
	find src/ -iname '*.hh' -o -iname '*.cpp' | xargs clang-format -i
	find tests/ -iname '*.hh' -o -iname '*.cpp' | xargs clang-format -i
