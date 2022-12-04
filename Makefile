MODULES = src

all:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} all); \
	done

clean:
	for dir in $(MODULES); do \
		(cd $$dir; ${MAKE} clean); \
	done

install:
	mkdir -p $(shell pwd)/bin
	cp -af src/signal-translator bin/
