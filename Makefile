all: hw-01_bmp

obj:
	mkdir obj

hw-01_bmp: obj obj/main.o obj/bmp.o obj/stego.o obj/list.o
	gcc -Wall -Wextra -Werror obj/main.o obj/bmp.o obj/stego.o obj/list.o -o hw-01_bmp

obj/bmp.o: src/bmp.c include/bmp.h
	gcc -Wall -Wextra -Werror -I include -c src/bmp.c -o obj/bmp.o

obj/main.o: src/main.c include/list.h include/bmp.h include/stego.h 
	gcc -Wall -Wextra -Werror -I include -c src/main.c -o obj/main.o

obj/stego.o: src/stego.c include/bmp.h include/stego.h include/list.h
	gcc -Wall -Wextra -Werror -I include -c src/stego.c -o obj/stego.o

obj/list.o: src/list.c include/list.h 
	gcc -Wall -Wextra -Werror -I include -c src/list.c -o obj/list.o

clean:
	rm -rf obj
	rm -f hw-01_bmp

.PHONY: clean
