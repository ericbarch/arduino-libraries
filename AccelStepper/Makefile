# Makefile
#
# Makefile for the Arduino AccelStepper project
#
# Author: Mike McCauley (mikem@open.com.au)
# Copyright (C) 2010 Mike McCauley
# $Id: Makefile,v 1.3 2011/03/21 00:42:15 mikem Exp mikem $

PROJNAME = AccelStepper
# Dont forget to also change the version at the top of AccelStepper.h:
DISTFILE = $(PROJNAME)-1.30.zip

all:	doxygen dist upload

doxygen: 
	doxygen project.cfg

ci:
	(cd ..;ci -l `cat $(PROJNAME)/MANIFEST`)

dist:	
	(cd ..; zip $(PROJNAME)/$(DISTFILE) `cat $(PROJNAME)/MANIFEST`)

upload:
	rsync -avz $(DISTFILE) doc/ server2:/var/www/html/mikem/arduino/$(PROJNAME)

#	scp $(DISTFILE) doc/*.html doc/*.gif doc/*.png doc/*.css doc/*.pdf server2:/var/www/html/mikem/arduino/$(PROJNAME)
