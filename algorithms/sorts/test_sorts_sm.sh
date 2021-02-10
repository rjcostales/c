echo "Sort Test"

./generate 10000 $RANDOM > data.txt

./bubble < data.txt > /dev/null
echo ""

./selection < data.txt > /dev/null
echo ""

./insertion < data.txt > /dev/null
echo ""

./shell < data.txt > /dev/null
echo ""

./merge < data.txt > /dev/null
echo ""

./quicksort < data.txt > /dev/null
echo ""
