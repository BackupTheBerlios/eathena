# $Id: Makefile,v 1.3 2004/02/01 16:11:53 rovert Exp $

CC = gcc -pipe
PACKETDEF = -DPACKETVER=4 -DNEW_006b
#PACKETDEF = -DPACKETVER=3 -DNEW_006b
#PACKETDEF = -DPACKETVER=2 -DNEW_006b
#PACKETDEF = -DPACKETVER=1 -DNEW_006b

PLATFORM = $(shell uname)

ifeq ($(findstring CYGWIN,$(PLATFORM)), CYGWIN)
OS_TYPE = -DCYGWIN
else
OS_TYPE =
endif

CFLAGS = -g -O2 -Wall -I../common $(PACKETDEF) $(OS_TYPE)

MKDEF = CC="$(CC)" CFLAGS="$(CFLAGS)"


all clean: common/GNUmakefile login/GNUmakefile char/GNUmakefile map/GNUmakefile
	cd common ; make $(MKDEF) $@ ; cd ..
	cd login ; make $(MKDEF) $@ ; cd ..
	cd char ; make $(MKDEF) $@ ; cd ..
	cd map ; make $(MKDEF) $@ ; cd ..

common/GNUmakefile: common/Makefile
	sed -e 's/$$>/$$^/' common/Makefile > common/GNUmakefile
login/GNUmakefile: login/Makefile
	sed -e 's/$$>/$$^/' login/Makefile > login/GNUmakefile
char/GNUmakefile: char/Makefile
	sed -e 's/$$>/$$^/' char/Makefile > char/GNUmakefile
map/GNUmakefile: map/Makefile
	sed -e 's/$$>/$$^/' map/Makefile > map/GNUmakefile
