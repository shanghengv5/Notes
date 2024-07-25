<!--
 * @Date: 2024-07-26 00:07:45
 * @LastEditors: LiShangHeng
 * @LastEditTime: 2024-07-26 00:13:19
 * @FilePath: /CS144/lab2.md
-->

# Checkpoint 2: the TCP receiver

## Translating between 64-bit indexes and 32-bit seqnos

1.需要实现一个32位的wrap，tcp却要接受任意长度的数据。2的32次方是4GB，当序号到2的32次方-1的时候，下一字节就会重置为0.<br>
2.TCP会开始于一个随机值，也被称为Initial Sequence Number(ISN)，代表流的开始。<br>
3.逻辑上的开始和结束会占领两个序列号，SYN和FIN这两个控制标值。他们不是字节数据，只代表开始和结束。

![alt text](image-6.png)

![alt text](image-7.png)
参考这里，注意checkpoint是个近似值，要猜测靠近他的情况。

```c++
#include "wrapping_integers.hh"

using namespace std;

Wrap32 Wrap32::wrap( uint64_t n, Wrap32 zero_point )
{
  // 1 line will be wise.
  return Wrap32 { zero_point + n };
}

uint64_t Wrap32::unwrap( Wrap32 zero_point, uint64_t checkpoint ) const
{
  Wrap32 diff( raw_value_ - zero_point.raw_value_ );
  uint64_t seqnos_diff = diff.raw_value_;
  uint64_t half_uint32 = 1UL << 31;
  if ( checkpoint > seqnos_diff ) {
    seqnos_diff += ( ( checkpoint - seqnos_diff + half_uint32 ) / ( 1UL << 32 ) ) * ( 1UL << 32 );
  }

  return seqnos_diff;
}
```