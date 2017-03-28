BashGen
=======

This project makes a code generator for generating the skeleton of bash
scripts with arguments. Arguments that require input are equals (=) separated.
```
./out.sh -l=/usr/local/lib/ -a -x -z="this can contain spaces"
```
Building
--------
```
mkdir build
cd build
cmake ../
make
./generator
```

Reminders
---------
- Double entries are not checked.
