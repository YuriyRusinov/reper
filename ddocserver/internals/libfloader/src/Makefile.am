CC = gcc
LD = gcc
CFLAGS  = -fpic -Wall $(PG_CFLAGS) $(DEFINES)
LIBDIR = -L$(shell pg_config --libdir)
LDFLAGS = -shared $(LIBDIR)  
RM      = rm -f
MV	= mv
CP      = cp -f
MKDIR	= mkdir -p
INSTALL = install --mode 755
build_dir = ../bld
tmp_dir = ../.tmp

OBJ     = $(tmp_dir)/testargs.o $(tmp_dir)/exec_spi.o $(tmp_dir)/text_to_cstring.o $(tmp_dir)/cstring_to_text.o \
	  $(tmp_dir)/rgetfile.o $(tmp_dir)/rgetfilebyurl.o $(tmp_dir)/rgetfilesizebyurl.o $(tmp_dir)/rwritefile.o \
	  $(tmp_dir)/rdeletefile.o $(tmp_dir)/rgetabsurl.o $(tmp_dir)/openfile.o \
	  $(tmp_dir)/xpm2jpeg.o $(tmp_dir)/rxpm2jpeg.o $(tmp_dir)/rand.o \
          $(tmp_dir)/hashfile.o $(tmp_dir)/rgethashfile.o

TARGET  = libfloader.so

all: $(TARGET)
	$(MKDIR) $(build_dir)
	$(MV) $< $(build_dir)

$(TARGET): $(OBJ)
	$(LD) $(LDFLAGS) $^ $(LIBDIR) -lpq -lm -ljpeg -lgsl -lgslcblas -lssl -lcrypto -o $@

$(tmp_dir)/%.o : %.c kksversion.h floader.h floader_config.h
	$(MKDIR) $(tmp_dir)
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

clean:
	$(RM) $(build_dir)/$(TARGET) $(tmp_dir)/*.o

install: $(TARGET)
	$(MKDIR) $(libdir)
	$(INSTALL) $(TARGET) $(libdir)

uninstall:
	$(RM) $(libdir)/$(TARGET)
