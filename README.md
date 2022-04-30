# nes
An emulator that allows for nes games to run. Following [guide](https://bugzmanov.github.io/nes_ebook/chapter_1.html) loosely. (Honestly, huge shoutout to this author this is really nicely written w/ good links that give thorough details. Makes me interested in doing something
  similar to this as I progress and learn new things.)<br><br>
Making this for fun. Want to learn how to use the [SDL library](https://www.libsdl.org), and about adding controller support.
Also learning about the way the hardware / software works w/ this system given no os



Dependency: [cmake](https://cmake.org/install/)
  A quick way to help compile C code
  
To compile, execute inside of your terminal:
```
cmake .
cmake --build .
make
```

To run a test, execute:
```
./test_*
```

To run program (Not running):
```
./nes
```

# Some Interesting Things To Me
1. The status register is used to hold flags about the operation and it just helped me solidify an understanding of what it means to use a flag
2. Addressing modes:<br>
  I feel like adressing modes are interesting b/c on one hand, I feel like it limits the variety of the instructions the cpu could handle.
  At the same time, I feel like it creates an easy langauge to understand for developers to build on top of.
    Which leads me to question if using addressing mode was a design decision, or just a really clever way to use a limited piece of hardware
