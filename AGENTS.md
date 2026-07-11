# Agents 项目协作说明

## 项目目标

本仓库实现“基于 Huffman 编码的文档压缩系统”课程项目，要求见 `REQUIREMENTS.md`。

项目使用 C++17，实现命令行可执行文件：

- Linux/macOS：`Huffman`
- Windows：`Huffman.exe`

必须支持：

- `Huffman -c <file>`：压缩文件，并生成 `.huff`、`_freq.txt`、`_unzip.txt`。
- `Huffman -x <file>`：解压 `.huff` 文件，并生成 `_unzip.txt`。
- `Huffman -a <dir>`：按 `1.txt`、`2.txt`、`3.txt` 执行自适应流式压缩。

## 仓库结构

```text
.
├── AGENTS.md
├── README.md
├── REQUIREMENTS.md
├── CMakeLists.txt
├── xmake.lua
├── .gitignore
├── .github/
│   └── workflows/
│       └── build.yml
├── resources/
│   └── 个人项目-基于Huffman编码的压缩/
└── src/
    ├── main.cpp
    ├── cli.hpp
    ├── cli.cpp
    ├── path_utils.hpp
    ├── path_utils.cpp
    ├── file_io.hpp
    ├── file_io.cpp
    ├── frequency.hpp
    ├── frequency.cpp
    ├── huffman_tree.hpp
    ├── huffman_tree.cpp
    ├── code_table.hpp
    ├── code_table.cpp
    ├── bit_stream.hpp
    ├── bit_stream.cpp
    ├── archive_format.hpp
    ├── archive_format.cpp
    ├── compressor.hpp
    ├── compressor.cpp
    ├── decompressor.hpp
    ├── decompressor.cpp
    ├── adaptive.hpp
    ├── adaptive.cpp
    ├── hash.hpp
    ├── hash.cpp
    ├── metrics.hpp
    └── metrics.cpp
```

约束：

- 所有 C++ 源码和头文件都放入 `src/`。
- 构建产物、临时文件、可执行文件不提交。
- `resources/` 是课程下发资料，除非用户明确要求，不修改。
- `REQUIREMENTS.md` 是课程要求整理文档，除非用户明确要求，不修改。

## 模块职责

### `main.cpp`

程序入口，只做最薄的一层控制：

1. 接收 `argc/argv`。
2. 调用 `cli` 解析命令。
3. 根据解析结果分发到压缩、解压或自适应流程。
4. 捕获顶层异常并输出用户可读错误。

### `cli.hpp` / `cli.cpp`

负责命令行接口：

- 识别 `-c`、`-x`、`-a` 三种模式。
- 校验参数数量。
- 输出帮助信息。
- 将命令转换为内部 `Command` 数据结构。

不负责文件读写、压缩算法或路径命名细节。

### `path_utils.hpp` / `path_utils.cpp`

负责所有输出路径和文件名规则：

- 普通压缩输出：
  - `<stem>.huff`
  - `<stem>_freq.txt`
  - `<stem>_unzip.txt`
- 单独解压输出：
  - `<stem>_unzip.txt`
- 自适应输出：
  - `1_adp.huff`、`1_freq.txt`、`1_unzip.txt`
  - `2_adp.huff`、`2_freq.txt`、`2_unzip.txt`
  - `3_adp.huff`、`3_freq.txt`、`3_unzip.txt`

所有输出必须位于输入文件或输入目录的同一目录下。

### `file_io.hpp` / `file_io.cpp`

负责二进制文件读写：

- 以二进制方式读取任意文件为字节序列。
- 以二进制方式写出压缩文件和解压文件。
- 以文本方式写出 `_freq.txt`。
- 屏蔽平台换行和编码差异。

不做 Huffman 编码逻辑。

### `frequency.hpp` / `frequency.cpp`

负责字节频率统计和频率表输出数据准备：

- 使用 256 个槽位统计 `0x00` 到 `0xFF`。
- 支持单文件频率。
- 支持自适应模式的累积频率。
- 提供按“次数降序、字节值升序”的排序结果。
- 判断字符列是否显示原字符或 `.`。

### `huffman_tree.hpp` / `huffman_tree.cpp`

负责 Huffman 树构建：

- 基于频率表创建叶子节点。
- 使用优先队列或小根堆合并节点。
- 保证构建流程满足课程对堆优化的要求。
- 提供根节点给编码表生成和解码过程使用。

不负责 bit 级读写和文件格式。

### `code_table.hpp` / `code_table.cpp`

负责从 Huffman 树生成编码表：

- 遍历 Huffman 树。
- 为每个出现过的字节生成 `0/1` 编码。
- 处理只有一种字节的边界情况。
- 提供平均码长计算所需的编码长度。

### `bit_stream.hpp` / `bit_stream.cpp`

负责 bit 级输入输出：

- 将 Huffman 编码 bit 串打包为字节。
- 读取压缩文件中的 bit 流。
- 处理最后一个字节的有效 bit 数。

不保存频率表，不理解 Huffman 树结构。

### `archive_format.hpp` / `archive_format.cpp`

负责 `.huff` 文件格式：

- 写入可识别的文件头。
- 写入原始文件大小。
- 写入 256 项频率表或等价的树重建信息。
- 写入压缩 bit 数据。
- 读取 `.huff` 并恢复解压所需元数据。

要求 `.huff` 能独立解压，不依赖原始文件。

### `compressor.hpp` / `compressor.cpp`

负责普通压缩主流程：

1. 读取输入文件。
2. 统计频率。
3. 构建 Huffman 树。
4. 生成编码表。
5. 写 `.huff`。
6. 写 `_freq.txt`。
7. 立即调用解压流程生成 `_unzip.txt` 验证文件。

### `decompressor.hpp` / `decompressor.cpp`

负责解压流程：

1. 读取 `.huff`。
2. 解析文件头和频率信息。
3. 重建 Huffman 树。
4. 根据 bit 流恢复原始字节。
5. 写 `_unzip.txt`。

### `adaptive.hpp` / `adaptive.cpp`

负责自适应流式压缩：

1. 接收目录路径。
2. 依次处理 `1.txt`、`2.txt`、`3.txt`。
3. 每一阶段更新累积频率。
4. 用累积频率构建 Huffman 树压缩当前文件。
5. 生成当前阶段的 `_adp.huff`、`_freq.txt`、`_unzip.txt`。

自适应模式下 `_freq.txt` 必须写累积频率。

### `hash.hpp` / `hash.cpp`

负责自定义哈希：

- 提供两种不同的自实现文件哈希算法。
- 用于比较原文件和 `_unzip.txt`。
- 不使用 `std::hash` 充当课程要求的自定义哈希。

### `metrics.hpp` / `metrics.cpp`

负责报告所需统计指标：

- 计算 Shannon 信息熵。
- 计算 Huffman 平均码长。
- 计算编码效率。
- 可辅助输出压缩率和耗时统计。

## 调用关系

普通压缩 `Huffman -c <file>`：

```text
main
└── cli
    └── compressor
        ├── path_utils
        ├── file_io
        ├── frequency
        ├── huffman_tree
        ├── code_table
        ├── bit_stream
        ├── archive_format
        ├── decompressor
        ├── hash
        └── metrics
```

单独解压 `Huffman -x <file>`：

```text
main
└── cli
    └── decompressor
        ├── path_utils
        ├── file_io
        ├── archive_format
        ├── huffman_tree
        └── bit_stream
```

自适应流式压缩 `Huffman -a <dir>`：

```text
main
└── cli
    └── adaptive
        ├── path_utils
        ├── file_io
        ├── frequency
        ├── huffman_tree
        ├── code_table
        ├── bit_stream
        ├── archive_format
        ├── decompressor
        ├── hash
        └── metrics
```

## 构建要求

后续实现完成后必须支持以下构建方式。

### g++

`README.md` 中需要给出等价命令，目标是生成根目录下的 `Huffman`。

### CMake

根目录提供 `CMakeLists.txt`，使用 C++17，目标名为 `Huffman`。

### xmake

根目录提供 `xmake.lua`，使用 C++17，目标名为 `Huffman`。

### GitHub Actions

`.github/workflows/build.yml` 负责在 push 和 pull request 时构建项目，并上传可执行文件 artifact。

## Git 与提交规则

- 本地开发时不要提交编译产物、临时目录、可执行文件和测试输出。
- 每次完成文件改动后，使用 `git add` 跟踪改动文件。
- 每个小步骤完成后，使用 `git commit -m "<完成任务内容>"` 提交。
- 如果用户明确要求不要提交，则以用户要求为准。

## 当前初始化状态

本阶段只初始化项目骨架：

- 创建源码文件和构建配置文件。
- 源码文件之间的职责和调用关系已在本文档中设计。
- 暂不写 C++ 实现代码。
- 后续实现应严格按本文档的模块边界推进。
