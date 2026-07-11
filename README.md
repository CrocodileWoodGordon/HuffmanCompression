# Huffman Compression

基于 Huffman 编码的命令行文档压缩系统，使用 C++17 实现。

## 功能

- `Huffman -c <file>`：压缩单个文件，生成 `.huff`、`_freq.txt`、`_unzip.txt`。
- `Huffman -x <file>`：解压 `.huff` 文件，生成 `_unzip.txt`。
- `Huffman -a <dir>`：对目录下的 `1.txt`、`2.txt`、`3.txt` 执行自适应流式压缩。

程序按字节处理输入文件，因此英文、中文和二进制内容统一按 `0x00` 到 `0xFF` 统计。

## 构建

### g++

```bash
g++ -std=c++17 -O2 -Wall -Wextra -pedantic src/*.cpp -o Huffman
```

### CMake

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
cp build/Huffman ./Huffman
```

### xmake

```bash
xmake
cp build/linux/*/release/Huffman ./Huffman
```

不同平台的 xmake 输出目录可能略有差异，可用 `xmake run Huffman` 或 `xmake run Huffman -- -c <file>` 直接运行。

## 使用示例

### 普通压缩

```bash
./Huffman -c resources/个人项目-基于Huffman编码的压缩/其他附件/hamlet.txt
```

生成：

```text
hamlet.huff
hamlet_freq.txt
hamlet_unzip.txt
```

### 单独解压

```bash
./Huffman -x resources/个人项目-基于Huffman编码的压缩/其他附件/hamlet.huff
```

生成：

```text
hamlet_unzip.txt
```

### 自适应流式压缩

```bash
./Huffman -a resources/个人项目-基于Huffman编码的压缩/其他附件
```

依次处理目录下的 `1.txt`、`2.txt`、`3.txt`，生成：

```text
1_adp.huff
1_freq.txt
1_unzip.txt
2_adp.huff
2_freq.txt
2_unzip.txt
3_adp.huff
3_freq.txt
3_unzip.txt
```

自适应模式中，`2_freq.txt` 使用 `1.txt + 2.txt` 的累积频率，`3_freq.txt` 使用 `1.txt + 2.txt + 3.txt` 的累积频率。

## 验证

压缩命令会在生成 `.huff` 后立刻解压并输出 `_unzip.txt`，同时使用两种自定义 64 位哈希验证原文件和解压文件一致。

也可以手动使用 `diff` 验证：

```bash
diff resources/个人项目-基于Huffman编码的压缩/其他附件/hamlet.txt \
     resources/个人项目-基于Huffman编码的压缩/其他附件/hamlet_unzip.txt
```

自定义哈希实现位于 `src/hash.cpp`，采用 `resources/Hash.cpp` 中的 `splitmix64` 和双通道混合思路，并改为适合文件字节序列的确定性实现。

## 项目结构

- `src/`：全部 C++17 源码。
- `AGENTS.md`：面向开发代理的模块职责和调用关系说明。
- `REQUIREMENTS.md`：课程项目要求整理。
- `resources/`：课程下发资料和测试文本。
- `.github/workflows/build.yml`：GitHub Actions 构建配置。
