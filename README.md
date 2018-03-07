# misc
Examples, Useful Helper Functions I Rewrite Over and Over, Small Code Snippets That Might Be Useful, Et Cetera

A major design goal of these is to have as few dependencies on libraries
other than libc as possible to enable them to be rapidly integrated
into many embedded systems. 

## xplatform_endian
Convert/serialize fixed width integer types to big-endian byte
buffers. These are optimized for cross platform compatibility not for
speed. The work is done entirely algebraically so it should work even on
a crazy hypothetical middle endian machine that doesn't use
twos-compliment encoding. Of course it can convert them back again as
well. 

## serialize_ieee754
This converts IEEE754 floats/doubles to integers preserving the
maximum precision (the secret ingredient is
[frexp](https://linux.die.net/man/3/frexp) ) and then can serialize
them further to big-endian byte buffers using the xplatform_endian
functions above. The return trip to IEEE754 floats/doubles is also
supported.

## void_ptr_list
C implementation of a linked list holding arbitrary data. The grownup
version of the kind of thing you wrote for yourself in your
Programming II class once you realized you would need it for every
project. It uses common conceits like iterators and contexts for
loading up function pointers for comparing / allocating data the
"class" knows nothing about.

## nostrtok
The default C tokenizer strtok() is so awful that even its own man
page recommended it be avoided for a time. This one uses a bit more
memory, but it doesn't corrupt the input string and doesn't keep any
state in hidden internal static variables making it much easier to
make threadsafe.


