/*
* Copyright (C) 2009 Mamadou Diop.
*
* Contact: Mamadou Diop <diopmamadou@yahoo.fr>
*	
* This file is part of Open Source Doubango Framework.
*
* DOUBANGO is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*	
* DOUBANGO is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*	
* You should have received a copy of the GNU General Public License
* along with DOUBANGO.
*
*/

/**@file tsip_header_Authorization.h
 * @brief SIP header 'Authorization'.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#ifndef _TSIP_HEADER_AUTHORIZATION_H_
#define _TSIP_HEADER_AUTHORIZATION_H_

#include "tinysip_config.h"
#include "tinysip/headers/tsip_header.h"

/**@def TSIP_HEADER_AUTHORIZATION_CREATE
* Creates new sip 'AUTHORIZATION' header.  You must call @ref TSIP_HEADER_AUTHORIZATION_SAFE_FREE to free the header.
* @sa TSIP_HEADER_AUTHORIZATION_SAFE_FREE.
*/
/**@def TSIP_HEADER_AUTHORIZATION_SAFE_FREE
* Safely free a sip 'AUTHORIZATION' header previously created using TSIP_HEADER_AUTHORIZATION_CREATE.
* @sa TSIP_HEADER_AUTHORIZATION_CREATE.
*/
#define TSIP_HEADER_AUTHORIZATION_CREATE()				tsk_object_new(tsip_header_Authorization_def_t)
#define TSIP_HEADER_AUTHORIZATION_SAFE_FREE(self)		tsk_object_unref(self), self = 0

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @struct	
///
/// @brief	SIP header 'Authorization' as per RFC 3261 subclause .
/// @author	Mamadou
/// @date	12/3/2009
///
/// @par ABNF = Authorization	= 	"Authorization" HCOLON credentials
/// 								credentials	= 	("Digest" LWS digest-response) / other-response
/// 								digest-response	= 	dig-resp *(COMMA dig-resp)
/// 								dig-resp	= 	username / realm / nonce / digest-uri / dresponse / algorithm / cnonce / opaque / message-qop / nonce-count / auth-param / auts
/// 								username	= 	"username" EQUAL username-value
/// 								username-value	= 	quoted-string
/// 								digest-uri	= 	"uri" EQUAL LDQUOT digest-uri-value RDQUOT
/// 								digest-uri-value	= 	auth-request-uri ; equal to request-uri as specified by HTTP/1.1
/// 								message-qop	= 	"qop" EQUAL qop-value
/// 								cnonce	= 	"cnonce" EQUAL cnonce-value
/// 								cnonce-value	= 	nonce-value
/// 								nonce-count	= 	"nc" EQUAL nc-value
/// 								nc-value	= 	8LHEX
/// 								dresponse	= 	"response" EQUAL request-digest
/// 								request-digest	= 	LDQUOT 32LHEX RDQUOT
/// 								auth-request-uri = not-defined
/// 								 
/// 								auth-param	= 	auth-param-name EQUAL ( token / quoted-string )
/// 								auth-param-name	= 	token
/// 								 
/// 								other-response	= 	auth-scheme LWS auth-param *(COMMA auth-param)
/// 								auth-scheme	= 	token
/// 								auts	= 	"auts" EQUAL auts-param
/// 								auts-param	= 	LDQUOT auts-value RDQUOT
/// 								auts-value	= 	<base64 encoding of AUTS>
/// 	
////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tsip_header_Authorization_s
{	
	TSIP_DECLARE_HEADER;

	char* scheme;
	char* username;
	char* realm;
	char* nonce;
	char* uri;
	char* response;
	char* algorithm;
	char* cnonce;
	char* opaque;
	char* qop;
	char* nc;
}
tsip_header_Authorization_t;

tsip_header_Authorization_t *tsip_header_Authorization_parse(const char *data, size_t size);

TINYSIP_API const void *tsip_header_Authorization_def_t;

#endif /* _TSIP_HEADER_AUTHORIZATION_H_ */