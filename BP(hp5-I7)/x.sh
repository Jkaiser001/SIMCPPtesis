
rm -r salidasBP

mkdir salidasBP

echo "NT=1"
mkdir salidasBP/output1
mkdir salidasBP/output1/out
mkdir salidasBP/output1/graf

 ./simulador 5000  64  1 3000000 ../Input/ traza-30-0-8.dat
 
# echo "NT=2"
# mkdir salidasBP/output2
# mkdir salidasBP/output2/out
# mkdir salidasBP/output2/graf

# ./simulador 5000  64  2 30000000 ../Input/ traza-0-0-8.dat >> xejec.dat

# echo "NT=4"
# mkdir salidasBP/output4
# mkdir salidasBP/output4/out
# mkdir salidasBP/output4/graf

# ./simulador 5000  64  4 10000000 ../Input/ traza-0-0-8.dat >> xejec.dat

# echo "NT=8"
# mkdir salidasBP/output8
# mkdir salidasBP/output8/out
# mkdir salidasBP/output8/graf

# ./simulador 5000 128 8 10000000 ../Input/ traza-0-0-8.dat >> xejec.dat

# echo "NT=16"
# mkdir salidasBP/output16
# mkdir salidasBP/output16/out
# mkdir salidasBP/output16/graf

# ./simulador 5000 256 16 10000000 ../Input/ traza-0-0-8.dat >> xejec.dat

# echo "NT=32"
# mkdir salidasBP/output32
# mkdir salidasBP/output32/out
# mkdir salidasBP/output32/graf

# ./simulador 5000 512 32 10000000 ../Input/ traza-0-0-8.dat >> xejec.dat