#include <linux/types.h>
#include <linux/sunrpc/xdr.h>
#include <linux/sunrpc/svc.h>
#include "../vsnfs.h"
#include "vsnfsd.h"
#include "xdr.h"



/*
 * XDR decode functions
 * 0 on failure 
 * 1 on success
 */
int
vsnfssvc_decode_nullargs(struct svc_rqst *rqstp, __be32 *p, struct vsnfsd_nullargs *args)
{
	args->dummy = ntohl(*p++);
	return xdr_argsize_check(rqstp, p);
}



static __be32 *
decode_pathname(__be32 *p, char **namp, unsigned int *lenp)
{
        char            *name;
        unsigned int    i;

        if ((p = xdr_decode_string_inplace(p, namp, lenp, VSNFS_MAXPATHLEN)) != NULL) {
                for (i = 0, name = *namp; i < *lenp; i++, name++) {
                        if (*name == '\0')
                                return NULL;
 }
 }
        return p;
}


int
vsnfssvc_decode_getrootargs(struct svc_rqst *rqstp, __be32 *p, struct vsnfsd_getrootargs *args)
{
  if(!(p=decode_pathname(p, &args->path, &args->len)))
    return 0;

	return xdr_argsize_check(rqstp, p);
}


/*
 * XDR encode functions
 * 0 on error
 * 1 on success
 */
int
vsnfssvc_encode_nullres(struct svc_rqst *rqstp, __be32 *p, struct vsnfsd_nullres *resp)
{
	*p++ = htonl(resp->dummy);	
	return xdr_ressize_check(rqstp, p);
}

static __be32 *
encode_fh(__be32 *p, struct svc_fh *fhp)
{
        memcpy(p, &fhp->fh.data, VSNFS_FHSIZE);
        return p + (VSNFS_FHSIZE>> 2);
}



int
vsnfssvc_encode_fhandle(struct svc_rqst *rqstp, __be32 *p,
                                        struct vsnfsd_fhandle *resp)
{
        p = encode_fh(p, &resp->fh);
        return xdr_ressize_check(rqstp, p);
}