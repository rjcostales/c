echo "Sort Test"

./generate 10000 > data.txt

./bubble < data.txt
echo ""

./selection < data.txt
echo ""

./insertion < data.txt
echo ""

./merge < data.txt
echo ""

./quicksort < data.txt
echo ""
