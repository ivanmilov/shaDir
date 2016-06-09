# shaDir

Performs sha512 for every file in selected directory.

**Requirements:**
- gcc > 4.9 (c++14)
- boost (program_options system filesystem thread)
- libcrypto

###Compile:
```
mkdir build && cd build
cmake .. && make
```

###Run:
```
./sha
```
will print sha512sum for files in current directory.

###help:
```
./sha -h
```
