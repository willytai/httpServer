STATIC ?= false

ECHO   = /bin/echo
EXEC   = httpServer
DIRS   = bin

all: directory

directory:
	@for dir in $(DIRS); \
	do \
		if [ ! -d $$dir ]; then \
			mkdir $$dir; \
			echo "Creating directory \"$$dir\" ..."; \
		fi; \
	done
	@$(MAKE) -C src -f Makefile.src --no-print-directory EXEC=$(EXEC) STATIC=$(STATIC);
	@ln -fs bin/$(EXEC)

clean:
	@$(MAKE) -C src -f Makefile.src --no-print-directory clean;
	@rm -rf $(EXEC)
