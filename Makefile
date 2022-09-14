SRC = \
	  main.c \
	  gui.c \
	  drawing.c

OBJ = ${SRC:.c=.o}

INCS = -I. -I/usr/include -I/usr/include/gtk-3.0 -I/usr/include/glib-2.0 \
	   -I/usr/include/glib-2.0/glib/ \
	   -I/usr/lib/x86_64-linux-gnu/glib-2.0/include \
	   -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/cairo \
	   -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/atk-1.0

CFLAGS = -g -Wall ${INCS}

LIBS = `pkg-config --cflags --libs cairo gtk+-3.0 glib-2.0`

all: main

main: ${OBJ}
	cc -o $@ ${OBJ} ${LIBS}

.c.o:
	${CC} -c ${CFLAGS} $<

clean:
	rm -f main ${OBJ}

