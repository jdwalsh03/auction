#!/bin/bash
LOG=log.txt
echo "Starting installation..."
echo "Starting installation..." &> ./$LOG
mkdir test 2> /dev/null
cd ./graph
echo " Compiling program <graph/graph.out>"
echo "----------------------------------------------------" &>> ../$LOG
echo " Compiling program <graph/graph.out>" &>> ../$LOG
echo "----------------------------------------------------" &>> ../$LOG
make clean &>> ../$LOG
make &>> ../$LOG
make clean &>> ../$LOG
rmdir dep 2> /dev/null
let IT=1
while [ $IT -le 12 ]; do
  case $IT in
    1)
      FNAME=I-0100-0100-001400-09.txt
      ;;
    2)
      FNAME=I-0100-0100-010000-09.txt
      ;;
    3)
      FNAME=I-0100-0100-010000-99.txt
      ;;
    4)
      FNAME=I-0500-0500-035000-19.txt
      ;;
    5)
      FNAME=I-0500-4500-315000-01.txt
      ;;
    6)
      FNAME=I-0500-4500-315000-09.txt
      ;;
    7)
      FNAME=R-0100-0100-005000-99.txt
      ;;
    8)
      FNAME=R-0100-0100-010000-09.txt
      ;;
    9)
      FNAME=R-0100-0100-010000-99.txt
      ;;
    10)
      FNAME=R-0500-0500-035000-19.txt
      ;;
    11)
      FNAME=R-0500-4500-315000-01.txt
      ;;
    12)
      FNAME=R-0500-4500-315000-09.txt
      ;;
  esac
  echo " Building graph <test/$FNAME>"
  echo "----------------------------------------------------" &>> ../$LOG
  echo " Building graph <test/$FNAME>" &>> ../$LOG
  echo "----------------------------------------------------" &>> ../$LOG
  ./graph.out -l ./test/$FNAME -s ../test/$FNAME &>> ../$LOG
  let IT=IT+1
done
cd ../
echo " Compiling program <auction.out>"
echo "----------------------------------------------------" &>> ./$LOG
echo " Compiling program <auction.out>" &>> ./$LOG
echo "----------------------------------------------------" &>> ./$LOG
make clean &>> ./$LOG
make &>> ./$LOG
make clean &>> ./$LOG
rmdir dep 2> /dev/null
echo "Installation complete"
echo "----------------------------------------------------" &>> ./$LOG
echo "Installation complete" &>> ./$LOG
exit 0
