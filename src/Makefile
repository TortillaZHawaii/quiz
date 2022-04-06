CC=${CROSS_COMPILE}gcc
OBJS=main.o
#LIBGDIOP_PATH="/malina/wysockid/lab2/buildroot-2022.02/output/build/libgpiod-1.6.3/include"
#LIBGDIOP_PATH="/home/tortilla/embedded/lab2/buildroot-2022.02/output/build/libgpiod-1.6.3/include"

# quiz: all

# all: ${OBJS}
# 	${CC} -o quiz -I${LIBGDIOP_PATH} ${CFLAGS} ${LDFLAGS} ${OBJS}

# ${OBJS}: %.o : %.c
# 	${CC} -c -Wall -I${LIBGDIOP_PATH} ${CFLAGS} -lgpiod $< -o $@

# clean:
# 	rm *.o quiz
quiz: all

all: ${OBJS}
	@echo Compiling quiz
	${CC} -o quiz ${CFLAGS} ${LDFLAGS} -lgpiod ${OBJS}

${OBJS}: %.o : %.c
	${CC} -c -Wall ${CFLAGS} $< -o $@

clean:
	rm *.o quiz

