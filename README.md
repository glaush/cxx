# Introduction 
Here you can find examples of C++ code.

# Getting Started

There are a few steps to get up and running on own system:

#### Software dependencies

<font size="4">[Conan](https://conan.io/index.html)<font size/>

This is an open source, decentralized and multi-platform package manager for C/C++.

Recommended install:

1. Install Python: 

     [Python 3.8.7.exe](https://www.python.org/ftp/python/3.8.7/python-3.8.7-amd64.exe)

2. Install Conan. Open ```cmd``` and type this command: 

    ``` $ pip install conan```

3. Execute sequentialy two .vsix files in repo root: 

 - ```Cxx Projects_debug.vsix```
 - ```Cxx Projects_release.vsix```

   (After this step, the "Cxx Template" project will be available in your Visual Studio in the "C++" group.
    You will only need this if you decide to add your example to this repository) 

4. Run ```setup.bat``` file and it starts installing all the libraries you need to run the examples.
   All required libraries are listed in the file ```conanfile.txt```

5. The last step is to run the Solution.sln file.
  From solution you will be able to run all the examples available in this repository. 

  <b>The contributors to this repository wish you luck, have fun ;)</b> 

![](https://img5.goodfon.ru/wallpaper/nbig/3/3c/vzgliad-temnyi-fon-kotenok-kotiata-kotionok-regdoll-mnogo-ch.jpg)

--- 