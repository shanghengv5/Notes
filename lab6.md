<!--
 * @Date: 2024-08-09 00:21:28
 * @LastEditors: LiShangHeng
 * @LastEditTime: 2024-08-09 00:21:30
 * @FilePath: /CS144/lab6.md
-->
# Lab Checkpoint 6: building an IP router
很高兴到第六讲，工作了，每天只有一个小时到两个小时做lab，但是效率意外的更高了，热情也高涨了。
这一讲我们将构建一个ip路由。

## 闲谈
完成已经是一个月以后了，果然工作后时间变少了，精力也少了。

### 重点
这次被卡了很久，主要原因是因为不熟route的机制，第二个原因就是lab5通过后居然一直影响着lab6.
因为在lab5用的保存dagrams_send_是map，实际应该用mutimap,因为一个ip会对应多个数据报。<br>
但是test就轻松通过了，找了很久才解决。
第二就是lab6无法接受到任何数据包，原因是我将arp的dagrams_send_放在了错误的位置去push队列，导致很多旧的数据包不会发送。但是test也是通过了，真是大坑。

### 结果
最后还是通过了lab6，这次参考了其他人的代码，就不重复贴代码了，这里主要写写遇到的坑。
