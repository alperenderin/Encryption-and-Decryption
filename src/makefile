LIBFDR="./libfdr"
TARGET=kripto

all: $(TARGET)

run:
	make clean 
	make
	./${TARGET} -e ./ornek_metin.txt ./encripted
	./${TARGET} -d ./encripted ./decripted

$(TARGET): ./libfdr/libfdr.a main.c
	gcc -I$(LIBFDR) main.c -o $@ -L$(LIBFDR) -lfdr

clean:
	rm -rf $(TARGET)

cleanall:
	rm -rf $(TARGET) .kilit encripted decripted *.txt