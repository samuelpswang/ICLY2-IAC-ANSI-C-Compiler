test_start=$(date +%s.%N)
sleep 20
test_end=$(date +%s.%N)
test_duration=$(echo "$test_end - $test_start" | bc)

if (( $test_duration >= 10))
then
    echo "FALIED (Duration: ${test_duration}s)"
else
    echo "PASSED (Duration: ${test_duration}s)"
fi
