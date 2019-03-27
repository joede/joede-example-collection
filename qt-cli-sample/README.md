# The qt-cli-sample example

To build this example, just use the following steps:

    git clone https://github.com/joede/joede-example-collection.git
    cd joede-example-collection/qt-cli-sample
    mkdir .build && cd .build
    cmake ..
    make

After that, you can play with the example

    $ ./qt-cli-sample -h
    Usage: ./qt-cli-sample [options] source
    Qt sample to parse the command line.

    Options:
      -h, --help               Displays this help.
      -v, --version            Displays version information.
      -d                       Show debugging messages
      -H, --height <num>       Sample option with numeric parameter
      -o, --output <filename>  Sample <filename> to write the result too.

    Arguments:
      source                   Sample input file
    $ ./qt-cli-sample -d -H 1080 -o bar.c foo.c
    info: Height = 1080
    info: OutputFile = "bar.c"
    info: InputFile = "foo.c"
