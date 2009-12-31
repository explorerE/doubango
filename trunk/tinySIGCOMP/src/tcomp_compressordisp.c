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

/**@file tcomp_compressordisp.h
 * @brief  Entity that receives application messages, invokes a compressor,and forwards the resulting SigComp compressed messages to a remote
 * endpoint.
 *
 * @author Mamadou Diop <diopmamadou(at)yahoo.fr>
 *
 * @date Created: Sat Nov 8 16:54:58 2009 mdiop
 */
#include "tcomp_compressordisp.h"
#include "tcomp_compressor_dummy.h"
#include "tcomp_compressor_deflate.h"

#include "tsk_memory.h"
#include "tsk_debug.h"

/**@defgroup tcomp_compressordisp_group SIGCOMP decompressor dispatcher.
* Entity that receives application messages, invokes a compressor,and forwards the resulting SigComp compressed messages to a remote
* endpoint.
*/

#define TCOMP_NACK_SUPPORTED (dispatcher->stateHandler->sigcomp_parameters->SigComp_version >= 0x02)

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	int tcomp_compressordisp_compress(tcomp_compressordisp_t *dispatcher,
/// 	uint64_t compartmentId, const void *input_ptr, size_t input_size, void *output_ptr,
/// 	size_t *output_size, int stream)
///
/// @brief	Tcomp compressordisp compress. 
///
/// @author	Mamadou
/// @date	11/29/2009
///
/// @param [in,out]	dispatcher	The compressor dispatcher. 
/// @param	compartmentId		The compartment to use to compress the message. 
/// @param [in,out]	input_ptr	The message to compress. 
/// @param	input_size			The size of the input buffer. 
/// @param [in,out]	output_ptr	The output buffer to copy the compressed data. 
/// @param [in,out]	output_size	The size of the output buffer. 
/// @param	stream				Indicates whether it's a stream buffer ot not. 
///
/// @return	1 if succeed and 0 otherwize. 
////////////////////////////////////////////////////////////////////////////////////////////////////

int tcomp_compressordisp_compress(tcomp_compressordisp_t *dispatcher, uint64_t compartmentId, const void *input_ptr, size_t input_size, void *output_ptr, size_t *output_size, int stream)
{
	int ret = 1;
	int i = 0;

	/* For each compartment id create/retrieve one compressor instance */
	tcomp_compartment_t *lpCompartment = tcomp_statehandler_getCompartment(dispatcher->stateHandler, compartmentId);
	
	if(!lpCompartment)
	{
		TSK_DEBUG_ERROR("You must provide a valid compartment to perform compression.");
		return 0;
	}

	
	/*
	*	Performs compression.
	*/
	tsk_safeobj_lock(dispatcher);
	
	for( i = 0; (i < TCOMP_MAX_COMPRESSORS && dispatcher->compressors[i]); i++ )
	{
		if((ret = dispatcher->compressors[i](lpCompartment, input_ptr, input_size, output_ptr, output_size, stream))) 
		{
			break;
		}
	}

	tsk_safeobj_unlock(dispatcher);

	/*
	*	STREAM case. FIXME:Because I'm a lazy man I only support 0xFF00 case
	*/
	if(stream)
	{
		uint8_t* escapedBuffer;
		size_t i, j;
		size_t escapedBufferSize = (*output_size + 2); /* 2 = strlen(0xffff) */

		for(i = 0; i < *output_size ; i++) 
		{
			escapedBufferSize += ((uint8_t*)output_ptr)[i] == 0xff ? 1 : 0;
		}
		escapedBuffer = (uint8_t*)tsk_calloc(escapedBufferSize, sizeof(uint8_t));

		for(i = 0, j = 0; i < *output_size; i++, j++)
		{
			escapedBuffer[j] = ((uint8_t*)output_ptr)[i];
			if(escapedBuffer[j] == 0xff) 
			{
				escapedBuffer[++j] = 0x00;
			}
		}
		
		/* End stream */
		escapedBuffer[escapedBufferSize-1] = escapedBuffer[escapedBufferSize-2] = 0xff;
		memcpy(output_ptr, escapedBuffer, escapedBufferSize);
		
		*output_size = escapedBufferSize; /* Update size */
		TSK_FREE(escapedBuffer); /* free */
	}

	/*
	* NACK
	*/
	if(ret && TCOMP_NACK_SUPPORTED)
	{
		/* store nack for later retrieval in case of errors */
		uint8_t nackId[TSK_SHA1_DIGEST_SIZE];
		tsk_sha1context_t sha;

		tsk_sha1reset(&sha);
		tsk_sha1input(&sha, (const uint8_t*)output_ptr, *output_size);
		tsk_sha1result(&sha, nackId);
		tcomp_compartment_addNack(lpCompartment, nackId);
	}

	return ret;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/// @fn	void tcomp_compressordisp_addCompressor(tcomp_compressordisp_t *dispatcher,
/// 	tcomp_compressor_compress compressor)
///
/// @brief	Add new compressor the list of the compressors. 
///
/// @author	Mamadou
/// @date	11/29/2009
///
/// @param [in,out]	dispatcher	The compressor dispatcher. 
/// @param	compressor			A function pointer to the new compressor. 
////////////////////////////////////////////////////////////////////////////////////////////////////
void tcomp_compressordisp_addCompressor(tcomp_compressordisp_t *dispatcher, tcomp_compressor_compress compressor)
{
	int i;

	for(i = 0; i < TCOMP_MAX_COMPRESSORS; i++)
	{
		if(!dispatcher->compressors[i])
		{
			dispatcher->compressors[i] = compressor;
		}
	}
}









//========================================================
//	SigComp compressor dispatcher object definition
//

/**@ingroup tcomp_compressordisp_group
*/
static void* tcomp_compressordisp_create(void * self, va_list * app)
{
	tcomp_compressordisp_t *compressordisp = self;
	if(compressordisp)
	{
		int i = 0;
		compressordisp->stateHandler = va_arg(*app, const tcomp_statehandler_t*);

		compressordisp->compressors[0] = tcomp_compressor_deflate_compress;	/* If you don't want deflate compressor then remove this line. */
		compressordisp->compressors[1] = tcomp_compressor_dummy_compress;	/* RFC 4896 [11. Uncompressed Bytecode]. */
		
		for(i = 2; i < TCOMP_MAX_COMPRESSORS; i++)
		{
			compressordisp->compressors[i] = 0;
		}

		/* Initialize safeobject */
		tsk_safeobj_init(compressordisp);
	}
	else
	{
		TSK_DEBUG_ERROR("Failed to create new compressor dispatcher.");
	}

	return self;
}

/**@ingroup tcomp_compressordisp_group
*/
static void* tcomp_compressordisp_destroy(void *self)
{
	tcomp_compressordisp_t *compressordisp = self;
	if(compressordisp)
	{
		/* Deinitialize safeobject */
		tsk_safeobj_deinit(compressordisp);

		// FIXME: clear compressors
	}
	else TSK_DEBUG_ERROR("Null dispatcher.");
	
	return self;
}

static const tsk_object_def_t tcomp_compressordisp_def_s = 
{
	sizeof(tcomp_compressordisp_t),
	tcomp_compressordisp_create,
	tcomp_compressordisp_destroy,
	0
};
const void *tcomp_compressordisp_def_t = &tcomp_compressordisp_def_s;