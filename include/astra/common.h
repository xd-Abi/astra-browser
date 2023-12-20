#pragma once

#include <stdint.h>
#include <memory>
#include <string>
#include <Windows.h>
#include <Windowsx.h>

/* These are basic types and are supported by almost every compiler and platform */

/* Unsigned base types */

typedef unsigned char 		uint8; 		/* 8-bit  unsigned integer */
typedef unsigned short int	uint16; 	/* 16-bit unsigned integer */
typedef unsigned int		uint32;		/* 32-bit unsigned integer */
typedef unsigned long long	uint64;		/* 64-bit unsigned integer */

/* Signed base types. */

typedef	signed char			int8;		/* 8-bit  signed integer */
typedef signed short int	int16; 		/* 16-bit signed integer */
typedef signed int			int32; 		/* 32-bit signed integer */
typedef signed long long	int64;		/* 64-bit signed integer */

/* Character types */

typedef char				AnsiChar;
typedef wchar_t				WideChar;
typedef uint8				char8;
typedef uint16				char16;
typedef uint32				char32;

typedef size_t size;

namespace Astra {

	template <typename T>
	using Unique = std::unique_ptr<T>;
	template <typename T, typename... Args>
	constexpr Unique<T> MakeUnique(Args&&... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template <typename T>
	using Ref = std::shared_ptr<T>;
	template <typename T, typename... Args>
	constexpr Ref<T> MakeRef(Args&&... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	using String = std::string;
}