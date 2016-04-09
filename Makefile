
CC		:= gcc
CFLAGS		:= -Wall -Wextra -O3
LDFLAGS		:=

SRCDIR		:= src
INCDIR		:= inc
BUILDDIR	:= build-make
CEDIR		:= vendor/ceedling/vendor/c_exception/lib/
OUT		:= $(BUILDDIR)/simulator

SRC		:= $(wildcard $(SRCDIR)/*)
CESRC		:= $(wildcard $(CEDIR)/*.c)
OBJS		:= $(addprefix $(BUILDDIR)/,$(notdir $(SRC:.c=.o)))
CEOBJS		:= $(addprefix $(BUILDDIR)/,$(notdir $(CESRC:.c=.o)))

all: $(OUT)

$(OUT): $(OBJS) $(CEOBJS) | $(BUILDDIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILDDIR):
	@mkdir -p $@

$(OBJS): $(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(CEDIR) -DCEXCEPTION_USE_CONFIG_FILE -c $< -o $@

$(CEOBJS): $(BUILDDIR)/%.o: $(CEDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(CEDIR) -DCEXCEPTION_USE_CONFIG_FILE -c $< -o $@

.PHONY: clean
clean:
	rm -rf $(BUILDDIR)
