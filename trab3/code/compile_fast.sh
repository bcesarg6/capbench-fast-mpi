#!/bin/bash

echo "FAST original..."
cd FAST
make
echo ""

echo "FAST mpi e mpiP..."
cd ../mpi_FAST
make
make mpip
echo ""
