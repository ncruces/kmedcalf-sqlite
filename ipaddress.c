/*
** This SQLite extension implements an ipaddrblob/ipblobaddr functions
** to convert an IP Address between Presentation format and a 16-byte
** network byte order blob; an IPADDRESS collation; and, a function
** to test whether a subnet contains a particular address.
**
** IPAddrBlob accepts either IPv4 or IPv6 Presentation format input
** which may be in CIDR format (address/masklen).  If the provided
** input is a CIDR Subnet, the subnet mask length will be ignored
** and conversion will occur using the part of the input string
** preceding the / separator.
**
** IPv4 Presentation will output a 16-byte Network Byte Order blob
** in IPv6 compatibility format (::ffff:a.b.c.d).
**
** IPBlobAddr accepts a 16-byte Network Byte Order blob and returns
** either an IPv6 Presentation string or, if the input blob is an
** IPv6 compatible IPv4 address (::ffff:a.b.c.d) then an IPv4
** Presentation string.
**
** Invalid input strings or input blobs will return NULL.
**
** COLLATE IPADDRESS implements an IPADDRESS collation which performs
** ipaddrblob on the two inputs to be sorted, and orders the resulting
** Network Byte Order blobs using BINARY collation.
**
** IPSubnetContains(a, b) returns True (1) if the IPAddress b is
** within the subnet defined by IPSubnet a.  Inputs are strings
** in Presentation format.  IPSubnet (argument a) must be a subnet
** with the subnet length following a '/' at the end of the address
** part (ie, in CIDR notation).  False (0) is returned if the b
** address is not in the subnet defined by a.  NULL is returned if
** a is not a CIDR subnet or the Presentation formats cannot be decoded.
**
** The SQLite functions are original code by Keith Medcalf and are
** released unencumbered into the public domain.
**
** The inet_PtoN and inet_NtoP code is reproduced inline and contains
** the accreditations and notices which provide that as long as the
** notices remain intact, the code may be freely used and modified.
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SQLITE_PRIVATE
    #define SQLITE_PRIVATE static
#endif

#ifdef SQLITE_CORE
    #include "sqlite3.h"
#else
    #ifdef _HAVE_SQLITE_CONFIG_H
        #include "config.h"
    #endif
    #include "sqlite3ext.h"
    SQLITE_EXTENSION_INIT1
#endif

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* Begin Included Code (with slight modifications by Keith Medcalf) */

/* This is from the BIND 4.9.4 release, modified to compile by itself */

/* Copyright (c) 1996 by Internet Software Consortium.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND INTERNET SOFTWARE CONSORTIUM DISCLAIMS
 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL INTERNET SOFTWARE
 * CONSORTIUM BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR
 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS
 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/* Original Author:  Paul Vixie, 1996 */

static __inline int _inet_pton4(const char *src, unsigned char *dst)
{
    static const char digits[] = "0123456789";
    int saw_digit, octets, ch;
    unsigned char tmp[4], *tp;

    saw_digit = 0;
    octets = 0;
    tp = tmp;
    *tp = 0;
    while((ch = *src++) != '\0')
    {
        const char *pch;
        if((pch = strchr(digits, ch)) != NULL)
        {
            unsigned int val = *tp * 10 + (unsigned int)(pch - digits);
            if(saw_digit && *tp == 0)
            {
                return (0);
            }
            if(val > 255)
            {
                return (0);
            }
            *tp = (unsigned char)val;
            if(! saw_digit)
            {
                if(++octets > 4)
                {
                    return (0);
                }
                saw_digit = 1;
            }
        }
        else if(ch == '.' && saw_digit)
        {
            if(octets == 4)
            {
                return (0);
            }
            *++tp = 0;
            saw_digit = 0;
        }
        else
        {
            return (0);
        }
    }
    if(octets < 4)
    {
        return (0);
    }
    memcpy(dst, tmp, 4);
    return (1);
}

static __inline int _inet_pton6(const char *src, unsigned char *dst)
{
    static const char xdigits_l[] = "0123456789abcdef",
                      xdigits_u[] = "0123456789ABCDEF";
    unsigned char tmp[16], *tp, *endp, *colonp;
    const char *xdigits, *curtok;
    int ch, saw_xdigit;
    unsigned int val;

    tp = tmp;
    memset(tmp, 0, 16);
    endp = tp + 16;
    colonp = NULL;
    /* Leading :: requires some special handling. */
    if (*src == ':')
    {
        if(*++src != ':')
        {
            return (0);
        }
    }
    curtok = src;
    saw_xdigit = 0;
    val = 0;
    while((ch = *src++) != '\0')
    {
        const char *pch;
        if((pch = strchr((xdigits = xdigits_l), ch)) == NULL)
        {
            pch = strchr((xdigits = xdigits_u), ch);
        }
        if(pch != NULL)
        {
            val <<= 4;
            val |= (pch - xdigits);
            if(++saw_xdigit > 4)
            {
                return (0);
            }
            continue;
        }
        if(ch == ':')
        {
            curtok = src;
            if(!saw_xdigit)
            {
                if(colonp)
                {
                    return (0);
                }
                colonp = tp;
                continue;
            }
            if(tp + 2 > endp)
            {
                return (0);
            }
            *tp++ = (unsigned char) (val >> 8) & 0xff;
            *tp++ = (unsigned char) val & 0xff;
            saw_xdigit = 0;
            val = 0;
            continue;
        }
        if(ch == '.' && ((tp + 4) <= endp) && _inet_pton4(curtok, tp) > 0)
        {
            tp += 4;
            saw_xdigit = 0;
            break;    /* '\0' was seen by inet_pton4(). */
        }
        return (0);
    }
    if(saw_xdigit)
    {
        if(tp + 2 > endp)
        {
            return (0);
        }
        *tp++ = (unsigned char) (val >> 8) & 0xff;
        *tp++ = (unsigned char) val & 0xff;
    }
    if(colonp != NULL)
    {
        /*
         * Since some memmove()'s erroneously fail to handle
         * overlapping regions, we'll do the shift by hand.
         */
        const long n = tp - colonp;
        long i;

        if(tp == endp)
        {
            return (0);
        }
        for (i = 1; i <= n; i++)
        {
            endp[- i] = colonp[n - i];
            colonp[n - i] = 0;
        }
        tp = endp;
    }
    if(tp != endp)
    {
        return (0);
    }
    memcpy(dst, tmp, 16);
    return (1);
}

static __inline const char * _inet_ntop4(const unsigned char *src, char *dst, size_t size)
{
    static const char fmt[] = "%u.%u.%u.%u";
    char tmp[sizeof "255.255.255.255"];

    if (sprintf(tmp, fmt, src[0], src[1], src[2], src[3]) > size)
    {
        return (NULL);
    }
    strcpy(dst, tmp);
    return (dst);
}

static __inline const char * _inet_ntop6(const unsigned char *src, char *dst, size_t size)
{
    char tmp[48], *tp;
    struct { int base, len; } best, cur;
    unsigned short words[8];
    int i;

    /*
     * Preprocess:
     *  Copy the input (bytewise) array into a wordwise array.
     *  Find the longest run of 0x00's in src[] for :: shorthanding.
     */
    memset(words, '\0', 16);
    for (i = 0; i < 16; i++)
    {
        words[i / 2] |= (src[i] << ((1 - (i % 2)) << 3));
    }
    best.base = -1;
    cur.base = -1;
    for (i = 0; i < 8; i++)
    {
        if (words[i] == 0)
        {
            if (cur.base == -1)
            {
                cur.base = i;
                cur.len = 1;
            }
            else
            {
                cur.len++;
            }
        }
        else
        {
            if (cur.base != -1)
            {
                if (best.base == -1 || cur.len > best.len)
                {
                    best = cur;
                }
                cur.base = -1;
            }
        }
    }
    if (cur.base != -1)
    {
        if (best.base == -1 || cur.len > best.len)
        {
            best = cur;
        }
    }
    if (best.base != -1 && best.len < 2)
    {
        best.base = -1;
    }

    /*
     * Format the result.
     */
    tp = tmp;
    for (i = 0; i < 8; i++)
    {
        /* Are we inside the best run of 0x00's? */
        if (best.base != -1 && i >= best.base && i < (best.base + best.len))
        {
            if (i == best.base)
            {
                *tp++ = ':';
            }
            continue;
        }
        /* Are we following an initial run of 0x00s or any real hex? */
        if (i != 0)
        {
            *tp++ = ':';
        }
        /* Is this address an encapsulated IPv4? */
        if (i == 6 && best.base == 0 && (best.len == 6 || (best.len == 5 && words[5] == 0xffff)))
        {
            if (!_inet_ntop4(src+12, tp, sizeof tmp - (tp - tmp)))
            {
                return (NULL);
            }
            tp += strlen(tp);
            break;
        }
        tp += sprintf(tp, "%x", words[i]);
    }
    /* Was it a trailing run of 0x00's? */
    if (best.base != -1 && (best.base + best.len) == 8)
    {
        *tp++ = ':';
    }
    *tp++ = '\0';

    /*
     * Check for overflow, copy, and we're done.
     */
    if ((size_t)(tp - tmp) > size)
    {
        return (NULL);
    }
    strcpy(dst, tmp);
    return (dst);
}

/* End of ISC / Paul Vixie's NtoP and PtoN code */

/* Start of code written by Keith Medcalf */

static __inline int _ipconvert(const char *src, void *dst)
{
    int rc;
    unsigned char outbuf[16];
    unsigned short *oct = (unsigned short *)outbuf;
    rc = _inet_pton6(src, outbuf);
    if (rc <=0)
    {
        oct[0] = 0;
        oct[1] = 0;
        oct[2] = 0;
        oct[3] = 0;
        oct[4] = 0;
        oct[5] = 0xffff;
        oct[6] = 0;
        oct[7] = 0;
        rc = _inet_pton4(src, (void*)&oct[6]);
    }
    if (rc==1)
    {
        memcpy(dst, outbuf, 16);
    }
    return (rc);
}

/*
** Implementation of the ipaddrblob() function.
**
** Input either an IPv4 or IPv6 address string and return
** a blob in network byte order containing the address
** with IPv4 addresses encoded in IPv6 compatibility
** format (::ffff:a.b.c.d) (blob length is always 16 bytes)
**
** If a CIDR mask is present it is ignored (by replacing the
** '/' with a null byte to terminate the string.
**
*/
static void _ipaddrblobFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    int i, j;
    unsigned char buf[16];
    unsigned char working[1024];
    if (sqlite3_value_type(argv[0])==SQLITE_NULL)
    {
        sqlite3_result_null(context);
        return;
    }
    memset(working, 0, 1024);
    i = sqlite3_value_bytes(argv[0]);
    if (i>1023)
    {
        i = 1023;
    }
    memcpy(working, sqlite3_value_text(argv[0]), i);
    for (j=0; j<i; j++)
    {
        if (working[j] == '/')
        {
            working[j] = '\0';
            break;
        }
    }
    if (_ipconvert(working, buf) <= 0)
    {
        sqlite3_result_null(context);
        return;
    }
    sqlite3_result_blob(context, buf, 16, SQLITE_TRANSIENT);
}

/*
** Implementation of the ipblobaddr() function
**
** Input a 16-byte blob containing an IPv6 address in
** Network Byte Order (big endian) and convert to the
** Presentation format string.  If the input blob is
** and IPv4 address in IPv6 compatibility format,
** the returned string is in IPv4 Presentation Format.
*/
static void _ipblobaddrFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    unsigned char *ip;
    int i;
    int hasdot = 0;
    int hascolon = 0;
    unsigned char buf[1024];
    if ((sqlite3_value_type(argv[0])==SQLITE_NULL) || (sqlite3_value_bytes(argv[0])!=16))
    {
        sqlite3_result_null(context);
        return;
    }
    if (_inet_ntop6((void*)sqlite3_value_blob(argv[0]), buf, 1023) <= 0)
    {
        sqlite3_result_null(context);
        return;
    }
    ip = buf;
    if (memcmp("::ffff:", ip, 7) == 0)
    {
        ip += 6;
        while (*ip++ != 0)
        {
            if (*ip == '.')
            {
                hasdot = 1;
            }
            else if (*ip == ':')
            {
                hascolon = 1;
            }
        }
        ip = buf;
        if (hasdot && (!hascolon))
        {
            ip += 7;
        }
    }
    sqlite3_result_text(context, ip, -1, SQLITE_TRANSIENT);
}

/*
** Implementation of IPSubnetContains
**
** Takes two parameters the first of which SHOULD be in CIDR notation
** The second parameter is an IPAddress (which MAY be in CIDR notation)
** but the mask length of parameter 2, if provided, is ignored.
** If parameter 1 does not have a subnet length it is treated as /128
** If the subnet length is not between 0 and 128, null is returned
**
** Convert the two parameters to Network Byte Order
** If the mask length is zero, return True
** Compute the subnet mask
** Apply the subnet mask to the two parameters
** Return True if the masked parameters compare equal
*/

static void _ipsubnetcontains(sqlite3_context *context, int argc, sqlite3_value **argv)
{
    int i, j;
    int hasdot = 0;
    int hasslash = 0;
    int hascolon = 0;
    int slashpos = 0;
    int masklen = 128;
    int fullbytes = 0;
    int bits = 0;
    char subnetblob[16];
    char addresblob[16];
    unsigned char working[1024];

    /* Compute Source and Subnet - first arg */

    memset(working, 0, 1024);
    i = sqlite3_value_bytes(argv[0]);
    if (i>1023)
    {
        i = 1023;
    }
    memcpy(working, sqlite3_value_text(argv[0]), i);
    for (j=0; j<i; j++)
    {
        if (working[j] == '.')
        {
            hasdot = 1;
        }
        else if (working[j] == ':')
        {
            hascolon = 1;
        }
        else if (working[j] == '/')
        {
            slashpos = j;
            hasslash = 1;
            working[j] = '\0';
            break;
        }
    }
    if (hasslash)
    {
        masklen = atoi(&working[slashpos+1]);
        if ((hasdot) && (!hascolon))
        {
            masklen += 96;
        }
    }
    if (masklen > 128 || masklen < 0)
    {
        sqlite3_result_null(context);
        return;
    }
    if (_ipconvert(working, subnetblob) <= 0)
    {
        sqlite3_result_null(context);
        return;
    }

    /* Compute Source IP ignoring subnet if any - second arg */

    memset(working, 0, 1024);
    i = sqlite3_value_bytes(argv[1]);
    if (i>1023)
    {
        i = 1023;
    }
    memcpy(working, sqlite3_value_text(argv[1]), i);
    for (j=0; j<i; j++)
    {
        if (working[j] == '/')
        {
            working[j] = '\0';
            break;
        }
    }
    if (_ipconvert(working, addresblob) <= 0)
    {
        sqlite3_result_null(context);
        return;
    }

    /* Short Circuit zero subnet mask */
    if (masklen == 0)
    {
        sqlite3_result_int(context, 1);
        return;
    }

    /* Compute and Apply the Mask for Comparison of masklen < 128 */

    if (masklen < 128)
    {
        memset(working, 0, 1024);
        fullbytes = masklen / 8;
        bits = masklen - (fullbytes * 8);
        for (i=0; i<fullbytes; i++)
        {
            working[i] = 0xff;
        }
        for (i=0; i<bits; i++)
        {
            working[fullbytes] |= (1 << (7-i));
        }

        /* Apply Mask to both comparands */

        for (i=0; i<16; i++)
        {
            subnetblob[i] &= working[i];
            addresblob[i] &= working[i];
        }
    }

    /* Return Truth value of Compare Equal */

    sqlite3_result_int(context, memcmp(subnetblob, addresblob, 16) == 0);
}


/*
** The input addresses are converted to network byte order binary
** values and the result of a binary comparison is returned.  If
** a subnet is input, the CIDR is ignored by replacing the '/'
** with a NULL byte to terminate the string.
**
** Invalid Presentation inputs sort first
*/

static int _ipaddressCollFunc(void *notUsed, int nKey1, const char *pKey1, int nKey2, const char *pKey2)
{
    int i, j;
    char buf1[16];
    char buf2[16];
    char inbuf[1024];

    memset(buf1, 0, 16);
    memset(buf2, 0, 16);

    /* Process Arg1 */

    i = nKey1 > 1023 ? 1023 : nKey1;
    memset(inbuf, 0, 1024);
    memcpy(inbuf, pKey1, i);
    for (j=0; j<i; j++)
    {
        if (inbuf[j] == '/')
        {
            inbuf[j] = '\0';
            break;
        }
    }
    _ipconvert(inbuf, buf1);

    /* Process Arg2 */

    memset(inbuf, 0, 1024);
    i = nKey2 > 1023 ? 1023 : nKey2;
    memcpy(inbuf, pKey2, i);
    for (j=0; j<i; j++)
    {
        if (inbuf[j] == '/')
        {
            inbuf[j] = '\0';
            break;
        }
    }
    _ipconvert(inbuf, buf2);

    /* Return Binary Comparison */

    return memcmp(buf1, buf2, 16);
}

#ifdef _WIN32
#ifndef SQLITE_CORE
__declspec(dllexport)
#endif
#endif
int sqlite3_ipaddress_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
    int rc = SQLITE_OK;
    SQLITE_EXTENSION_INIT2(pApi);
    (void)pzErrMsg;  /* Unused parameter */
    rc = sqlite3_create_function(db, "ipaddrblob", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _ipaddrblobFunc, 0, 0);
    if (rc==SQLITE_OK) {
        rc = sqlite3_create_function(db, "ipblobaddr", 1, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _ipblobaddrFunc, 0, 0);
    }
    if (rc==SQLITE_OK) {
        rc = sqlite3_create_function(db, "ipsubnetcontains", 2, SQLITE_UTF8|SQLITE_DETERMINISTIC|SQLITE_INNOCUOUS, 0, _ipsubnetcontains, 0, 0);
    }
    if (rc==SQLITE_OK) {
        rc = sqlite3_create_collation(db, "IPADDRESS", SQLITE_UTF8, 0, (void*)_ipaddressCollFunc);
    }
    return rc;
}

#ifdef __cplusplus
}
#endif
