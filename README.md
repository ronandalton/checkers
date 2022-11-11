Checkers/Draughts Game
======================

This is an implementation of the American Checkers or English Draughts game.
You can play against the computer, play with another person on the same computer, or watch the computer play itself.

Building
--------

Issue the following commands to download, build and run the application:

    git clone --recurse-submodules https://github.com/ronandalton/checkers.git
    cd checkers
    mkdir build && cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make -j$(nproc)
    ./bin/checkers_run
