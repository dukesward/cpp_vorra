#ifndef _GLOB_DEFINE_H
#define _GLOB_DEFINE_H

#ifndef BOOST_ASIO_HAS_MOVE
	#define BOOST_ASIO_HAS_MOVE
#endif

#include <cstddef>
#include <cstdint>
#include <cinttypes>
#include <climits>
#include <cstring>
#include <memory>

#include <string>
#include <vector>
#include <unordered_map>

#include <iostream>

#include <stdio.h>
#include <stdarg.h>
#include <stdexcept>
#include <sstream>
#include <functional>

#define COMMON_API
#define _READ_BUF_SIZE_ 4096

#define _HEADER_HTTP_ROUTE     "HTTP/"
#define _HEADER_HTTP_HOST      "Host"
#define _HEADER_HTTP_CONN      "Connection"
#define _HEADER_HTTP_CACHE_CTL "Cache-Control"
#define _HEADER_HTTP_USR_AGENT "User-Agent"
#define _HEADER_HTTP_ACCEPT    "Accept"
#define _HEADER_HTTP_ENCODING  "Accept-Encoding"
#define _HEADER_HTTP_LANGUAGE  "Accept-Language"

// define convenient int types from std int
typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;
typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

#endif