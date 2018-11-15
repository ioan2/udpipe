
CPPFLAGS=-std=c++11 -I../src_lib_only -g

UDPIPESRC=../../src


all: UDPipeTextprocessor

UDPipeTextprocessor.o: UDPipeTextprocessor.cc UDPipeTextprocessor.h
	g++ ${CPPFLAGS} -c $<

UDPipeTextprocessor: UDPipeTextprocessor.o
	g++ ${CPPFLAGS} -o $@ $< ${UDPIPESRC}/libudpipe.a


UDDATADIR=/data2/udpipe/ud-2.0
UDVERSION=${UDDATADIR}/old/questions-20180625/EAQ_UD_XPOS_NOISE-all-100-edims50-swap-tag1M-xpos
TAGMODELS=--tag ${UDVERSION}/udpipe.model --lex ${UDVERSION}-lex/lex2.model


test: UDPipeTextprocessor
	@echo "*** Tests avec file"
	./UDPipeTextprocessor              ${UDDATADIR}/fr-ud-train.tokenizer.model test.txt
	./UDPipeTextprocessor ${TAGMODELS} ${UDDATADIR}/fr-ud-train.tokenizer.model test.txt

	@echo "*** Tests avec lecture de cin"
	echo "la petite souris mange." | ./UDPipeTextprocessor              ${UDDATADIR}/fr-ud-train.tokenizer.model
	echo "la petite souris mange." | ./UDPipeTextprocessor ${TAGMODELS} ${UDDATADIR}/fr-ud-train.tokenizer.model

testd: UDPipeTextprocessor
	gdb --args ./UDPipeTextprocessor              ${UDDATADIR}/fr-ud-train.tokenizer.model
#	gdb --args ./UDPipeTextprocessor ${TAGMODELS} ${UDDATADIR}/fr-ud-train.tokenizer.model

testv: UDPipeTextprocessor
	echo "la petite souris mange." | \
		valgrind --log-file=val1.log --track-origins=yes -v --num-callers=25 \
		--tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes \
		./UDPipeTextprocessor ${UDDATADIR}/fr-ud-train.tokenizer.model
	valgrind --log-file=val2.log --track-origins=yes -v --num-callers=25 \
	 	--tool=memcheck --leak-check=full --leak-resolution=high --show-reachable=yes \
		./UDPipeTextprocessor ${TAGMODELS} ${UDDATADIR}/fr-ud-train.tokenizer.model test.txt



