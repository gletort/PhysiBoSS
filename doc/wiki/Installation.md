# Installing PhysiBoSS 

PhysiBoSS requires moderatly recent version of C++ (at least c++11) and OpenMP support. Compilation of MaBoSS library requires `flex` and `bison` library, usually already present (and can be easily installed on e.g. Linux ubuntu with `sudo apt-get install bison flex`).

To install it on Unix system, from a Terminal:
Clone the repository on your local machine, and go inside the main directory.
Install and compile  MaBoSS, by going into the MaBoSS directory and type 'make'. Or directly type 'make maboss' in the main PhysiBoSS directory.
Then type 'make' in the main directory to compile everything. The executables will be created in the 'bin' directory if all goes well. 
It can be compiled in 'Debug', 'Release' or 'Proliling' modes, to set in the 'Makefile' file. Default is 'Release' mode (fastest).
You might also have to change your c++ compiler in the Makefile according to your operating system.

Commands list:
~~~bash
git clone https://github.com/gletort/PhysiBoSS.git
cd PhysiBoSS
make install
~~~

If it doesn't work, make it step by step until you reach the error:
~~~bash
git clone https://github.com/gletort/PhysiBoSS.git
cd PhysiBoSS
cd MaBoSS
make clean
make
cd ..
make
~~~

## Errors on Linux
If you got errors while comiling MaBoSS library (first `make`), you might be lacking some required library (`yacc`, `flex` or `bison`).
On Ubuntu, you can install them with: `sudo apt-get install bison flex yacc`.
We were able to directly compile it with `yum install -y make gcc flex bison gcc-c++` on Fedora 23.
On CentOS, you can also directly install the development tools with `yum groupinstall -y "Development tools"` which allows you to install all necessary dependencies.

## Errors on MacOS
You need to have a c++ compiler compatible with OpenMP. On MacOS, it is not always straightforward. If you got the error `-fopenmp not found` while compiling PhysiBoSS (second `make`), you need to install it. You can refer to this [page](http://www.mathcancer.org/blog/setting-up-gcc-openmp-on-osx-homebrew-edition/) for detailed tutorial on how setting up gcc with OpenMP support.

If, when compiling MaBoSS part, you got the error `Undefined symbols for architecture x86_64`, you may have to use `g++-7` instead of `g++`. For this change in the file `Makefile.maboss` the command `g++` to `g++-7`. Then you will have to compile PhysiBoSS with `make CC=g++-7` in the PhysiBoSS folder.

NB: in MacOS system, on run time, you might get the warning `Warning: non reentrant random, may not work properly in multi-threaded mode`, which comes from the MaBoSS part. However, it had always run properly without issue despite this warning.


## Errors still not fixed
If you still cannot compile PhysiBoSS after trying our suggestions, please don't hesitate to contact us or create an issue request. And/or don't hesitate to use the [Docker image](https://github.com/gletort/PhysiBoSS/tree/master/Docker) !