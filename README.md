# GO++ 

## Overview

GO++ is a C++ library that brings Golang-style concurrency features to C++. It offers a suite of concurrency primitives such as channels, thread pools, and wait groups, designed to simplify the development of concurrent applications in C++.

## Features

- **Channels**: Facilitates communication between threads with finite and list-based channels.
- **Thread Pools**: Manages a pool of threads to execute tasks concurrently, with finite and list-based options.
- **Wait Groups**: Synchronizes the completion of a collection of threads.

## Getting Started

### Installation

Clone the repository:
```sh
git clone https://github.com/AnirudhVetukuri/Gopp
cd Gopp
```

## Detailed API

### Channels

Channels provide a way for threads to communicate with each other by sending and receiving messages.

- `RChannel<T>`: A finite capacity channel. If the capacity is exhausted, the channel blocks until space is available.
- `LChannel<T>`: A list-based channel with no fixed capacity.

Usage Example:

```cpp
RChannel<std::string> channel(3);
channel.Add("test");

std::optional<std::string> res = channel.Get();
assert(res.value() == "test");
```

Stream Operator:

```cpp
channel << "test";

std::string res;
channel >> res;
```

Golang-style Channel Range Iteration:

```cpp
LChannel<int> channel;
auto fut = std::async(std::launch::async, [&] { 
    for (int i = 0; i < 3; ++i) {
        channel << i;
    }
    channel.Close();
});

for (auto const& elem : channel) {
    std::cout << elem << ' ';
}
std::cout << std::endl;
```

### Thread Pools

Thread pools manage a pool of threads to perform tasks concurrently.

- `ThreadPool<T>`: A thread pool with a finite number of tasks. If the capacity is exhausted, new tasks block until existing tasks complete.
- `LThreadPool<T>`: A list-based thread pool without a fixed capacity.

Usage Example:

```cpp
ThreadPool<int> pool;
std::future<int> fut = pool.Add([] {
    int sum = 0;
    for (int i = 0; i < 5; ++i) {
        sum += i;
    }
    return sum;
});

assert(fut.get() == 1 + 2 + 3 + 4);
```

### Wait Groups

Wait groups synchronize the completion of a collection of threads, allowing you to wait for all of them to complete before proceeding.

Usage Example:

```cpp
WaitGroup wg;
wg.Add();

auto fut = std::async(std::launch::async, [&] {
    std::this_thread::sleep_for(2s);
    wg.Done();
});

auto start = std::chrono::steady_clock::now();
wg.Wait();
auto end = std::chrono::steady_clock::now();

std::cout << std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
```

## Contributing

This is a work in progress and we welcome contributions!

## License

This project is licensed under the MIT License - see the LICENSE file for details.
