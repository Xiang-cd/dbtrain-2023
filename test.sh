#!/bin/bash
RED='\033[1;31m'
GREEN='\033[1;32m'
NC='\033[0m'

cd ../dbtrain-lab
LAB=3
if [ -e LAB.txt ]; then
    LAB=$(cat LAB.txt)
fi
mkdir -p build && cd build
cmake .. || { echo -e "${RED}Cmake Error${NC}"; exit 1; }
make -j16 -l${MAX_LOAD_AVERAGE:-40} || { echo -e "${RED}Compile Error${NC}"; exit 1; }

cd ../../dbtrain-lab-test && \
python3 check.py -l ${LAB} -o || FAIL=1

if [ ${SEND_RESULT:-0} -eq 1 -a -e report.json ]; then
    echo -e "${GREEN}Start to send result${NC}"
    curl -s -F "project=${CI_PROJECT_NAME}" -F "pipeline=${CI_PIPELINE_ID}"\
    -F "job=${CI_JOB_ID}" -F "lab=${LAB}" -F "file=@report.json"\
    172.6.31.14:9876/collect/ || { echo -e "${RED}Failed to send result, please contact TA${NC}"; exit 1; }
    echo -e "${GREEN}Result has been sent${NC}"
else
    echo "No report.json"
fi

if [ ${FAIL:-0} -eq 0 ]; then
    echo -e "${GREEN}Passed lab${LAB} test${NC}";
else
    echo -e "${RED}Failed${NC}";
    exit 1;
fi
