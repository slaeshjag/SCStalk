default:
	mkdir -p bin
	cd src && make

clean:
	rm -Rf bin
	cd src && make clean

