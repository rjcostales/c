echo "Merge Sort Test"

./generate 10000 > data.txt
./insertion < data.txt > /dev/null
echo "10K"

./generate 20000 > data.txt
./insertion < data.txt > /dev/null
echo "20K"

./generate 30000 > data.txt
./insertion < data.txt > /dev/null
echo "30K"

./generate 40000 > data.txt
./insertion < data.txt > /dev/null
echo "40K"

./generate 50000 > data.txt
./insertion < data.txt > /dev/null
echo "50K"

./generate 60000 > data.txt
./insertion < data.txt > /dev/null
echo "60K"

rm data.txt
