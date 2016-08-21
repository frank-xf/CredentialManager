#ifndef _bnb_Global_H_
#define _bnb_Global_H_

#define _bnb_space_begin_   namespace bnb {
#define _bnb_space_end_     }
#define _bnb                ::bnb::

namespace std
{
    template<typename _Ty> class allocator;
    template<typename _Ty> struct char_traits;
    template<typename _Ty, typename _Traits, typename _Alloc> class basic_string;
}

_bnb_space_begin_

using char_t = char;
using byte_t = unsigned char;
using size_t = unsigned int;

using string_type = std::basic_string<char_t, std::char_traits<char_t>, std::allocator<char_t>>;
using memory_type = std::basic_string<byte_t, std::char_traits<byte_t>, std::allocator<byte_t>>;

_bnb_space_end_

#endif // _bnb_Global_H_
