echo 'write test'
time ./write > test.txt
echo ''
echo 'read test'
time ./read < test.txt
echo ''
echo 'copy test'
time cp test.txt copy.txt
echo ''
echo 'move test'
time mv test.txt move.txt
echo ''
echo 'remove test'
time rm copy.txt move.txt
