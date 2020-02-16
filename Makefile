# Build kernel
.PHONY: build
build:
	mkdir build
	cp -r kernel/* build
	cd build
	make

.PHONY: run
run:
	build


# Cleanup
.PHONY: clean
clean:
	rm -r build
