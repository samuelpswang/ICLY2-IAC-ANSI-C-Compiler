ID=`tput sgr0``tput bold``tput setab 7``tput setaf 0`
COMD=`tput sgr0``tput bold`
SUCC=`tput sgr0``tput bold``tput setaf 2`
FAIL=`tput sgr0``tput bold``tput setaf 1`
RSET=`tput sgr0`

echo "${ID}[debug_compiler.sh]${COMD} make clean${RSET}"
make clean

echo "${ID}[debug_compiler.sh]${COMD} make bin/compiler${RSET}"
make bin/compiler

echo "${ID}[debug_compiler.sh]${COMD} bin/compiler -S tests/_debug/target.c -o bin/output/_debug/target.out.txt${RSET}"
bin/compiler -S tests/_debug/target.c -o bin/output/_debug/target.out.txt
