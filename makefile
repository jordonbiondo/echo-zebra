## #################################################################
## SUPER COMPLICATED BUILD SYSTEM BELOW: BEWARE
## #################################################################

buildDir = build
srcDir = src
exec = $(buildDir)/multTServer	
writerFile = $(srcDir)/zebra-writer.c
readerFile = $(srcDir)/zebra-reader.c

writer: $(writerFile) $(buildDir)
	gcc -Wall $(writerFile) -o $(buildDir)/zebra-writer


reader: $(readerFile) $(buildDir)
	gcc -Wall $(readerFile) -o $(buildDir)/zebra-reader


$(buildDir):
	mkdir $(buildDir)