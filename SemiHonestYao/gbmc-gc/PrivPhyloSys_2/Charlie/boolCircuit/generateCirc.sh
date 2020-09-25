#! /bin/bash

echo "Let us start" 

~/CBMC-GC-2/bin/cbmc-gc --unwind 50000 --minimization-time-limit 60 --function mpc_main main.c
echo "cbmc-gc circuit generated"

~/CBMC-GC-2/bin/circuit-utils --as-bristol mainBristol.txt
echo "bristol circuit generated"

cp mainBristol.txt ~/libscapi/tools/circuits/scapiBristolConverter
echo "moved bristol circuit to convert to scapi"

~/libscapi/tools/circuits/scapiBristolConverter/scapiBristolConverter bristol_to_scapi mainBristol.txt mainScapi.txt 2 true
echo "scapi circuit generated"

rm ~/libscapi/tools/circuits/scapiBristolConverter/mainBristol.txt
rm mainBristol.txt

rm output.constants.txt
rm output.gate.txt
rm output.inputs.partyA.txt
rm output.inputs.partyB.txt
rm output.inputs.txt
rm output.mapping.txt
rm output.numberofgates.txt
rm output.spec.txt
rm output.stats.txt

 
