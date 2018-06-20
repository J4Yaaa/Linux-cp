a.out:mycp.c com_copy.c
	gcc -o $@ $^ -g
.PHONY:clean
clean:
	rm a.out
