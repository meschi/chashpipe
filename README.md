# chashpipe - pipe on matching hash
`chashpipe` is licensed under the ISC license

## Overview
- Pipes stdin to stdout if and only if the checksum matches the provided hash
- hashpipe (https://github.com/jbenet/hashpipe) reimplemented in C
- Compatible with sha1sum, sha256sum etc

This tool is compatible with the standard unix checksum tools like sha1sum, sha256sum etc.
I included neither compatibility with the multihash format (https://github.com/jbenet/multihash/) nor with the original hashpipe implementation. I decided to do this to keep the code and the binaries slim. Implementing traditional unix-tool compatibility and multihash would introduce too much bloat.

## Usage
Generate sha1 checksum and check with hashpipe
```sh
% echo "foo" | shasum
f1d2d2f924e986ac86fdf7b36c94bcdf32beec15
% echo "foo" | hashpipe -f sha1 f1d2d2f924e986ac86fdf7b36c94bcdf32beec15
foo
% echo "bar" | hashpipe -f sha1 f1d2d2f924e986ac86fdf7b36c94bcdf32beec15
hashpipe: Hashes do not match: e242ed3bffccdf271b7fbaf34ed72d089537b42f
% echo "foo" | hashpipe -f sha1 f1d2
```

Supported hash functions:
-sha1
-sha256 (Default)
-sha512
-ripemd
-whirlpool
-md5 (Don't use this!)

You can specify the hash function with the `-f` flag.

## Building
```sh
make
sudo make install
```

Build dependencies: openssl-dev
