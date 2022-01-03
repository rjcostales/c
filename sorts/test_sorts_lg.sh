echo "Sort Test"

./generate 40000 $RANDOM > data.txt

./quicksort < data.txt > /dev/null
echo ""

./merge < data.txt > /dev/null
echo ""

./shell < data.txt > /dev/null
echo ""

./insertion < data.txt > /dev/null
echo ""

./selection < data.txt > /dev/null
echo ""

./bubble < data.txt > /dev/null
echo ""

rm data.txt
