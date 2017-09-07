# CLib
CLib attempts to add data structures that make the tedious, common tasks in C less tedious.

The documentation for these types can be found in their corresponding .h files.

# String
The String type provides methods for manipulating Strings that are not null-terminated.

_Documentation in [string.h](src/string.h#L63)_

# StringBuilder
StringBuilder provides a way to efficiently build Strings of unknown length by allocating extra space before it is used.

_Documentation in [string.h](src/string.h#L389)_

# Number Types
Provides several number types with unambiguous sizes.

**The number types are:** <br />
_u8_ : Unsigned 8 bit integer <br />
_u16_ : Unsigned 16 bit integer <br />
_u32_ : Unsigned 32 bit integer <br />
_u64_ : Unsigned 64 bit integer <br />
_s8_ : Signed 8 bit integer <br />
_s16_ : Signed 16 bit integer <br />
_s32_ : Signed 32 bit integer <br />
_s64_ : Signed 64 bit integer

_Documentation in [numbers.h](src/numbers.h)_