
#if !defined(BITS_H)
#define BITS_H
extern void setBitAt( unsigned int * bitmap, int i, int bitValue);
extern int getBitAt( unsigned int bitmap, unsigned int i);
extern int countBits( unsigned int bitmap, unsigned int bitValue);
extern int maxContinuousOnes(unsigned int bitmap, int * position);
extern unsigned int replaceBits(unsigned int val, unsigned int from, unsigned int to, int n);
extern void printBits(unsigned int bitmap);
#endif

