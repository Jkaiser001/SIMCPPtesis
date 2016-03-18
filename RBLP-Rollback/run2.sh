# /scratch/carolina/archivos/indiceOr/

#make todo

nohup ./simulador 10000   64  1 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat >  NT-1.dat &

nohup ./simulador 10000   64  2 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat > NT-2.dat &

nohup ./simulador 10000   64  4 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat > NT-4.dat &

nohup ./simulador 10000   64  8 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat > NT-8.dat &

nohup ./simulador 10000   64 16 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat > NT-16.dat &

nohup ./simulador 10000   64 32 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat > NT-32.dat &

nohup ./simulador 10000   64 64 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat > NT-64.dat &

nohup ./simulador 10000   64 128 /scratch/carolina/archivos/indiceOr/ traza-0-0-8.dat > NT-128.dat &

