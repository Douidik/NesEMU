#ifndef CPU_BITWISE_HPP
#define CPU_BITWISE_HPP

#define NEGATIVE_BIT (0x80)

#define CONCATENATE_WORDS(hi, lo) ((hi << 8) | lo)
#define SET_NEGATIVE_FLAG(x) (m_Status.N = (x)&NEGATIVE_BIT)
#define SET_ZERO_FLAG(x) (m_Status.Z = (x) == 0)

#endif
