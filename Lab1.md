<!--
 * @Date: 2024-07-23 09:59:32
 * @LastEditors: LiShangHeng
 * @LastEditTime: 2024-07-23 10:07:11
 * @FilePath: /CS144/Lab1.md
-->
# Lab Checkpoint 1: stitching substrings into a byte stream

## 要点
pdf涉及的都有了，主要要按照老师给的test去测试。

* 注意超出边界的不管是小于已有排序的，还是大于未排序加剩余空间的都要舍去。
* 注意内存释放的问题，这个因为我用的map在speed test我花了一些时间解决这个问题。

以下是代码<br>
reassembler.hh
```c++ 
private:
  ByteStream output_; // the Reassembler writes to this ByteStream
  std::map<uint64_t, std::string> reassembler_ {};
  uint64_t last_string_index_ = 0;
  bool last_string_flag_ {};

  // How many bytes are pushed(ordered) in the Reassembler itself?
  uint64_t bytes_pushed() const;
```

reassembler.cc
```c++
#include "reassembler.hh"
#include <iostream>
using namespace std;

void Reassembler::insert( uint64_t first_index, string data, bool is_last_substring )
{
  size_t cap = output_.writer().available_capacity();
  // The left bound
  size_t overSize = first_index + data.size() - ( bytes_pushed() + cap );
  // set last string index
  if ( is_last_substring ) {
    last_string_index_ = first_index + data.size();
    last_string_flag_ = true;
  }
  // Bigger than capacity
  if ( first_index >= bytes_pushed() + cap || cap == 0 ) {
    return;
  }
  // If index is same and size is bigger than older, don't update
  if ( ( reassembler_.find( first_index ) != reassembler_.end()
         && reassembler_[first_index].size() >= data.substr( 0, data.size() - overSize ).size() )
       == false ) {
    reassembler_[first_index] = data.substr( 0, data.size() - overSize );
  }

  // Process order part and push
  for ( map<uint64_t, string>::iterator it = reassembler_.begin(); it != reassembler_.end();
        it = reassembler_.erase( it ) ) {
    // unorder part break
    if ( it->first > bytes_pushed() ) {
      break;
    }
    // order part push
    if ( bytes_pushed() - it->first < it->second.size() ) {
      string insert_str = it->second.substr( bytes_pushed() - it->first );
      output_.writer().push( insert_str );
    }
    // remove all key before ordered index
    // If erase key, will return map size
    // earse iterator will return next iterator
  }
  // is last to close
  if ( last_string_flag_ && ( bytes_pushed() >= last_string_index_ ) ) {
    output_.writer().close();
  }
}

uint64_t Reassembler::bytes_pending() const
{
  map<uint64_t, bool> pendings;
  for ( const auto& pair : reassembler_ ) {
    for ( size_t start = 0; start < pair.second.size(); start++ ) {
      pendings[start + pair.first] = true;
    }
  }
  return pendings.size();
}

uint64_t Reassembler::bytes_pushed() const
{
  return output_.writer().bytes_pushed();
}
```

最后结果
![alt text](image-5.png)