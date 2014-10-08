#include <Arduino.h>
#define SWAP(a, b) ((a) ^= (b), (b) ^= (a), (a) ^= (b))
class RC4 
{
public:

	  RC4 () 
	  {
		memset(sbox,0,256);
		memset(key,0,256);
	  }
	  virtual ~RC4 ()
	  {							
		memset(sbox,0,256);
		memset(key,0,256);   
	  }

	  char *Encrypt(char *pszText,const char *pszKey) 
	  {
		i=0, j=0,n = 0;
		ilen = (int)strlen(pszKey);

		for (m = 0;  m < 256; m++)
		{
			*(key + m)= *(pszKey + (m % ilen));
			*(sbox + m) = m;
		}
		for (m=0; m < 256; m++)
		{
			n = (n + *(sbox+m) + *(key + m)) &0xff;
			SWAP(*(sbox + m),*(sbox + n));
		}

		ilen = (int)strlen(pszText);
		for (m = 0; m < ilen; m++)
		{
			i = (i + 1) &0xff;
			j = (j + *(sbox + i)) &0xff;
			SWAP(*(sbox+i),*(sbox + j));

			k = *(sbox + ((*(sbox + i) + *(sbox + j)) &0xff ));
			if(k == *(pszText + m))       
			       k = 0;
			*(pszText + m) ^=  k;
		}

		return pszText;
	  }

	  char *Decrypt(char *pszText,const char *pszKey)
	  {
	        return Encrypt(pszText,pszKey) ;
	  }

private:
          unsigned char sbox[256];
          unsigned char key[256],k;
	 int  m, n, i, j, ilen;
};
;