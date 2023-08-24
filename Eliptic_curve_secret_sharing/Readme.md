# Eliptic curve

I set the elliptic curve type to NID_secp256k1 in the ecc.c  
The generator is likewise a point on that elliptic curve.

m is degree of polynomial when creating polynomial for secret sharing
Let m = 3 and generator of elliptic curve group = G (G is point on curve) and secret number = S

$$ a_3x^3 + a_2x^2 + a_1x + S + 3G$$

First of all
$$mG (m = 3)$$
Second  
S convert to (S, 1)  
Third  
secp256k1 is using 256-bit prime number.  
So $$S+3G = (256bits, 256bits)$$ convert to $$256bits||256bits$$ by bitwise shifting.  
Then we create 512bits number.