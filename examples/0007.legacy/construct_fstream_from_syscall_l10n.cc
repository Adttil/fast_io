﻿#include<fast_io_legacy.h>
#include<fstream>
#include<fast_io_i18n.h>

int main()
{
/*
This is an example to explain how fast_io's files work with each other, and how you could use fast_io with existing FILE* or fstream apis
*/
#if (defined(_WIN32) && !defined(__WINE__)) || defined(__CYGWIN__)
#ifdef _WIN32_WINDOWS
//_WIN32_WINDOWS macro targets WIN9X kernel (windows 95, windows 98 and windows ME)
//On 9x kernel, although ntdll.dll does exist, it does not do anything.
	fast_io::win32_file nf("win32_file.txt",fast_io::open_mode::out);
#else
//NT kernel
	fast_io::nt_file nf("nt_file.txt",fast_io::open_mode::out);
#endif
	fast_io::posix_file pf(std::move(nf),fast_io::open_mode::out);
#else
	fast_io::posix_file pf("posix_file.txt",fast_io::open_mode::out);
#endif
	fast_io::c_file cf(std::move(pf),fast_io::open_mode::out);
	fast_io::filebuf_file fbf(std::move(cf),fast_io::open_mode::out);
	std::ofstream fout;
	*fout.rdbuf()=std::move(*fbf.fb);
	fast_io::filebuf_io_observer fiob{fout.rdbuf()};
	fout<<"Hello World from std::ofstream\n";
	print(fiob,"Hello World from fast_io::filebuf_io_observer\n");
	fast_io::posix_tzset();
	auto unix_ts{fast_io::posix_clock_gettime(fast_io::posix_clock_id::realtime)};
	fast_io::native_l10n l10n(u8"");
	println(imbue(l10n,fast_io::c_stdout()),
	"Unix Timestamp:",unix_ts,"\n"
	"Universe Timestamp:",static_cast<fast_io::universe_timestamp>(unix_ts),"\n"
	"UTC iso8601:",utc(unix_ts),"\n",
	"UTC date_fmt:",fast_io::mnp::date_fmt(utc(unix_ts)),"\n"
	"Local iso8601:",local(unix_ts),"\n"
	"Local date_fmt:",fast_io::mnp::date_fmt(local(unix_ts)),"\n"
	"Local Timezone:",fast_io::timezone_name(),"\n"
#ifdef __clang__
	"LLVM clang " __clang_version__ "\n"
#elif defined(__GNUC__) && defined(__VERSION__)
	"GCC " __VERSION__ "\n"
#elif defined(_MSC_VER)
	"Microsoft Visual C++ ",_MSC_VER,"\n"
#else
	"Unknown C++ compiler\n"
#endif
#if defined(__GLIBC__)
	"GNU C Library ", __GLIBC__,".",__GLIBC_MINOR__, "\n"
#elif defined(__CYGWIN__) || defined(__NEWLIB__)
	"Newlib cygwin\n"
#elif defined(_WIN32) && !defined(__WINE__)
#if defined(_UCRT) || defined(_MSC_VER)
	"Universal CRT\n"
#elif defined(__MSVCRT_VERSION__)
	"MSVCRT ",fast_io::mnp::hex0x(__MSVCRT_VERSION__),"\n"
#endif
#endif
#if defined(_LIBCPP_VERSION)
	"LLVM libc++ ", _LIBCPP_VERSION, "\n"
#elif defined(__GLIBCXX__)
	"GNU C++ Library ",_GLIBCXX_RELEASE," ", __GLIBCXX__, "\n"
#elif defined(_MSVC_STL_UPDATE)
	"Microsoft Visual C++ STL ", _MSVC_STL_UPDATE, "\n"
#else
	"Unknown C++ standard library\n"
#endif
	"fstream.rdbuf():",fiob.fb,"\n"
	"FILE*:",static_cast<fast_io::c_io_observer>(fiob).fp,"\n"
	"fd:",static_cast<fast_io::posix_io_observer>(fiob).fd
#if (defined(_WIN32) && !defined(__WINE__)) || defined(__CYGWIN__)
	,"\n"
	"win32 HANDLE:",static_cast<fast_io::win32_io_observer>(fiob).handle
#ifndef _WIN32_WINDOWS
//NT kernel
	,"\n"
	"zw HANDLE:",static_cast<fast_io::zw_io_observer>(fiob).handle,"\n"
	"nt HANDLE:",static_cast<fast_io::nt_io_observer>(fiob).handle
#endif
#endif
);
}
/*
MinGW-w64 needs -lntdll
g++ -o construct_fstream_from_syscall construct_fstream_from_syscall.cc -Ofast -std=c++20 -s -flto -march=native -lntdll

9x kernel with clang might need
-Wl,--major-subsystem-version -Wl,4 -Wl,--minor-subsystem-version -Wl,0 -D_WIN32_WINDOWS=0x0400 -DWINVER=0x0400

D:\hg\fast_io\examples\0007.legacy>g++ -o construct_fstream_from_syscall_l10n construct_fstream_from_syscall_l10n.cc -Ofast -std=c++23 -s -flto -march=native -I../../include -lntdll

D:\hg\fast_io\examples\0007.legacy>construct_fstream_from_syscall_l10n
Unix Timestamp:1,663,581,555.4071499
Universe Timestamp:434,602,343,092,816,755.4071499
UTC iso8601:2022-09-19T09:59:15.4071499Z
UTC date_fmt:Mon Sep 19 09:59:15 AM +00:00 2022
Local iso8601:2022-09-19T05:59:15.4071499-04:00
Local date_fmt:Mon Sep 19 05:59:15 AM -04:00 2022
Local Timezone:Eastern Daylight Time
GCC 13.0.0 20220703 (experimental)
MSVCRT 0x700
GNU C++ Library 13 20,220,703
fstream.rdbuf():0x0,000,002,929,1ff,358
FILE*:0x0,000,7ff,92e,b5f,a90
fd:3
win32 HANDLE:0x0,000,000,000,000,0a8
zw HANDLE:0x0,000,000,000,000,0a8
nt HANDLE:0x0,000,000,000,000,0a8
*/
