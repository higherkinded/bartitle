EXE_NAME=bartitle
CC=clang
LD=clang
SRCDIR=src
BUILDDIR=build
DESTDIR=dist
INCLUDES=$(SRCDIR)
LIBS=-L/usr/lib   \
     -L/usr/lib64 \
     -L/lib       \
     -L/lib64     \
     -L/usr/local/lib
DEPS=-lX11 \
     -lc
CFLAGS=-I$(INCLUDES) \
       -O2 \
       -D_FORTIFY_SOURCE \
       -W\#pragma-messages

SRC := $(SRCDIR)/bartitle.c

# Flags
ifneq ($(BATTERY),)
	CFLAGS += -D_BATTERY_SUPPORT
	CFLAGS += -DBATTERY=\"$(BATTERY)\"
	SRC += $(SRCDIR)/battery.c
endif
ifneq ($(BATTERY_POLL_TICK),)
	CFLAGS += -D_BATTERY_POLL_TICK=$(BATTERY_POLL_TICK)
endif

OBJ := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SRC))

all:	dirs \
	$(DESTDIR)/bin/$(EXE_NAME)

$(DESTDIR)/bin/$(EXE_NAME): $(OBJ) $(DESTDIR)/bin
	$(LD) $(LDFLAGS) $(LIBS) $(DEPS) $(OBJ) -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

dirs: $(BUILDDIR) $(DESTDIR)/bin

$(BUILDDIR):
	install -d $(BUILDDIR)

$(DESTDIR):
	install -d $(DESTDIR)

$(DESTDIR)/bin: $(DESTDIR)
	install -d $(DESTDIR)/bin

.PHONY: clean
clean:
	rm -rf $(BUILDDIR) $(DESTDIR)
