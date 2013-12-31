all: nueoscilib nueosci01 nueosci02

.PHONY: nueosci01 

nueosci01:
	${MAKE} -C nueosci01/src/

.PHONY: nueosci02 

nueosci02:
	${MAKE} -C nueosci02/src/

.PHONY: nueoscilib

nueoscilib:
	${MAKE} -C nueoscilib/src/

clean   :
	${MAKE} clean -C nueoscilib/src/
	${MAKE} clean -C nueosci01/src/
	${MAKE} clean -C nueosci02/src/


