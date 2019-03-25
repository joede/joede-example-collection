# The getopt example

To build this example, just use the following steps:

    git clone https://github.com/joede/joede-example-collection.git
    cd joede-example-collection/getopt-sample
    mkdir .build && cd .build
    cmake ..
    make

After that, you can play with the example

    ./sample -?
    ./sample -v /dev/video0 -h 720 befehl1 befehl2
