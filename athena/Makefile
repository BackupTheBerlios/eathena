# $Id: Makefile,v 1.6 2004/03/09 01:46:42 mjflick Exp $

CC = gcc -pipe
PACKETDEF = -DPACKETVER=5 -DNEW_006b
#PACKETDEF = -DPACKETVER=4 -DNEW_006b
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


all: common/GNUmakefile login/GNUmakefile char/GNUmakefile map/GNUmakefile tool/Makefile
	cd common && make $(MKDEF) $@ && cd ..
	cd login && make $(MKDEF) $@ && cd ..
	cd char && make $(MKDEF) $@ && cd ..
	cd map && make $(MKDEF) $@ && cd ..

tools:
	cd tool && make $(MKDEF) && cd ..
	$(CC) -o setupwizard setupwizard.c

clean:
	cd common && make $(MKDEF) $@ && cd ..
	cd login && make $(MKDEF) $@ && cd ..
	cd char && make $(MKDEF) $@ && cd ..
	cd map && make $(MKDEF) $@ && cd ..
	cd tool && make $(MKDEF) $@ && cd ..
	rm -f setupwizard
	rm -f *.exe
	cd mysql_login && rm -f *.o && cd ..
	cd mysql_char && rm -f *.o && cd ..

sql:
	cd mysql_login && sh make.sh && cd ..
	cd mysql_char && sh make.sh && cd ..	

common/GNUmakefile: common/Makefile
	sed -e 's/$$>/$$^/' common/Makefile > common/GNUmakefile
login/GNUmakefile: login/Makefile
	sed -e 's/$$>/$$^/' login/Makefile > login/GNUmakefile
char/GNUmakefile: char/Makefile
	sed -e 's/$$>/$$^/' char/Makefile > char/GNUmakefile
map/GNUmakefile: map/Makefile
	sed -e 's/$$>/$$^/' map/Makefile > map/GNUmakefile
