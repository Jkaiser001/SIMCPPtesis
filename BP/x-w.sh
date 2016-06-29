
rm -r salidasBP

mkdir salidasBP

echo "NT=1"
mkdir salidasBP/output1
mkdir salidasBP/output1/out
mkdir salidasBP/output1/graf

./simulador 1000  128  1 300000 ../Input/ traza-0-0-8.dat>xejec-R.dat

echo "NT=2"
mkdir salidasBP/output2
mkdir salidasBP/output2/out
mkdir salidasBP/output2/graf

./simulador 1000  256  2 300000 ../Input/ traza-0-0-8.dat >> xejec-R.dat

echo "NT=4"
mkdir salidasBP/output4
mkdir salidasBP/output4/out
mkdir salidasBP/output4/graf

./simulador 1000  512  4 300000 ../Input/ traza-0-0-8.dat >> xejec-R.dat

echo "NT=8"
mkdir salidasBP/output8
mkdir salidasBP/output8/out
mkdir salidasBP/output8/graf

./simulador 1000 1024 8 300000 ../Input/ traza-0-0-8.dat >> xejec-R.dat

echo "NT=16"
mkdir salidasBP/output16
mkdir salidasBP/output16/out
mkdir salidasBP/output16/graf

./simulador 1000 2048 16 3000 ../Input/ traza-0-0-8.dat >> xejec-R.dat

echo "NT=32"
mkdir salidasBP/output32
mkdir salidasBP/output32/out
mkdir salidasBP/output32/graf

./simulador 1000 4096 32 3000 ../Input/ traza-0-0-8.dat >> xejec-R.dat

