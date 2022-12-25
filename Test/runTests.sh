#!/bin/bash

FILENAME="TestResults.txt"
rm $FILENAME && touch $FILENAME
./BrokerTest | grep "PASS\|FAIL" >> $FILENAME
./BrokerManagerTest | grep "PASS\|FAIL" >> $FILENAME
./ImporterTest | grep "PASS\|FAIL" >> $FILENAME
./B2mTest | grep "PASS\|FAIL" >> $FILENAME
./BinanceTest | grep "PASS\|FAIL" >> $FILENAME


test_count=$(wc -l $FILENAME)
tests_passed=$(grep -o -i PASS $FILENAME | wc -l)
test_failed=$(grep -o -i FAIL $FILENAME | wc -l)
echo "There are $test_count tests. PASS: $tests_passed. FAIL: $test_failed"

echo "--------------------------------------------------------------------" >> $FILENAME
echo "There are $test_count tests. PASS: $tests_passed. FAIL: $test_failed" >> $FILENAME



