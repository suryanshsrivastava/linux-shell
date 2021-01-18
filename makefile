TARGETS = shell

all: $(TARGETS)
%o: %.c %.h
	gcc -g -c -o $@ $<
shell: shell.c prompt.c cd.c pwd.c echo.c ls.c exec.c pinfo.c history.c ioredir.c signals.c setenv.c jobs.c def.h
	$(CC) $(CFLAGS) -o $@ $^
	
.PHONY: clean
clean:
	rm -f $(TARGETS) *.o