PREFIX="/usr/local"

default:
	gcc *.c -g -DPREFIX=\"$(PREFIX)\" -Wall -pedantic -ansi -o scriptune
clean:
	rm *.o || rm scriptune

install:
	-rm -rf $(PREFIX)/share/scriptune
	-mkdir $(PREFIX)/share/scriptune 
	cp *.book *.layout $(PREFIX)/share/scriptune
	cp scriptune $(PREFIX)/share/scriptune
	chmod 444 $(PREFIX)/share/scriptune/*
	chmod 755 $(PREFIX)/share/scriptune/scriptune
	chown root:0 $(PREFIX)/share/scriptune/scriptune
	ln -sf $(PREFIX)/share/scriptune/scriptune $(PREFIX)/bin
	echo "Installation complete."
