echo "Insertion Sort Test"

./generate 10000 > data.txt
./insertion < data.txt > /dev/null
echo

./generate 20000 > data.txt
./insertion < data.txt > /dev/null
echo

./generate 30000 > data.txt
./insertion < data.txt > /dev/null
echo

./generate 40000 > data.txt
./insertion < data.txt > /dev/null
echo

./generate 50000 > data.txt
./insertion < data.txt > /dev/null
echo

./generate 60000 > data.txt
./insertion < data.txt > /dev/null
echo

rm data.txt
