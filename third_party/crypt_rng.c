/*
 * crypt_rng.c
 *
 *  Created on: Jul 12, 2015
 *      Author: szilveszterb
 */

/*Copy CRYPT_RNG api here
 * Reason: we don't use WCM DK's cyassl, we have ported it for ourselves
 * to be able to compile WCM DK with the Exosite SDK we need these
 *
 * */

#include <app.h>
#include <crypto/crypto.h>
#include "crypto/src/error.h"

/* RNG Initialize, < 0 on error */
int CRYPT_RNG_Initialize(CRYPT_RNG_CTX* rng)
{
    typedef char rng_test[sizeof(CRYPT_RNG_CTX) >= sizeof(RNG) ? 1 : -1];
    (void)sizeof(rng_test);

    if (rng == NULL)
        return BAD_FUNC_ARG;

    return InitRng((RNG*)rng);
}

/*TODO this has to be here, because CYASSK from WCM DK is not used*/
/* RNG Get single bytes, < 0 on error */
int CRYPT_RNG_Get(CRYPT_RNG_CTX* rng, unsigned char* b)
{
    if (rng == NULL || b == NULL)
        return BAD_FUNC_ARG;

    return RNG_GenerateByte((RNG*)rng, (byte*)b);
}


/* RNG Block Generation of sz bytes, < 0 on error */
int CRYPT_RNG_BlockGenerate(CRYPT_RNG_CTX* rng, unsigned char* b,
                            unsigned int sz)
{
    if (rng == NULL || b == NULL)
        return BAD_FUNC_ARG;

    return RNG_GenerateBlock((RNG*)rng, b, sz);
}





