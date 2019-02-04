# CLib
CLib attempts to make tedious, common tasks in C less tedious.

The documentation for the features of CLib can all be found in their corresponding .h files.



# :rocket: Features
## String
The String type provides methods for manipulating Strings that are not null-terminated.

**Example** : _Formats and splitting_
```C
// Create a String from a format
String contrived = str_format("name: %s, version: %i, url: %s", name, version, url);

// Print out each key value pair in contrived
String keyValue;
String remaining = contrived;
while(!str_isEmpty(remaining)) {
    keyValue = str_splitAtCString(&remaining, ", ");

    // Print out the result
    printf("%s\n", str_toCString(keyValue));
}

// Destroy the String
str_destroy(contrived);
```
```
name: CLib
version: 1
url: https://github.com/Sothatsit/CLib
```

_Documentation in [string.h](src/string.h#L63)_

## StringBuilder
StringBuilder provides a way to efficiently build Strings of unknown length by allocating extra space before it is used.

**Example** : _Comma seperated array_ <br />
```C
// Create a StringBuilder to build a comma seperated array
StringBuilder builder = strbuilder_create(32);

// Loop through each fruit in fruits and add it to builder
for(u64 index = 0; index < numFruits; ++index) {
    String fruit = fruits[index];

    if(index != 0)
        strbuilder_appendC(&builder, ", ");

    strbuilder_append(&builder, fruit);
}

// Print out the built String
printf("Fruits: [%s]\n", str_toCString(builder.string));

// Destroy the StringBuilder
strbuilder_destroy(builder);
```
```
Fruits: [Apple, Banana, Cumquat, Dragon fruit, Eggplant]
```

_Documentation in [string.h](src/string.h#L389)_

## Number Types
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
## Automatically running tests:
The script [`looptests.sh`](looptests.sh) will automatically re-run the tests when ever any source files change and can be used as follows:
```
cd <CLib base directory>
./looptests.sh
```

## Manually running tests:
The script [`test.sh`](test.sh) will compile and run the tests once and can be used as follows:
```
cd <CLib base directory>
./test.sh
```

## Compiling the test binary:
The binary for the test program is outputted to `buildtest/test` and can be built using:
```
cd <CLib base directory>
make test
```


# :book: License
CLib uses the [MIT](https://choosealicense.com/licenses/mit/) license.