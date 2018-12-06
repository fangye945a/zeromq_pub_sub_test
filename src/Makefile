TARGET := ../bin/linux/zmq_sub_pub_PC
CC := gcc
INCLUDE:= -I ../include

LIBS:= -L ../lib/linux/ -Wl,-Bstatic -lzmq  -Wl,-Bdynamic -lpthread -lstdc++ -lm

CSRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(CSRCS))

$(TARGET): $(OBJS) 
	$(CC) $+ -o $@  $(LIBS)
%.o:%.c
	$(CC) -c -g $< -o $@ $(INCLUDE)

clean:
	rm -rf $(OBJS) $(TARGET)
	#rm *.o
