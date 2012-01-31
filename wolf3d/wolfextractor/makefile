#
# Project: Wolfenstein 3-D Redux
#
# Program: wolfextractor
#

COMPILER = gcc
CFLAGS = -Wall -c -O2
LDFLAGS = -s
ODIR = Release
                                                                                
EXEBASE = wolfextractor
EXE = $(ODIR)/$(EXEBASE)

DO_CC = $(COMPILER) $(CFLAGS) -o $@ -c $<       


all: $(EXE)




clean:
	rm -f $(ODIR)/*.o $(EXE)


PROGRAM_FILES = $(ODIR)/adlib.o \
		$(ODIR)/fmopl.o \
		$(ODIR)/arch.o \
		$(ODIR)/file.o \
		$(ODIR)/unix_file.o \
		$(ODIR)/glob.o \
		$(ODIR)/com_string.o \
		$(ODIR)/tga.o \
		$(ODIR)/wav.o \
		$(ODIR)/memory.o \
		$(ODIR)/mac.o \
		$(ODIR)/wl6_name.o \
		$(ODIR)/wolf_aud.o \
		$(ODIR)/wolf_gfx.o \
		$(ODIR)/wolf_map.o \
		$(ODIR)/wolf_pal.o \
		$(ODIR)/wolf_pm.o \
		$(ODIR)/zipfile.o \
		$(ODIR)/adler32.o \
		$(ODIR)/compress.o \
		$(ODIR)/crc32.o \
		$(ODIR)/deflate.o \
		$(ODIR)/trees.o \
		$(ODIR)/zutil.o \
		$(ODIR)/hq2x.o \
		$(ODIR)/pak.o \
		$(ODIR)/vorbisenc_inter.o \
		$(ODIR)/main.o



$(EXE): $(PROGRAM_FILES) 
	${COMPILER} -o $(EXE) $(PROGRAM_FILES) $(LDFLAGS) -lvorbis -lvorbisenc -logg -lm



#===========================================================================
#	Build
#===========================================================================
   
										

#
#	adlib/
#
$(ODIR)/adlib.o : adlib/adlib.c 
	$(DO_CC)

$(ODIR)/fmopl.o : adlib/fmopl.c 
	$(DO_CC)

#
#	common/
#
$(ODIR)/arch.o : ../../common/arch.c 
	$(DO_CC)

#
#	filesys/
#
$(ODIR)/file.o : filesys/file.c 
	$(DO_CC)

#
#	filesys/unix/
#
$(ODIR)/unix_file.o : filesys/unix/unix_file.c 
	$(DO_CC)

$(ODIR)/glob.o : filesys/unix/glob.c 
	$(DO_CC)

#
#	loaders/
#
$(ODIR)/tga.o : loaders/tga.c 
	$(DO_CC)

$(ODIR)/wav.o : loaders/wav.c 
	$(DO_CC)

#
#	mac/
#
$(ODIR)/mac.o : mac/mac.c 
	$(DO_CC)

#
#	memory/
#
$(ODIR)/memory.o : memory/memory.c 
	$(DO_CC)

#
#	string/
#
$(ODIR)/com_string.o : string/com_string.c 
	$(DO_CC)

#
#	wolf/
#
$(ODIR)/wl6_name.o : wolf/wl6_name.c 
	$(DO_CC)

$(ODIR)/wolf_aud.o : wolf/wolf_aud.c 
	$(DO_CC)

$(ODIR)/wolf_gfx.o : wolf/wolf_gfx.c 
	$(DO_CC)

$(ODIR)/wolf_map.o : wolf/wolf_map.c 
	$(DO_CC)

$(ODIR)/wolf_pal.o : wolf/wolf_pal.c 
	$(DO_CC)

$(ODIR)/wolf_pm.o : wolf/wolf_pm.c 
	$(DO_CC)

#
#	zip/
#
$(ODIR)/zipfile.o : zip/zipfile.c 
	$(DO_CC)

#
#	zlib/
#
$(ODIR)/adler32.o : ../../zlib/adler32.c 
	$(DO_CC)

$(ODIR)/compress.o : ../../zlib/compress.c 
	$(DO_CC)

$(ODIR)/crc32.o : ../../zlib/crc32.c 
	$(DO_CC)

$(ODIR)/deflate.o : ../../zlib/deflate.c 
	$(DO_CC)

$(ODIR)/trees.o : ../../zlib/trees.c 
	$(DO_CC)

$(ODIR)/zutil.o : ../../zlib/zutil.c 
	$(DO_CC)

#
#
#
$(ODIR)/hq2x.o : hq2x.c 
	$(DO_CC)

$(ODIR)/pak.o : pak.c 
	$(DO_CC)

$(ODIR)/vorbisenc_inter.o : vorbisenc_inter.c
	$(DO_CC)

$(ODIR)/main.o : main.c 
	$(DO_CC)




