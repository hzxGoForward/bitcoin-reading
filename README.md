Bitcoin Core-reading
=====================================

- 这个Repository没有对Bitcoin Core的逻辑进行修改,而是本人对Bitcoin Core运行过程源代码的阅读理解
- 里面关于本人对代码的二次注释,每次的注释前都会加上 hzx 标记.
- 这里的版本号为209, 对应的版本Bitcoin Core的版本为70015.


## 尚未清楚的函数

### **SocketEvents**

- 在net.cpp文件中SocketHandler函数中调用了这个函数,这个函数的作用是什么,我的猜测是,查看相关的socket是否有数据的发送和接收,如果有,则防止到相应的Socket中, 后续进行数据的接收和发送工作.

### **CNode**
- CNode中addrFrom

### AddTimeData
- 在net_processing.cpp中的 AddTimeData(pfrom->addr, nTimeOffset);这个函数用来作甚?


## CNode 中的fOneShot
- fOneShot的含义,我至今尚不清楚其到底是什么意思

## Bitcoin中信息发送

假设本地是客户端C, outbound连接peer作为服务器Server,C->S表示客户端向服务器发送信息, S->C表示服务器向客户端发送信息,NetMsgType表示消息的枚举类型.
VERSION消息中更多的内容可以访问 [Bitcoin官网.](https://bitcoin.org/en/developer-reference#version )下面列出消息发送机制,假设刚开始客户端没有当前最新的区块,需要从outbound节点处同步区块.

* C->S: NetMsgType::VERSION, 客户端向服务器发送本机版本号以及其他信息
* S->C: NetMsgType::VERSION, 服务器向客户端发送服务器版本号
* S->C: NetMsgType::VERACK,  服务器向客户端发送版本号确认关系
* S->C: NetMsgType::GETADDR, 服务器向客户端请求客户端告知客户端知道的地址.(前提是,客户端的版本大于CADDR_TIME_VERSION(31402) && 本地已知的地址数量<1000 && 客户端是fOneShot节点)
* S->C: NetMsgType::PING,    服务器向客户端发送ping信息测试延迟.
* C->S: NetMsgType::VERACK,  客户端向服务器发送版本号的确认信息
* S->C:NetMsgType::SENDHEADER_VERSION, 服务器告知客户端, 客户端广播区块的时候,直接使用区块头进行广播, 发送这个信息的前提是, 客户端的版本不小于SENDHEADERS_VERSION(70012)
* S->C:NetMsgType::SENDCMPCT, 服务器向客户端发送信息,告知客户端服务器支持发送压缩区块,但是前提是,客户端的版本不小于SHORT_IDS_BLOCKS_VERSION(70014)
* 

## ThreadOpenAddedConnections和ThreadOpenConnections
- 这两个函数中,看起来ThreadOpenConnections会执行ThreadOpenAddedConnections的功能,为什么还要额外初始化这个函数?
目前对 ThreadOpenAddedConnections 中功能并未深究.


