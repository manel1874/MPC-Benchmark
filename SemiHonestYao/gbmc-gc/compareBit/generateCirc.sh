#! /bin/bash


echo "Let us start" 

../CBMC-GC-2/bin/cbmc-gc --minimization-time-limit 30 --function mpc_main main.c
echo "cbmc-gc circuit generated"

../CBMC-GC-2/bin/circuit-utils --as-bristol mainBristol.txt
echo "bristol circuit generated"

cp mainBristol.txt ../libscapi/tools/circuits/scapiBristolConverter
echo "moved bristol circuit to convert to scapi"

cd ../libscapi/tools/circuits/scapiBristolConverter
./scapiBristolConverter bristol_to_scapi mainBristol.txt mainScapi.txt 2 true
echo "scapi circuit generated"

mv mainScapi.txt ~/MPC-Benchmark/SemiHonestYao
rm mainBristol.txt

 
