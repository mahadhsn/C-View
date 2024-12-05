declare -i tc=0
declare -i fails=0

############################################
# Run a single test. Runs a given command 
# to check the return value, stdout, and output file.
#
# GLOBALS: tc, fails
# PARAMS: $1 = command
#         $2 = input file name (if applicable)
#         $3 = expected return value
#         $4 = output file for program text output (if applicable)
#         $5 = expected text output file
#         $6 = output BMP file (if applicable)
#         $7 = expected BMP file
#
# OUTPUTS: A pass/fail report showing the
# difference between expected and actual
# output on a failure (diff command).
############################################
test() {
    tc=tc+1

    local COMMAND=$1
    local INPUT_FILE=$2
    local EX_RET=$3
    local OUTPUT_FILE_TXT=$4
    local EX_OUT_TXT=$5
    local OUTPUT_FILE_BMP=$6
    local EX_OUT_BMP=$7


    if [ -n $OUTPUT_FILE_TXT ]; then
        OUTPUT_FILE_TXT=output.txt
    fi
    # Run command and capture output
    $COMMAND > $OUTPUT_FILE_TXT
    local RET=$?

    # Check return value
    if [ $RET != $EX_RET ]; then
        echo "TC $tc Failed"
        echo "Expected return value: $EX_RET"
        echo "Actual return value: $RET"
        fails=$((fails+1))
        return
    fi

    # Compare text output if applicable
    if [ -n "$EX_OUT_TXT" ]; then
        local DIFF=$(diff "$OUTPUT_FILE_TXT" "$EX_OUT_TXT")
        if [ "$DIFF" != "" ]; then
            echo "TC $tc Failed: Text Output Mismatch"
            echo "Expected output: $EX_OUT_TXT"
            echo "Actual output: $OUTPUT_FILE_TXT"
            echo "$DIFF"
            fails=$((fails+1))
            return
        fi
    fi

    # Compare BMP output if applicable
    if [ -n "$EX_OUT_BMP" ]; then
        cmp -s "$OUTPUT_FILE_BMP" "$EX_OUT_BMP"
        if [ $? != 0 ]; then
            echo "TC $tc Failed: BMP Output Mismatch"
            echo "Expected file: $EX_OUT_BMP"
            echo "Actual file: $OUTPUT_FILE_BMP"
            fails=$((fails+1))
            return
        fi
    fi

    echo "TC $tc Passed"
}

# Run tests
test "./c-view-test --help" "" 0 "./test/help.txt" "./test/help.txt" "" ""
test "./c-view-test -ed ./test/testimages/file_not_found.bmp ./test/testimages/file_out.bmp" "" 1 "" "./test/usage.txt" "" ""
test "./c-view-test -rot ./test/testimages/snail.bmp ./test/testimages/rotate_out.bmp" "./test/testimages/rotate.bmp" 0 "" "" "./test/testimages/rotate_out.bmp" "./test/testimages/snail_rotated.bmp"
test "./c-view-test -gs ./test/testimages/snail.bmp ./test/testimages/grayscale_out.bmp" "./test/testimages/grayscale.bmp" 0 "" "" "./test/testimages/grayscale_out.bmp" "./test/testimages/snail_grayscale.bmp"
test "./c-view-test -rs ./test/testimages/snail.bmp ./test/testimages/reflect_out.bmp" "./test/testimages/reflect.bmp" 0 "" "" "./test/testimages/reflect_out.bmp" "./test/testimages/snail_reflect.bmp"
test "./c-view-test -bl ./test/testimages/snail.bmp ./test/testimages/blur_out.bmp" "./test/testimages/blur.bmp" 0 "" "" "./test/testimages/blur_out.bmp" "./test/testimages/snail_blur.bmp"
test "./c-view-test -ed ./test/testimages/snail.bmp ./test/testimages/edge_out.bmp" "./test/testimages/edge.bmp" 0 "" "" "./test/testimages/edge_out.bmp" "./test/testimages/snail_edge.bmp"
test "./c-view-test -ed ./test/testimages/snail.bmp ./test/testimages/edge_out.bmp" "./test/testimages/edge.bmp" 0 "" "" "./test/testimages/edge_out.bmp" "./test/testimages/snail_edge.bmp"

# Clean up
rm output.txt
rm testimages/*_out.bmp

exit $fails