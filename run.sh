#!/bin/sh
# Simple game executable wrapper script example.

# "." is the current working directory, so we put the bundled "./lib" on the
# library path before the previous contents of that environment variable.
export LD_LIBRARY_PATH=./lib:$LD_LIBRARY_PATH

# Run the game itself.
./bin/snow
