# CLib
CLib attempts to make tedious, common tasks in C less tedious.

The .h files are all documented using block comments and should be used as a reference for the functionality of specific functions.



# :rocket: Features
### String
The String type provides methods for manipulating Strings that are not null-terminated.

_Documentation in [string.h](src/string.h#L63)_

### StringBuilder
StringBuilder provides a way to efficiently build Strings of unknown length by allocating extra space before it is used.

_Documentation in [string.h](src/string.h#L389)_

### Number Types
Provides several number types with unambiguous sizes as well as a few maths functions using these types.

**The number types are:** <br />
_u8_  : Unsigned 8 bit integer <br />
_u16_ : Unsigned 16 bit integer <br />
_u32_ : Unsigned 32 bit integer <br />
_u64_ : Unsigned 64 bit integer <br />
_s8_  : Signed 8 bit integer <br />
_s16_ : Signed 16 bit integer <br />
_s32_ : Signed 32 bit integer <br />
_s64_ : Signed 64 bit integer

_Documentation in [numbers.h](src/numbers.h)_



# :microscope: Testing
### Automatically running tests:
The script [`looptests.sh`](looptests.sh) will automatically re-run the tests when ever any source files change and can be used as follows:
```
cd <CLib base directory>
./looptests.sh
```

### Manually running tests:
The script [`test.sh`](test.sh) will compile and run the tests once and can be used as follows:
```
cd <CLib base directory>
./test.sh
```

### Compiling the test binary:
The binary for the test program is outputted to [`build/test`](build/test) and can be built using:
```
make test
```
