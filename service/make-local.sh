./venv/bin/python3 gen.py gen-texture-headers
./venv/bin/python3 gen.py gen-objects-cpp
cd src && make clean && make -j16; cd -
./venv/bin/python3 gen.py pack-resources
cp src/rick .
