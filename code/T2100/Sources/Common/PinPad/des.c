#define ENCRYPT   0
#define DECRYPT   1

typedef unsigned char uchar;

extern void Encrypt(uchar *Key,uchar *Data,uchar *Result);
extern void Decrypt(uchar *Key,uchar *Data,uchar *Result);



static void dea_perm(char *istr, int *perm, char *ostr, int len);
static dea_lsenc(char *ikey, char *okey);
static dea_lsdec(char *ikey, char * okey);
static dea_sub(char *in, char *out);


static void pk(char *p1, char *p2, int len)
{
	int k,x,i;
	for(i = 0; i < len; i+= 2) {
		x = p1[i] % 16;
		if(p1[i] > '9') {
			x += 9;
		}
		k = x << 4;
		x = p1[i + 1] % 16;
		if(p1[i + 1] > '9') {
			x += 9;
		}
		*p2++ = k | x;
	}
}

static void upk(char *p1, char *p2, int len)
{
	int i,x;
	for(i = 0; i < len; i ++) {
		x = (p1[i] >> 4) & 0x0f;
		if(x > 9) {
			*p2++ = 'A' + x - 10;
		}
		else {
			*p2++ = '0' + x;
		}
		x = p1[i] & 0x0f;
		if(x > 9) {
			*p2++ = 'A' + x - 10;
		}
		else {
			*p2++ = '0' + x;
		}
	}
}

/* ******************** Data Encryption Algorithm ***************** */
  
/* document for DEA is :
   Data Encryption Standard
   Federal Information Processing Standards Publication 46,
   National Bureau of Standards, January 15, 1977. */

/* IP : Initial Permutation Table */
static int ip[64] = {
    58, 50, 42, 34, 26, 18, 10,  2,
    60, 52, 44, 36, 28, 20, 12,  4,
    62, 54, 46, 38, 30, 22, 14,  6,
    64, 56, 48, 40, 32, 24, 16,  8,
    57, 49, 41, 33, 25, 17,  9,  1,
    59, 51, 43, 35, 27, 19, 11,  3,
    61, 53, 45, 37, 29, 21, 13,  5,
    63, 55, 47, 39, 31, 23, 15,  7 };

/* E : E-bit selection table */
static int e[48] = {
    32,  1,  2,  3,  4,  5,
     4,  5,  6,  7,  8,  9,
     8,  9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32,  1 };

/* PC-1 : Permuted Choice Table - 1 */
static int cd0[56] = {
     57, 49, 41, 33, 25, 17,  9,
      1, 58, 50, 42, 34, 26, 18,
     10,  2, 59, 51, 43, 35, 27,
     19, 11,  3, 60, 52, 44, 36,
     
     63, 55, 47, 39, 31, 23, 15,
      7, 62, 54, 46, 38, 30, 22,
     14,  6, 61, 53, 45, 37, 29,
     21, 13,  5, 28, 20, 12,  4 };

/* LS : Left Shift Table */
static int ls[16] = {
     1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };

/* PC-2 : Permuted choice table - 2 */
static int pc2[48] = {
     14, 17, 11, 24,  1,  5,
      3, 28, 15,  6, 21, 10,
     23, 19, 12,  4, 26,  8,
     16,  7, 27, 20, 13,  2,
     41, 52, 31, 37, 47, 55,
     30, 40, 51, 45, 33, 48,
     44, 49, 39, 56, 34, 53,
     46, 42, 50, 36, 29, 32 };

/* S : Substitution Boxes */
static int s[8][64] = {
    14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
     0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
     4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
    15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
    
    15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
     3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
     0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
    13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
    
    10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
    13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
    13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
     1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
     
     7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
    13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
    10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
     3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
     
     2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
    14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
     4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
    11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
    
    12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
    10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
     9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
     4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
     
     4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
    13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
     1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
     6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
     
    13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
     1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
     7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
     2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
     };

/* P : Permutation Table */
static int p[32] = {
     16,  7, 20, 21,
     29, 12, 28, 17,
      1, 15, 23, 26,
      5, 18, 31, 10,
      2,  8, 24, 14,
     32, 27,  3,  9,
     19, 13, 30,  6,
     22, 11,  4, 25 };

/* IP-1 : Inverse Initial Permutation Table */
static int ip1[64] = {
    40,  8, 48, 16, 56, 24, 64, 32,
    39,  7, 47, 15, 55, 23, 63, 31,
    38,  6, 46, 14, 54, 22, 62, 30,
    37,  5, 45, 13, 53, 21, 61, 29,
    36,  4, 44, 12, 52, 20, 60, 28,
    35,  3, 43, 11, 51, 19, 59, 27,
    34,  2, 42, 10, 50, 18, 58, 26,
    33,  1, 41,  9, 49, 17, 57, 25 };

/*  DEA logical flow for one iteration :
    Input 64 bit data block (8 bytes)         Key (64 bits)
                |                                   |
       Initial Permutation                    pc-1 permutation
        |               |                       |         |
    L0 (32 bits)      R0(32 bits)           C0 28 bits  d0 28 bits
        |               |                        |        |
        |      ---------o                      LS left shift table
        |     |         |                        |        |
    ----------->  Expansion (48 bits)      C1 (28 bits)  D1(28 bits)
   |    |     |         |                        |        |
   |    |     |         +--------K1(48bits)--- PC-2 permutation
   |    |     |         |
   |    |     |   s1 s2 ... s8
   |    |     |         |
   |    |     |   Permutation
   |    |     |         |
   |     -----|---------+
   |          |         |
    ------[ L1=R0   R1=L0+f(R0,K1)  ]
              |         |
       Inverse Initial Permutation

general dea program logic :
function inputs are : encrypt/decrypt,input data, key, input data length.
function invernal data :
    k0 k1 each 64 bits in length
    lr0 64 bits in length
    lr1 96 bits in length
    l0 r0 each 32 bits in length
    l1 r1 each 48 bits in length
calculate key from K0 -> C1,D1
calculate L0,R0 from input
counter = 0;
do {
   calculate K1
   calculate R1 as L0 + f(R0,K1)
   } while(counter < 16);
apply inverse permutation to L1 and R1.
*/

static void dea(int type, char *tinp, char *key, int tlen)
{
int i, j, k, counter;
char *k1, *k0, *t1;
char *lr0, *lr1, *l0, *r0, *l1, *r1;
char kbit[128];
char tbit[160];

/* set key pointers */
k0 = &kbit[0];  k1 = &kbit[64];

/* set input text pointers */
lr0 = &tbit[0];  lr1 = &tbit[64];
l0  = &tbit[0];  r0  = &tbit[32];
l1  = &tbit[64]; r1  = &tbit[112];

/* convert packed key into a bit string */
for(i = 0; i < 8; i++) {
    k = i << 3;
    for(j = 0; j < 8; j++)
        k0[k + j] = (key[i] >> (7 - j)) & 1;
    }

/* get c0 and d0 from k0; pc-1 permutation */
dea_perm(k0, cd0, k1, 56);
t1 = k1; k1 = k0; k0 = t1;

/* for every 8 bytes of input data do the following */
while(tlen > 0) {
    for(i = 0; i < 8; i++) {
        k = i << 3;
        for(j = 0; j < 8; j++)
            lr1[k + j] = (tinp[i] >> (7 - j)) & 1;
        }
    /* get l0 and r0 from lr; initial permutation */
    dea_perm(lr1, ip, lr0, 64);
    /* 16 times iteration steps */
    counter = 0;
    do {
       /* e-bit selection table */
       dea_perm(r0, e, l1, 48);
       /* Kn key calculation steps */
       if(type == ENCRYPT) {
           /* left shift for encryption */
           for(i = ls[counter]; i > 0; i--) {
               dea_lsenc(k0,k1);
               /* swap current key value */
               t1 = k1; k1 = k0; k0 = t1;
           }
       }
       /* second permutation choice table */
       dea_perm(k0, pc2, k1, 48);
       for(i = 0; i < 48; i++)
           l1[i] ^= k1[i];
       if(type == DECRYPT) {
           /* left shift for decryption */
           for(i = ls[15 - counter]; i > 0; i--) {
               dea_lsdec(k0,k1);
               /* swap current key value */
               t1 = k1; k1 = k0; k0 = t1;
               }
           }
       /* substitution */
       dea_sub(l1,r1);
       /* permutation */
       dea_perm(r1, p, l1, 32);
       for(i = 0; i < 32; i++) {
           j     = r0[i];
           r0[i] = l0[i] ^ l1[i];
           l0[i] = j;
           }
       counter++;
       } while(counter < 16);
    /* swap left and right halves */
    for(i = 0; i < 32; i++) {
        j     = r0[i];
        r0[i] = l0[i];
        l0[i] = j;
        }
    /* inverse initial pemutation */
    dea_perm(lr0, ip1, lr1, 64);
    /* save current dea value to input area */
    for(i = 0; i < 8; i++) {
       k = i << 3;
       for(j = 0,tinp[i] = 0; j < 8; j++)
           tinp[i] |= lr1[k + j] << (7 - j);
       }
    /* set next input values */
    tinp += 8;
    tlen -= 8;
    }
}

static void dea_perm(char *istr, int *perm, char *ostr, int len)
{
int i;

for(i = len - 1; i >= 0; i--)
    ostr[i] = istr[perm[i] - 1];
}

static dea_lsenc(char *ikey, char *okey)
{
int i;
for(i = 0; i < 55; i++)
    okey[i] = ikey[i + 1];
okey[27] = ikey[0];
okey[55] = ikey[28];
}

static dea_lsdec(char *ikey, char * okey)
{
int i;
for(i = 1; i < 56; i++)
    okey[i] = ikey[i - 1];
okey[0]  = ikey[27];
okey[28] = ikey[55];
}

static dea_sub(char *in, char *out)
{
int i, j, k, col;

for(i = 0; i < 8; i++) {
    /* get 6 bits input value for s table in col */
    k = i * 6;
    col = in[k] << 5;
    col |= in[k + 5] << 4;
    for(j = 1; j < 5; j++)
        col |= in[k + j] << (4 - j);
    /* convert subtitution table value to bits */
    k = i << 2;
    for(j = 0; j < 4; j++)
        out[k + j] = (s[i][col] >> (3 - j)) & 1;
    }
}





extern void Encrypt(uchar *Key,uchar *Data,uchar *Result)
{
	char encbuf[17];
	char pkey[17];
	char pan1[17],pdata[17],perkey[17];
	pk((char *)Key, pkey,16);
	strncpy((char *) pan1, (char *) Data,16);
	pan1[16]='\0';
	pk((char *)pan1,pdata,16);
	dea(ENCRYPT,pdata,pkey,8);
	upk(pdata,perkey,8);
	memcpy(Result,perkey,16);
}

extern void Decrypt(uchar *Key,uchar *Data,uchar *Result)
{
	char encbuf[17];
	char pkey[17];
	char pan1[17],pdata[17],perkey[17];
	pk((char *)Key,pkey,16);
	strncpy((char *)pan1,(char *)Data,16);
	pan1[16]='\0';
	pk((char *)pan1,pdata,16);
	dea(DECRYPT,pdata,pkey,8);
	upk(pdata,perkey,8);
	memcpy(Result,perkey,16);
}