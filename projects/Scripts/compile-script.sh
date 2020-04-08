PROJECT_DIR=../..
SOURCE_DIR=${PROJECT_DIR}/sources
OUTPUT_DIR=${PROJECT_DIR}/outputs

cd $(dirname "$0")

if [ ! -d ${OUTPUT_DIR} ]; then
  mkdir ${OUTPUT_DIR}
else
  rm -rf ${OUTPUT_DIR}/*
fi

echo "--> compiling cm-core"
mkdir ${OUTPUT_DIR}/cm-core
$1 $3 -c ${SOURCE_DIR}/cm-core/cm_core.cpp       -o ${OUTPUT_DIR}/cm-core/cm_core.o
$1 $3 -c ${SOURCE_DIR}/third/pugixml/pugixml.cpp -o ${OUTPUT_DIR}/cm-core/pugixml.o
$2 rcs ${OUTPUT_DIR}/cm-core/libcm-core.a ${OUTPUT_DIR}/cm-core/*.o

echo "--> compiling cm-console"
mkdir ${OUTPUT_DIR}/cm-console
$1 $3 -c ${SOURCE_DIR}/cm-console/cm_console_main.cpp        -o ${OUTPUT_DIR}/cm-console/cm_console_main.o
$1 ${OUTPUT_DIR}/cm-console/*.o -o ${OUTPUT_DIR}/cm-console/cmgr -L${OUTPUT_DIR}/cm-core/ -lcm-core
