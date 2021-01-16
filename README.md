# ChanlunX

## 如何编译

通达信插件需要编译成32位，下面以Visual Studio 2019举例，作者用的是Visual Studio 2019社区版。

```cmd
mkdir build
cd build
cmake -G "Visual Studio 16 2019" -A Win32 ..
cmake --build .
```

## 缠论主图说明
- 1分钟图上画的是标准笔和标准线段
- 5分钟及以上图画的是简笔和用简笔计算的标准线段
