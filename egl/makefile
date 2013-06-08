OBJS=egl.o
BIN=egl.bin

LDFLAGS+=-L/opt/vc/lib/ -lGLESv2 -lEGL
INCLUDES+=-I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I./

all: $(BIN) $(LIB)

%.o: %.c
	@rm -f $@ 
	$(CC) $(CFLAGS) $(INCLUDES) -g -c $< -o $@ 

%.bin: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

clean:
	for i in $(OBJS); do (if test -e "$$i"; then ( rm $$i ); fi ); done
	@rm -f $(BIN) $(LIB)


