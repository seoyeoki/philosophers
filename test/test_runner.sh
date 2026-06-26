#!/bin/bash

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0;35m' # No Color
RESET='\033[0m'

echo -e "${BLUE}==================================================${RESET}"
echo -e "${BLUE}        Philosophers Behavior Tests               ${RESET}"
echo -e "${BLUE}==================================================${RESET}"

# Step 1: Compile all executables
echo -e "${YELLOW}[1/6] Compiling original philo (with ft_usleep & guard)...${RESET}"
make -C ../philo re > /dev/null
if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Failed to compile original philo.${RESET}"
    exit 1
fi
echo -e "${GREEN}Original philo compiled successfully.${RESET}"

echo -e "${YELLOW}[2/6] Compiling test binaries (usleep, no_guard & busy_wait)...${RESET}"
make re > /dev/null
if [ $? -ne 0 ]; then
    echo -e "${RED}Error: Failed to compile test binaries.${RESET}"
    exit 1
fi
echo -e "${GREEN}Test binaries compiled successfully.${RESET}"

# Parameters
ARGS_SURVIVE="4 402 200 100"
ARGS_DIE="4 310 200 100"
ARGS_SINGLE="1 800 200 200"
TEST_DURATION=5 # seconds

# -----------------------------------------------------------------------------
# Case 1: Original (ft_usleep + guarded print)
# -----------------------------------------------------------------------------
echo -e "\n${BLUE}==================================================${RESET}"
echo -e "${YELLOW}[3/6] CASE 1: Original (ft_usleep + Print Guard)${RESET}"
echo -e "Parameters: ${ARGS_SURVIVE} (Should survive)"

../philo/philo $ARGS_SURVIVE > original_run.log &
ORIGINAL_PID=$!

sleep $TEST_DURATION

if kill -0 $ORIGINAL_PID 2>/dev/null; then
    if grep -q "died" original_run.log; then
        echo -e "${RED}❌ FAILED: A philosopher died!${RESET}"
        cat original_run.log | tail -n 5
    else
        echo -e "${GREEN}✅ PASSED: All philosophers survived for ${TEST_DURATION}s!${RESET}"
    fi
    kill $ORIGINAL_PID 2>/dev/null
    wait $ORIGINAL_PID 2>/dev/null
else
    echo -e "${RED}❌ FAILED: Exited early.${RESET}"
    cat original_run.log | tail -n 5
fi

# -----------------------------------------------------------------------------
# Case 2: Standard usleep (usleep + guarded print)
# -----------------------------------------------------------------------------
echo -e "\n${BLUE}==================================================${RESET}"
echo -e "${YELLOW}[4/6] CASE 2: Sleep Latency (standard usleep + Print Guard)${RESET}"
echo -e "Parameters: ${ARGS_SURVIVE} (Should die early due to OS scheduler delay)"

./philo_usleep_test $ARGS_SURVIVE > test_usleep_run.log &
TEST_USLEEP_PID=$!

SURVIVED=0
for ((i=1; i<=TEST_DURATION; i++)); do
    sleep 1
    if ! kill -0 $TEST_USLEEP_PID 2>/dev/null; then
        SURVIVED=$i
        break
    fi
done

if grep -q "died" test_usleep_run.log; then
    echo -e "${GREEN}✅ PASSED (Experiment): Died as expected! (Survived ~${SURVIVED}s)${RESET}"
    echo -e "Last few log lines:"
    echo -e "${RED}"
    cat test_usleep_run.log | tail -n 5
    echo -e "${RESET}"
else
    if kill -0 $TEST_USLEEP_PID 2>/dev/null; then
        echo -e "${RED}❌ FAILED: Survived (no scheduler latency observed).${RESET}"
        kill $TEST_USLEEP_PID 2>/dev/null
        wait $TEST_USLEEP_PID 2>/dev/null
    else
        echo -e "${RED}❌ FAILED: Exited early without 'died' log.${RESET}"
        cat test_usleep_run.log | tail -n 5
    fi
fi

# -----------------------------------------------------------------------------
# Case 3: Unguarded print (ft_usleep + no print guard)
# -----------------------------------------------------------------------------
echo -e "\n${BLUE}==================================================${RESET}"
echo -e "${YELLOW}[5/6] CASE 3: Post-Death Print (ft_usleep + No Print Guard)${RESET}"
echo -e "Parameters: ${ARGS_DIE} (Should die at 310ms, but print extra logs afterwards)"

./philo_no_guard_test $ARGS_DIE > test_noguard_run.log &
TEST_NOGUARD_PID=$!

# Wait for it to finish (it will die in ~310ms, so 2 seconds is plenty)
sleep 2

# Check if there are logs printed after the "died" line
died_line=$(grep -n "died" test_noguard_run.log | cut -d: -f1 | head -n 1)

if [ -z "$died_line" ]; then
    echo -e "${RED}❌ FAILED: No philosopher died (unexpected).${RESET}"
    cat test_noguard_run.log | tail -n 10
else
    total_lines=$(wc -l < test_noguard_run.log)
    late_lines=$((total_lines - died_line))
    
    echo -e "${GREEN}✅ PASSED (Experiment): Philosopher died at line $died_line.${RESET}"
    
    if [ $late_lines -gt 0 ]; then
        echo -e "${RED}⚠️  Observation: Detected $late_lines late log line(s) printed AFTER the death log!${RESET}"
        echo -e "This will cause an immediate FAIL in 42 Seoul evaluation."
        echo -e "\nLogs around the death event (Line $died_line):"
        echo -e "${BLUE}"
        
        # Show lines around the death
        start_print=$((died_line - 2))
        if [ $start_print -lt 1 ]; then start_print=1; fi
        tail -n +$start_print test_noguard_run.log | head -n 6
        
        echo -e "${RESET}"
    else
        echo -e "${YELLOW}ℹ️  Observation: No late log lines printed after death in this run (CPU core scheduling lucky timing).${RESET}"
        echo -e "Logs around death:"
        cat test_noguard_run.log | tail -n 5
    fi
fi

# -----------------------------------------------------------------------------
# Case 4: CPU Usage - Busy Wait vs Blocked Sleep (Single Philo)
# -----------------------------------------------------------------------------
echo -e "\n${BLUE}==================================================${RESET}"
echo -e "${YELLOW}[6/6] CASE 4: CPU Resource Usage (Blocked Sleep vs Busy Wait)${RESET}"
echo -e "Parameters: ${ARGS_SINGLE} (Single philosopher, should die at 800ms)"

# 4.1 Run Original version (Sleep)
../philo/philo $ARGS_SINGLE > test_sleep_single.log &
SLEEP_PID=$!
sleep 0.4
# Measure CPU of sleep version
SLEEP_CPU=$(ps -p $SLEEP_PID -o %cpu 2>/dev/null | tail -n 1 | xargs)
if [ -z "$SLEEP_CPU" ]; then SLEEP_CPU="0.0"; fi
# Wait for it to die (800ms)
sleep 0.6
kill $SLEEP_PID 2>/dev/null
wait $SLEEP_PID 2>/dev/null

# 4.2 Run Busy Wait version
./philo_busy_wait_test $ARGS_SINGLE > test_busy_single.log &
BUSY_PID=$!
sleep 0.4
# Measure CPU of busy version
BUSY_CPU=$(ps -p $BUSY_PID -o %cpu 2>/dev/null | tail -n 1 | xargs)
if [ -z "$BUSY_CPU" ]; then BUSY_CPU="0.0"; fi
# Wait for it to die (800ms)
sleep 0.6
kill $BUSY_PID 2>/dev/null
wait $BUSY_PID 2>/dev/null

echo -e "${GREEN}✅ PASSED (Experiment): CPU measurements captured during execution.${RESET}"
echo -e "Original Version CPU Usage (ft_usleep):  ${GREEN}${SLEEP_CPU}%${RESET}"
echo -e "Busy Wait Version CPU Usage (while-loop): ${RED}${BUSY_CPU}%${RESET}"

if (( $(echo "$BUSY_CPU > $SLEEP_CPU" | bc -l 2>/dev/null || [ $(echo "$BUSY_CPU" | cut -d. -f1) -gt $(echo "$SLEEP_CPU" | cut -d. -f1) ]) )); then
    echo -e "${RED}⚠️  Observation: Busy wait version consumed significantly more CPU!${RESET}"
    echo -e "This blocks other threads (like the monitor) from executing on time."
else
    echo -e "${YELLOW}ℹ️  Observation: CPU readings compiled (Mac CPU virtualization might smooth out short PID measurements).${RESET}"
fi

# Clean up logs
rm -f original_run.log test_usleep_run.log test_noguard_run.log test_sleep_single.log test_busy_single.log

echo -e "${BLUE}==================================================${RESET}"
echo -e "${GREEN}Testing complete!${RESET}"
echo -e "${BLUE}==================================================${RESET}"
