echo "[test.sh] make clean"
make clean

echo "[test.sh] make bin/prettyprint"
make bin/prettyprint

echo "[test.sh] bin/prettyprint -S tests/input/simple.c -o tests/output/simple.out.txt"
bin/prettyprint -S tests/input/simple.c -o tests/output/simple.out.txt
