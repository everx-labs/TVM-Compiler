#ifndef SHA256_LITERAL_H
#define SHA256_LITERAL_H

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <array>

namespace sha256_literal {

static constexpr uint32_t SHA256_K[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
  0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
  0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
  0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
  0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
  0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
  0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
  0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
  0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

using StateType = std::array<uint32_t, 8>;
using BlockType = std::array<uint32_t, 16>;
using WType     = std::array<uint32_t, 64>;
using HashType  = std::array<uint8_t, 4 * 8>;

static constexpr unsigned BlockTypeSize = 4 * 16;

namespace details {

template<class F, class T, size_t N, class... Args>
static constexpr auto map_zip(F const f, std::array<T, N> const data, Args const... Arrays)
{
  std::array<decltype(f(data[0], Arrays[0]...)), N> out = {0};
  auto* itOut = &std::get<0>(out);
  for(size_t i = 0; i < N; ++i)
    itOut[i] = f(data[i], Arrays[i]...);
  return out;
}

template<class F, class T, size_t N, class... Args>
static constexpr auto map(F const f, std::array<T, N> const data, Args const... args)
{
  using RetType = decltype(f(std::declval<T>(), args...));
  std::array<RetType, N> out = {0};
  auto* itOut = &std::get<0>(out);
  for(size_t i = 0; i < N; ++i)
    itOut[i] = f(data[i], args...);
  return out;
}

template<class F, class T, class... Args>
static constexpr auto map(F const f, std::array<T, 0> const, Args const... args)
{
  using RetType = decltype(f(std::declval<T>(), args...));
  return std::array<RetType, 0>{};
}

// BlockType constexpr helpers

static constexpr uint32_t xor_(uint32_t a, uint32_t b) { return a^b; }
static constexpr BlockType blocktype_xor(BlockType const A, uint8_t const B)
{
  const uint32_t B32  = (uint32_t)B;
  const uint32_t B32x4 = B32|(B32<<8)|(B32<<16)|(B32<<24);
  return map(xor_, A, B32x4);
}

static constexpr uint32_t u8x4_to_be_u32(uint8_t const a, uint8_t const b, uint8_t const c, uint8_t const d)
{
  return ((uint32_t)d)|(((uint32_t)c)<<8)|(((uint32_t)b)<<16)|(((uint32_t)a)<<24);
}

// SHA256 routines
// Based on code from https://github.com/thomdixon/pysha2/blob/master/sha2/sha256.py

static constexpr uint32_t rotr(uint32_t const v, int off)
{
  return (v >> off) | (v << (32-off));
}

static constexpr uint32_t sum(uint32_t const a, uint32_t const b) { return a+b; }

static constexpr StateType transform(StateType const S, BlockType const data)
{
  WType W = {0};
  auto* ItW = &std::get<0>(W);
  auto const* CItW = &std::get<0>(W);
  for (size_t i = 0; i < data.size(); ++i) {
    ItW[i] = data[i];
  }

  for (size_t i = 16; i < 64; ++i) {
      const uint32_t s0 = rotr(CItW[i-15], 7) ^ rotr(CItW[i-15], 18) ^ (CItW[i-15] >> 3);
      const uint32_t s1 = rotr(CItW[i-2], 17) ^ rotr(CItW[i-2], 19)  ^ (CItW[i-2] >> 10);
      ItW[i] = (CItW[i-16] + s0 + CItW[i-7] + s1);
  }

  StateType InS = S;
  auto const* CInS = &std::get<0>(InS);
  for (size_t i = 0; i < 64; ++i) {
    const uint32_t s0 = rotr(CInS[0], 2) ^ rotr(CInS[0], 13) ^ rotr(CInS[0], 22);
    const uint32_t maj = (CInS[0] & CInS[1]) ^ (CInS[0] & CInS[2]) ^ (CInS[1] & CInS[2]);
    const uint32_t t2 = s0 + maj;
    const uint32_t s1 = rotr(CInS[4], 6) ^ rotr(CInS[4], 11) ^ rotr(CInS[4], 25);
    const uint32_t ch = (CInS[4] & CInS[5]) ^ ((~CInS[4]) & CInS[6]);
    const uint32_t t1 = CInS[7] + s1 + ch + SHA256_K[i] + CItW[i];

    InS = {
      t1 + t2,
      CInS[0],
      CInS[1],
      CInS[2],
      CInS[3] + t1,
      CInS[4],
      CInS[5],
      CInS[6]
    };
  }

  return map_zip(sum, S, InS);
}

static auto constexpr u8_to_block(uint8_t const* It)
{
  BlockType B = {0};
  auto* ItB = &std::get<0>(B);
  for (size_t i = 0; i < std::tuple_size<BlockType>(); i++) {
    ItB[i] = u8x4_to_be_u32(
        It[i*4],
        It[i*4+1],
        It[i*4+2],
        It[i*4+3]
      );
  }
  return B;
}

template <uint64_t BlockCount, typename Ar>
constexpr std::enable_if_t<BlockCount != 0, std::array<BlockType, BlockCount>>
u8_to_blocks_(Ar const Data)
{
  std::array<BlockType, BlockCount> Ret = { {0} };
  auto* ItRet = &std::get<0>(Ret);
  for (uint64_t i = 0; i < BlockCount; ++i) {
    ItRet[i] = u8_to_block(&Data[i*BlockTypeSize]);
  }
  return Ret;
}

template <uint64_t BlockCount, typename Ar>
constexpr std::enable_if_t<BlockCount == 0, std::array<BlockType, 0>>
u8_to_blocks_(Ar const __attribute__((unused)) Data)
{
  return std::array<BlockType, 0>{};
}

template <size_t Len_>
constexpr auto u8_to_blocks(std::array<uint8_t, Len_> const Data)
{
  constexpr uint64_t Len = Len_;
  constexpr uint64_t BlockCount = Len/BlockTypeSize;
  return u8_to_blocks_<BlockCount>(Data);
}

static constexpr HashType state_to_hash(StateType const S)
{
  HashType Ret = {0};
  auto* ItRet = &std::get<0>(Ret);
  for (size_t i = 0; i < std::tuple_size<StateType>(); ++i) {
    uint32_t const V = S[i];
    ItRet[i*4]   = (V>>24) & 0xff;
    ItRet[i*4+1] = (V>>16) & 0xff;
    ItRet[i*4+2] = (V>>8)  & 0xff;
    ItRet[i*4+3] = (V)     & 0xff;
  }
  return Ret;
}

template< class InputIt, class OutputIt >
static constexpr OutputIt constexpr_copy(InputIt first, InputIt last, OutputIt d_first)
{
  for (; first != last; ++first) {
    *d_first = *first;
    ++d_first;
  }
  return d_first;
}

template <class T, size_t N>
constexpr auto* get_array_it(std::array<T, N> const& Data)
{
  return &std::get<0>(Data);
}

template <class T>
constexpr T* get_array_it(std::array<T, 0> const&)
{
  // Do not use nullptr here, because it is of type "nullptr_t", and this will
  // give erros for the pointer arithmetics done in sha256.
  // This would be much easier with "if constexpr" in C++17!
  return 0;
}

template <size_t Len_>
static constexpr HashType sha256(std::array<uint8_t, Len_> const Data)
{
  constexpr StateType StateOrg = {0x6a09e667, 0xbb67ae85, 0x3c6ef372,
    0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};

  StateType State = StateOrg;
  constexpr uint64_t Len = Len_;
  constexpr uint64_t BlockCount = Len/BlockTypeSize;

  auto const Blocks = u8_to_blocks(Data);

  for (size_t i = 0; i < Blocks.size(); ++i) {
    State = transform(State, Blocks[i]);
  }

  struct {
    uint8_t Data[64];
  } LastB = {0};

  auto* const ItLastBBegin = &LastB.Data[0];
  auto* ItLastB = ItLastBBegin;

  auto* ItData = get_array_it(Data);
  if (ItData != 0) {
    ItLastB = constexpr_copy(
        ItData + BlockCount*BlockTypeSize,
        ItData + Len,
        ItLastB);
  }
  *ItLastB = 0x80;

  constexpr uint64_t Rem = Len-BlockCount*BlockTypeSize;
  if (Rem >= 56) {
    BlockType const LastB_ = u8_to_block(ItLastBBegin);
    State = transform(State, LastB_);
    LastB = {0};
  }
  constexpr uint64_t Len3 = Len << 3;
  for (size_t i = 0; i < 8; ++i) {
    LastB.Data[56+i] = (Len3 >> (56-(i*8))) & 0xff;
  }
  BlockType const LastB_ = u8_to_block(ItLastBBegin);
  State = transform(State, LastB_);

  return state_to_hash(State);
}

static constexpr uint8_t char_to_u8(char const v) { return v; }

template <size_t N, typename T, size_t N_, size_t... I>
static constexpr auto get_array(T const (&Data)[N_], std::index_sequence<I...>)
{
  return std::array<T, N>{Data[I]...};
}

template <size_t N, typename T, size_t N_>
static constexpr auto get_array(T const (&Data)[N_])
{
  return get_array<N>(Data, std::make_index_sequence<N>());
}

template <typename T, size_t N>
static constexpr auto get_array(T const (&Data)[N])
{
  return get_array<N>(Data);
}

} // details

template <size_t N>
static constexpr auto compute(std::array<uint8_t, N> const Data)
{
  return details::sha256(Data);
}

template <size_t N>
static constexpr auto compute(char const (&Data)[N])
{
  auto const Ar = details::get_array(Data);
  return details::sha256(details::map(details::char_to_u8, Ar));
}

template <size_t N>
static constexpr auto compute_str(char const (&Data)[N])
{
  auto const Ar = details::get_array<N-1>(Data);
  return details::sha256(details::map(details::char_to_u8, Ar));
}

} // sha256_literal

template <typename CharT, CharT... Cs>
static constexpr auto operator "" _sha256()
{
  static_assert(std::is_same<CharT, char>::value, "only support 8-bit strings");
  const char Data[] = {Cs...};
  return sha256_literal::compute(Data);
}

// TVM local begin
template <typename CharT, CharT... Cs>
static constexpr unsigned operator "" _func_id()
{
  static_assert(std::is_same<CharT, char>::value, "only support 8-bit strings");
  constexpr char Data[] = {Cs...};
  constexpr auto hash = sha256_literal::compute(Data);
  return ((hash[0] & 0x7f) << (3*8)) | (hash[1] << (2*8)) | (hash[2] << (8)) | (hash[3]);
}
// TVM local end

#endif
