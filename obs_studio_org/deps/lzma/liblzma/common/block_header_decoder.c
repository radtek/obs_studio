//////////////////////////////////基本信息///////////////////////////////////////////////////////  
// ><免责声明 ><  Copyright (c) 2017-2017 by Xie Zhimin All Rights Reserved  
// ><创建日期 ><  2017/03/21  
// ><创建时间 ><  2017年:03月:21日   18时:12分:06秒  
// ><文件     ><  block_header_decoder.c  
// ><文件路径 ><  D:\newSvnCode\OBS\trunk\obs_studio\deps\lzma\liblzma\common  
// ><隶属工程><   obs-studio  
// ><当前用户 ><  Administrator  
// ><作者     ><  Open Broadcaster Software   
// ><出处     >< 《 https://obsproject.com/ 》  
// ><目的     >< 【】  
// ><设计技术 ><   
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
// ><         ><  4.  
//////////////////////////////////迭代修改///////////////////////////////////////////////////////  
// ><作者     ><  xzm  
// ><修改日期 ><  2017年:03月:21日   18时:12分:06秒  
// ><原因     ><    
// ><         ><  1.  
// ><         ><  2.  
// ><         ><  3.  
/////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
/// \file       block_header_decoder.c
/// \brief      Decodes Block Header from .xz files
//
//  Author:     Lasse Collin
//
//  This file has been put into the public domain.
//  You can do whatever you want with this file.
//
///////////////////////////////////////////////////////////////////////////////

#include "common.h"
#include "check.h"


static void
free_properties(lzma_block *block, lzma_allocator *allocator)
{
	// Free allocated filter options. The last array member is not
	// touched after the initialization in the beginning of
	// lzma_block_header_decode(), so we don't need to touch that here.
	for (size_t i = 0; i < LZMA_FILTERS_MAX; ++i) {
		lzma_free(block->filters[i].options, allocator);
		block->filters[i].id = LZMA_VLI_UNKNOWN;
		block->filters[i].options = NULL;
	}

	return;
}


extern LZMA_API(lzma_ret)
lzma_block_header_decode(lzma_block *block,
		lzma_allocator *allocator, const uint8_t *in)
{
	// NOTE: We consider the header to be corrupt not only when the
	// CRC32 doesn't match, but also when variable-length integers
	// are invalid or over 63 bits, or if the header is too small
	// to contain the claimed information.

	// Initialize the filter options array. This way the caller can
	// safely free() the options even if an error occurs in this function.
	for (size_t i = 0; i <= LZMA_FILTERS_MAX; ++i) {
		block->filters[i].id = LZMA_VLI_UNKNOWN;
		block->filters[i].options = NULL;
	}

	// Always zero for now.
	block->version = 0;

	// Validate Block Header Size and Check type. The caller must have
	// already set these, so it is a programming error if this test fails.
	if (lzma_block_header_size_decode(in[0]) != block->header_size
			|| (unsigned int)(block->check) > LZMA_CHECK_ID_MAX)
		return LZMA_PROG_ERROR;

	// Exclude the CRC32 field.
	const size_t in_size = block->header_size - 4;

	// Verify CRC32
	if (lzma_crc32(in, in_size, 0) != unaligned_read32le(in + in_size))
		return LZMA_DATA_ERROR;

	// Check for unsupported flags.
	if (in[1] & 0x3C)
		return LZMA_OPTIONS_ERROR;

	// Start after the Block Header Size and Block Flags fields.
	size_t in_pos = 2;

	// Compressed Size
	if (in[1] & 0x40) {
		return_if_error(lzma_vli_decode(&block->compressed_size,
				NULL, in, &in_pos, in_size));

		// Validate Compressed Size. This checks that it isn't zero
		// and that the total size of the Block is a valid VLI.
		if (lzma_block_unpadded_size(block) == 0)
			return LZMA_DATA_ERROR;
	} else {
		block->compressed_size = LZMA_VLI_UNKNOWN;
	}

	// Uncompressed Size
	if (in[1] & 0x80)
		return_if_error(lzma_vli_decode(&block->uncompressed_size,
				NULL, in, &in_pos, in_size));
	else
		block->uncompressed_size = LZMA_VLI_UNKNOWN;

	// Filter Flags
	const size_t filter_count = (in[1] & 3) + 1;
	for (size_t i = 0; i < filter_count; ++i) {
		const lzma_ret ret = lzma_filter_flags_decode(
				&block->filters[i], allocator,
				in, &in_pos, in_size);
		if (ret != LZMA_OK) {
			free_properties(block, allocator);
			return ret;
		}
	}

	// Padding
	while (in_pos < in_size) {
		if (in[in_pos++] != 0x00) {
			free_properties(block, allocator);

			// Possibly some new field present so use
			// LZMA_OPTIONS_ERROR instead of LZMA_DATA_ERROR.
			return LZMA_OPTIONS_ERROR;
		}
	}

	return LZMA_OK;
}
