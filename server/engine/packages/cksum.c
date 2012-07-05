/*-
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef lint
#if 0
static char sccsid[] = "@(#)sum2.c	8.1 (Berkeley) 6/6/93";
#endif
#endif /* not lint */
#include <sys/cdefs.h>
__FBSDID("$FreeBSD: src/usr.bin/cksum/sum2.c,v 1.8.32.1 2009/04/15 03:14:26 kensmith Exp $");

#include <sys/types.h>

#include <unistd.h>
#include <stdint.h>

int cksum(const unsigned char *buf, off_t len, uint32_t *cval)
{
	uint32_t lcrc, tmpcrc;
	const unsigned char *p;
	off_t tmplen = len;

	/*
	 * Draft 8 POSIX 1003.2:
	 *
	 *   s = sum of all bytes
	 *   r = s % 2^16 + (s % 2^32) / 2^16
	 * lcrc = (r % 2^16) + r / 2^16
	 */
	lcrc = 0;
	for (p = buf; len--; ++p)
		lcrc += *p;

	/* Include the length of the file. */
	for (; tmplen != 0; tmplen >>= 8) {
		lcrc += (tmplen & 0xff);
	}

	tmpcrc = lcrc;
	tmpcrc = (tmpcrc & 0xffff) + (tmpcrc >> 16);
	tmpcrc = (tmpcrc & 0xffff) + (tmpcrc >> 16);
	for (; tmpcrc != 0; tmpcrc >>= 8) {
		lcrc += (tmpcrc & 0xff);
	}

	lcrc = (lcrc & 0xffff) + (lcrc >> 16);
	lcrc = (lcrc & 0xffff) + (lcrc >> 16);
	*cval = lcrc;

	return (0);
}

