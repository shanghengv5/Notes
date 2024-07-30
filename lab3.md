<!--
 * @Date: 2024-07-30 23:30:37
 * @LastEditors: LiShangHeng
 * @LastEditTime: 2024-07-30 23:31:40
 * @FilePath: /CS144/lab3.md
-->

# Checkpoint 3: the TCP sender

有了之前receiver的经验，实际实现sender更加的容易，课件有了所有满足的条件，这一章也没有很大的难度。
完整的阅读课件，并且通过测试用例即可。

```c++
#include "tcp_sender.hh"
#include "tcp_config.hh"
#include <iostream>

using namespace std;

uint64_t TCPSender::sequence_numbers_in_flight() const
{
  // cout << "sequence_numbers_in_flight:" << ";abs_seqnos_:" << abs_seqnos_ << "; ackno:" << ackno_ << endl;
  return ackno_ - abs_seqnos_;
}

uint64_t TCPSender::consecutive_retransmissions() const
{
  return consecutive_retransmissions_;
}

void TCPSender::push( const TransmitFunction& transmit )
{
  do {
    TCPSenderMessage msg = make_empty_message();
    uint64_t len {};
    if ( window_size_ > sequence_numbers_in_flight() ) {
      len = window_size_ - sequence_numbers_in_flight() > TCPConfig::MAX_PAYLOAD_SIZE
              ? TCPConfig::MAX_PAYLOAD_SIZE
              : window_size_ - sequence_numbers_in_flight();
    } else if ( window_size_ == 0 && sequence_numbers_in_flight() == 0 ) {
      len = 1;
    }
    read( input_.reader(), len, msg.payload );
    if ( input_.reader().is_finished() && window_size_ > msg.sequence_length() + sequence_numbers_in_flight() ) {
      msg.FIN = true;
    } else if ( input_.reader().is_finished() && window_size_ == 0 && msg.sequence_length() == 0
                && sequence_numbers_in_flight() == 0 && len == 1 ) {
      msg.FIN = true;
    }
    // cout << "window_size_:" << window_size_ << ";abs_seqnos_:" << abs_seqnos_ << "; ackno:" << ackno_
    //      << "; msg.Fin:" << msg.FIN << "; msg.sequence_length():" << msg.sequence_length()
    //      << "; sequence_numbers_in_flight:" << sequence_numbers_in_flight() << "; len:" << len << endl;
    if ( msg.sequence_length() > 0 && FIN_ == false ) {
      ackno_ += msg.sequence_length();
      if ( msg.FIN ) {
        FIN_ = true;
      }
      transmit( msg );
      // insert in outstanding_
      outstanding_.insert( pair<uint64_t, TCPSenderMessage>( ackno_, msg ) );
    } else {
      break;
    }
  } while ( window_size_ > 0 );
}

TCPSenderMessage TCPSender::make_empty_message() const
{
  TCPSenderMessage msg;
  msg.seqno = isn_.wrap( ackno_, isn_ );
  if ( ackno_ == 0 ) {
    msg.SYN = true;
  }
  if ( writer().has_error() || reader().has_error() ) {
    msg.RST = true;
  }
  return msg;
}

void TCPSender::receive( const TCPReceiverMessage& msg )
{
  uint64_t msg_abs_seqnos = msg.ackno.has_value() ? msg.ackno.value().unwrap( isn_, abs_seqnos_ ) : 0;
  // cout << "msg_abs_seqnos:" << msg_abs_seqnos << ";abs_seqnos_:" << abs_seqnos_ << "; ackno:" << ackno_ << endl;
  if ( msg_abs_seqnos > abs_seqnos_ && msg_abs_seqnos <= ackno_ ) {
    abs_seqnos_ = msg_abs_seqnos;
    RTO_ms_ = initial_RTO_ms_;
    consecutive_retransmissions_ = 0;
    retransmission_timer_ = alive_ms_;
  }
  window_size_ = msg.window_size;
  // remove outstanding_
  for ( map<uint64_t, TCPSenderMessage>::iterator it = outstanding_.begin(); it != outstanding_.end();
        it = outstanding_.erase( it ) ) {
    if ( it->first > msg_abs_seqnos ) {
      break;
    }
  }
  if ( msg.RST )
    writer().set_error();
}

void TCPSender::tick( uint64_t ms_since_last_tick, const TransmitFunction& transmit )
{
  alive_ms_ += ms_since_last_tick;
  // cout << "retransmission_timer_:" << retransmission_timer_ << ";initial_RTO_ms_:" << initial_RTO_ms_
  //  << "; RTO_ms_:" << RTO_ms_ << "; alive_ms_:" << alive_ms_ << endl;
  if ( retransmission_timer_ + RTO_ms_ <= alive_ms_ ) {
    if ( consecutive_retransmissions_ > TCPConfig::MAX_RETX_ATTEMPTS ) {
      // aborted
      input_.set_error();
    }
    if ( outstanding_.size() > 0 ) {
      TCPSenderMessage msg = outstanding_.begin()->second;
      transmit( msg );
      consecutive_retransmissions_++;
    }
    if ( window_size_ > 0 ) {
      RTO_ms_ *= 2;
    }
    retransmission_timer_ = alive_ms_;
  }
}

```