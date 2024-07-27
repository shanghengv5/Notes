<!--
 * @Date: 2024-07-26 00:07:45
 * @LastEditors: LiShangHeng
 * @LastEditTime: 2024-07-27 18:43:45
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

## Implementing the TCP receiver
receive函数用于接受message对应信息，接受对应FLAG然后根据不同情况插入到reassembler中。
Receiver的send函数是告诉TCPSender已经接受了多少数据ackno以及window值和RST值是否需要断开连接。
根据tests的完善实现
```c++
#include "tcp_receiver.hh"

using namespace std;

void TCPReceiver::receive( TCPSenderMessage message )
{
  if ( message.SYN ) {
    SYN_ = message.SYN;
    ISN_ = message.seqno;
  }
  if ( message.RST ) {
    RST_ = message.RST;
    reassembler_.reader().set_error();
  }
  if ( SYN_ ) {
    message.seqno = message.seqno + message.SYN;
    uint64_t abs_seqno = message.seqno.unwrap( ISN_, reassembler().writer().bytes_pushed() );
    // SYN index
    if ( abs_seqno == 0 ) {
      reassembler_.insert( abs_seqno, "", message.FIN );
    } else {
      reassembler_.insert( abs_seqno - SYN_, message.payload, message.FIN );
    }
  }
}

TCPReceiverMessage TCPReceiver::send() const
{
  TCPReceiverMessage msg;
  Writer w = reassembler().writer();
  uint64_t ws = w.available_capacity() > UINT16_MAX ? UINT16_MAX : w.available_capacity();
  msg.window_size = (uint16_t)ws;
  msg.RST = RST_ || w.has_error() || reassembler().reader().has_error();
  msg.ackno = SYN_ ? ISN_ + SYN_ + w.bytes_pushed() + w.is_closed() : msg.ackno;
  return msg;
}
```