all: 
	make -C hzeller
	npm update
	npm build .