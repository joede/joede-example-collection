# The gtkmm-sample example

To build this example, just use the following steps:

    git clone https://github.com/joede/joede-example-collection.git
    cd joede-example-collection/gtkmm-sample
    mkdir .build && cd .build
    cmake ..
    make

Make sure you have `gtkmm-3.0` at least in version 3.22 installed. The sample
is taken from the book [Programming with gtkmm 3](https://developer.gnome.org/gtkmm-tutorial/stable/).
I have written a small CMake build file to allow an easy to use build.

**Note:** the older version of `gtkmm` provided by Debian 9 makes it necessary to add
an implementation of `make_managed`. Since I am new to GTK and `gtkmm`, I've simply
added the code to the example.

