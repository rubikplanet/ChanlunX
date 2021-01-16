# ChanlunX

## 如何编译

### Visual Studio 2019

通达信插件需要编译成32位，下面以Visual Studio 2019举例，作者用的是Visual Studio 2019社区版。

```cmd
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A Win32 ..
cmake --build . --config Release
```

### Visual Studio 2015 或者 Visual Studio 2017

在项目根目录下依次执行以下命令

```cmd
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## 交流

很长一段时间没有维护这个项目，现在重新建立QQ群方便大家交流，但是入群设置了门槛，略微收取入群费用作为项目的持续维护，工具本身会提供给大家免费使用。有意向加群的可以通过下面的方式先联系作者。

- WeChat: kldcty
- QQ: 1106628276 9394908
- 微信公众号: zeroquant

## 效果图

![](效果图.png)