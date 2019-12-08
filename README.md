Bitcoin Core-Reading
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

Client启动时,如果本地之前存有可以访问的节点, 就通过本地的记录随机选择一些节点作为outbound peer. 否则会通过DNS Seed寻找一些可用的节点作为outbound peer.建立连接的过程是,Client首先向outbound peer发送本方Bitcoin Core的Version, 等待Server的回信. 

Server收到Version信息之后,向Client发送Server的Version信息,并且对Client的Version信息发送VERACK消息表示收到version信息, Client也会向Server发送VERACK消息, 收到VERACK之后,双方都会标记对方的连接状态fSuccessfullyConnected =  true.只有fSucessfullyConnected =  true之后,双方才会发送其他信息.

VERSION消息中更多的内容可以访问 [Bitcoin官网.](https://bitcoin.org/en/developer-reference#version )下面列出消息发送机制,假设刚开始客户端没有当前最新的区块,需要从outbound节点处同步区块.

Client和Server的fSuccessfullyConnected = true状态之后, 双方会有一些信息会按照一定的间隔时间进行发送,其中包括ping和Addr信息. ping信息每隔2分钟发送一次,互相发送Ping以检测对方是否离线. 第一次建立连接后双方就会互相发送Ping和Addr消息.

Addr用户互相交换已知的地址信息,Addr的时间间隔为30秒,在client 发送己方的version之后,随之就会向server发送Ping和Server, 而服务器也第一次向客户端发送信息时,也会发送Ping和Server信息.

发送完上述信息之后, 如果client是第一次启动,那么必然会和对方进行区块同步, 此外, 如果检查本地最长的区块的时间和server的时间差, 如果server的时间戳领先client本地最新区块的时间戳,并且领先时间不超过2小时,则进行区块同步.

区块同步的过程,即Client向Server发送GETHEADERS消息, 告知Server已有的最长的一个区块头的哈希值,Server收到后,如果有更多的区块头信息,则会发送给Client.

## ThreadOpenAddedConnections和ThreadOpenConnections
- 这两个函数中,看起来ThreadOpenConnections会执行ThreadOpenAddedConnections的功能,为什么还要额外初始化这个函数?
目前对 ThreadOpenAddedConnections 中功能并未深究.


## Bitcoin Core中的一些细节
### Bitcoin Core中检查一个区块头的合法性的步骤
- 检查本地是否已经存在该区块头,存在则丢弃
- 检查区块的工作量是否符合难度需求,如果不符合则丢弃
- 检查区块的难度值是否正确,不正确则丢弃
- 检查区块高度低于上一个检查点的高度,如果低于则丢弃
- 检查区块的时间戳,是否大于过去11个区块的平均时间戳,如果不大于则丢弃
- 检查区块的时间戳是否超过当前时间2小时,超过则丢弃
- 检查区块的版本和高度是否符合,不符合则丢弃
- 检查该区块是否指向一个非法的区块,如果是则丢弃
