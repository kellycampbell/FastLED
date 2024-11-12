#!/bin/bash

mkdir -p /mapped && cp -r /js/fastled/examples/wasm /mapped/wasm
python /js/run.py compile --debug && rm -rf /mapped && rm -rf /js/.pio/build/wasm/src/wasm.ino.o || echo "failed to delete wasm.ino.o"

mkdir -p /mapped && cp -r /js/fastled/examples/wasm /mapped/wasm
python /js/run.py compile --quick && rm -rf /mapped && rm -rf /js/.pio/build/wasm/src/wasm.ino.o || echo "failed to delete wasm.ino.o"

mkdir -p /mapped && cp -r /js/fastled/examples/wasm /mapped/wasm
python /js/run.py compile --release && rm -rf /mapped && rm -rf /js/.pio/build/wasm/src/wasm.ino.o || echo "failed to delete wasm.ino.o"