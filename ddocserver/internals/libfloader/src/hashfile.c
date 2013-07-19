#include "floader.h"
#include <openssl/evp.h>

#define BLOCK_SIZE 8192

unsigned char * hashFile(const char * url, int * digestLength)
{
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    char buffer[BLOCK_SIZE];
    unsigned char * md_value;
    int md_len;
    int64 readed;
    FILE * fFile = NULL;


    fFile = openFile(fFile, url, "rb");

    if (fFile == NULL){
        elog(ERROR, "Cannot open file with given URL. \
                File does not exist or permission denied. URL='%s'", 
                url);

        return NULL;
    }

    md_value = malloc(EVP_MAX_MD_SIZE);
    
    OpenSSL_add_all_digests();

    //md = EVP_get_digestbyname("sha256");
    md = EVP_sha1();

    if(!md) {
        elog(ERROR, "An error was occured while creating message digest!");
        return NULL;
    }

    mdctx = EVP_MD_CTX_create();

    EVP_DigestInit_ex(mdctx, md, NULL);

    while (! feof(fFile) ){
        readed = fread(buffer, 1, BLOCK_SIZE, fFile);
        
        if(readed != 0)
            EVP_DigestUpdate(mdctx, buffer, readed);
    }

    EVP_DigestFinal_ex(mdctx, md_value, &md_len);

    EVP_MD_CTX_destroy(mdctx);

    if(digestLength){
        *digestLength = md_len;
    }

    return md_value;
}
