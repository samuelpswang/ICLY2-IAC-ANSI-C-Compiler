ID=`tput sgr0``tput bold``tput setab 7``tput setaf 0`
COMD=`tput sgr0``tput bold`
SUCC=`tput sgr0``tput bold``tput setaf 2`
FAIL=`tput sgr0``tput bold``tput setaf 1`
RSET=`tput sgr0`

echo "${ID}[debug_prettyprint.sh]${COMD} make clean${RSET}"
make clean

echo "${ID}[debug_prettyprint.sh]${COMD} make bin/prettyprint${RSET}"
make bin/prettyprint

echo "${ID}[debug_prettyprint.sh]${COMD} bin/prettyprint -S tests/_debug/target.c -o bin/_debug/target.out.txt${RSET}"
bin/prettyprint -S tests/_debug/target.c -o bin/_debug/target.out.txt
