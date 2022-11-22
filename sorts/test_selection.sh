echo "Selection Sort Test"

./generate 10000 $RANDOM > data.txt
./selection < data.txt > /dev/null
echo

./generate 20000 $RANDOM > data.txt
./selection < data.txt > /dev/null
echo

./generate 30000 $RANDOM > data.txt
./selection < data.txt > /dev/null
echo

./generate 40000 $RANDOM > data.txt
./selection < data.txt > /dev/null
echo

./generate 50000 $RANDOM > data.txt
./selection < data.txt > /dev/null
echo

./generate 60000 $RANDOM > data.txt
./selection < data.txt > /dev/null
echo

rm data.txt
