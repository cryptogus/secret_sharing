# Secret Sharing
https://en.wikipedia.org/wiki/Secret_sharing

## Shamir's secret sharing
https://en.wikipedia.org/wiki/Shamir%27s_secret_sharing

### naive version

OS: Ubuntu 22.04 LTS  

This is a code that conceals and decrypts location information assuming it is secret. 

```bash
$ cd native_secret_sharing
$ cmake -B build -S ./
$ cd build
$ make
```
Test example
```bash
$ ./secret-sharing-test 39.097263 173.12345678
```
If you want to test multiple times with random locations, you can do this  
```bash
$ cd test
$ ./test > test.txt
$ less test.txt
```
