/*!The Tiny Box Library
 * 
 * TBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * TBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with TBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2010, ruki All rights reserved.
 *
 * \author		ruki
 * \file		stream.h
 *
 */
#ifndef TB_STREAM_H
#define TB_STREAM_H

// c plus plus
#ifdef __cplusplus
extern "C" {
#endif

/* /////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "bits.h"
#include "string.h"

#ifdef TB_CONFIG_ZLIB
# 	include "zlib.h"
#endif
/* /////////////////////////////////////////////////////////
 * macros
 */

#ifdef TB_MEMORY_MODE_SMALL
# 	define TB_STREAM_DATA_MAX 		(4096)
# 	define TB_STREAM_ZDATA_MAX 		(TB_STREAM_DATA_MAX >> 1)
#else
# 	define TB_STREAM_DATA_MAX 		(8192)
# 	define TB_STREAM_ZDATA_MAX 		(TB_STREAM_DATA_MAX >> 1)
#endif

/* /////////////////////////////////////////////////////////
 * types
 */

// the stream flag type
typedef enum __tb_stream_flag_t
{
	TB_STREAM_FLAG_NULL 			= 0
, 	TB_STREAM_FLAG_RO 				= 1
, 	TB_STREAM_FLAG_WO 				= 2
, 	TB_STREAM_FLAG_TRUNC 			= 4
, 	TB_STREAM_FLAG_ZLIB 			= 8
, 	TB_STREAM_FLAG_BLOCK 			= 16


}tb_stream_flag_t;

// the stream seek type
typedef enum __tb_stream_seek_t
{
 	TB_STREAM_SEEK_BEG 				= 0
, 	TB_STREAM_SEEK_CUR 				= 1
, 	TB_STREAM_SEEK_END 				= 2

}tb_stream_seek_t;

// the stream type
typedef struct __tb_stream_t
{
	// the stream flag
	tb_uint32_t 		flag;

	// the stream data
	tb_byte_t 			data[TB_STREAM_DATA_MAX];
	tb_byte_t* 			head;
	tb_size_t 			size;
	tb_size_t 			offset;

	// the private data
	tb_byte_t* 			pdata;

#ifdef TB_CONFIG_ZLIB

	// for zlib
	tb_handle_t 		hzlib;

	// the stream zdata, size must be TB_STREAM_DATA_MAX for tb_stream_switch
	tb_byte_t 			zdata[TB_STREAM_DATA_MAX];

#endif

	// the url
	tb_string_t 		url;

	// the operations 
	tb_int_t 			(*read)(struct __tb_stream_t* st, tb_byte_t* data, tb_size_t size);
	tb_int_t 			(*write)(struct __tb_stream_t* st, tb_byte_t* data, tb_size_t size);
	tb_bool_t 			(*seek)(struct __tb_stream_t* st, tb_int_t offset, tb_stream_seek_t flag);
	tb_byte_t* 			(*need)(struct __tb_stream_t* st, tb_size_t size);
	void 				(*close)(struct __tb_stream_t* st);
	tb_size_t 			(*ssize)(struct __tb_stream_t* st);

}tb_stream_t;

// the http stream
typedef struct __tb_http_stream_t
{
	tb_stream_t 		base;
	tplat_handle_t 		hsocket;
	tb_size_t 			size;

}tb_http_stream_t;

// the file stream
typedef struct __tb_data_stream_t
{
	tb_stream_t 		base;
	tb_byte_t* 			data;
	tb_byte_t* 			head;
	tb_size_t 			size;

}tb_data_stream_t;

// the file stream
typedef struct __tb_file_stream_t
{
	tb_stream_t 		base;
	tplat_handle_t 		hfile;

}tb_file_stream_t;

// the generic stream
typedef struct __tb_generic_stream_t
{
	// streams
	union
	{
		tb_data_stream_t 	data;
		tb_http_stream_t 	http;
		tb_file_stream_t 	file;

	}u;

	// the reference to the specific stream
	tb_stream_t* 			st;

}tb_generic_stream_t;

/* /////////////////////////////////////////////////////////
 * interfaces
 */

// the stream operation
tb_stream_t* 		tb_stream_open_from_http(tb_http_stream_t* st, tb_char_t const* url, tb_stream_flag_t flag);
tb_stream_t* 		tb_stream_open_from_file(tb_file_stream_t* st, tb_char_t const* url, tb_stream_flag_t flag);
tb_stream_t* 		tb_stream_open_from_data(tb_data_stream_t* st, tb_byte_t const* data, tb_size_t size, tb_stream_flag_t flag);
tb_stream_t* 		tb_stream_open(tb_generic_stream_t* st, tb_char_t const* url, tb_byte_t const* data, tb_size_t size, tb_stream_flag_t flag);
void 				tb_stream_close(tb_stream_t* st);

tb_int_t 			tb_stream_read(tb_stream_t* st, tb_byte_t* data, tb_size_t size);
tb_int_t 			tb_stream_write(tb_stream_t* st, tb_byte_t* data, tb_size_t size);
tb_byte_t* 			tb_stream_need(tb_stream_t* st, tb_size_t size);
tb_bool_t 			tb_stream_seek(tb_stream_t* st, tb_int_t offset, tb_stream_seek_t flag);
void 				tb_stream_flush(tb_stream_t* st);
tb_int_t 			tb_stream_printf(tb_stream_t* st, tb_char_t const* fmt, ...);
tb_size_t 			tb_stream_size(tb_stream_t const* st);
tb_size_t 			tb_stream_offset(tb_stream_t const* st);
tb_stream_flag_t 	tb_stream_flag(tb_stream_t const* st);
tb_bool_t 			tb_stream_switch(tb_stream_t* st, tb_stream_flag_t flag);
tb_char_t const* 	tb_stream_url(tb_stream_t const* st);

// c plus plus
#ifdef __cplusplus
}
#endif

#endif
