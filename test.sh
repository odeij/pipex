#!/bin/bash

# Pipex Test Script
# This script tests various functionality of the pipex program

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Test counter
TESTS_PASSED=0
TESTS_FAILED=0

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}     Pipex Test Suite${NC}"
echo -e "${BLUE}========================================${NC}"
echo ""

# Function to run a test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected="$3"
    
    echo -e "${YELLOW}Testing: ${test_name}${NC}"
    eval "$command" > /dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ FAILED${NC}"
        ((TESTS_FAILED++))
    fi
    echo ""
}

# Check if pipex exists
if [ ! -f "./pipex" ]; then
    echo -e "${RED}Error: pipex executable not found. Run 'make' first.${NC}"
    exit 1
fi

# Create test files
echo "Creating test files..."
echo "hello world" > test_input.txt
echo "test line 1" >> test_input.txt
echo "test line 2" >> test_input.txt
echo ""

# Test 1: Basic functionality
echo -e "${BLUE}Test 1: Basic pipe functionality${NC}"
./pipex test_input.txt "cat" "wc -l" test_output1.txt
if [ -f test_output1.txt ]; then
    result=$(cat test_output1.txt | tr -d ' ')
    if [ "$result" = "3" ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ FAILED (Expected 3, got $result)${NC}"
        ((TESTS_FAILED++))
    fi
else
    echo -e "${RED}✗ FAILED (Output file not created)${NC}"
    ((TESTS_FAILED++))
fi
echo ""

# Test 2: Grep functionality
echo -e "${BLUE}Test 2: Grep filter${NC}"
./pipex test_input.txt "grep test" "wc -l" test_output2.txt
if [ -f test_output2.txt ]; then
    result=$(cat test_output2.txt | tr -d ' ')
    if [ "$result" = "2" ]; then
        echo -e "${GREEN}✓ PASSED${NC}"
        ((TESTS_PASSED++))
    else
        echo -e "${RED}✗ FAILED${NC}"
        ((TESTS_FAILED++))
    fi
else
    echo -e "${RED}✗ FAILED${NC}"
    ((TESTS_FAILED++))
fi
echo ""

# Test 3: Error handling - file not found
echo -e "${BLUE}Test 3: Error handling (file not found)${NC}"
./pipex nonexistent.txt "cat" "wc" test_output3.txt 2>&1 | grep -q "No such file"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ PASSED${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ FAILED${NC}"
    ((TESTS_FAILED++))
fi
echo ""

# Test 4: Error handling - wrong number of arguments
echo -e "${BLUE}Test 4: Error handling (wrong arguments)${NC}"
./pipex file1 cmd1 2>&1 | grep -q "USAGE"
if [ $? -eq 0 ]; then
    echo -e "${GREEN}✓ PASSED${NC}"
    ((TESTS_PASSED++))
else
    echo -e "${RED}✗ FAILED${NC}"
    ((TESTS_FAILED++))
fi
echo ""

# Test 5: Bonus - Multiple pipes (if bonus exists)
if [ -f "./pipex_bonus" ]; then
    echo -e "${BLUE}Test 5: Bonus - Multiple pipes${NC}"
    ./pipex_bonus test_input.txt "cat" "grep test" "wc -l" test_output5.txt
    if [ -f test_output5.txt ]; then
        result=$(cat test_output5.txt | tr -d ' ')
        if [ "$result" = "2" ]; then
            echo -e "${GREEN}✓ PASSED${NC}"
            ((TESTS_PASSED++))
        else
            echo -e "${RED}✗ FAILED${NC}"
            ((TESTS_FAILED++))
        fi
    else
        echo -e "${RED}✗ FAILED${NC}"
        ((TESTS_FAILED++))
    fi
    echo ""
fi

# Summary
echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}     Test Summary${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "${GREEN}Tests Passed: ${TESTS_PASSED}${NC}"
echo -e "${RED}Tests Failed: ${TESTS_FAILED}${NC}"
echo ""

# Cleanup
echo "Cleaning up test files..."
rm -f test_input.txt test_output*.txt

if [ $TESTS_FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed! ✓${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi

