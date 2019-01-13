# _Academy_, an university manager

* **Current version** - 0.2.0
* **Date** - Jan 11, 2019
* **Available languages** - Portuguese
* **Author** - Igor Nunes, UBI
* **Advisor** - Abel Gomes, PhD, UBI


## What is _Academy_?

**_Academy_** is a software dedicated to manage the most fundamental components of an university or college:

* Curricular units (or subjects);
* Courses;
* Professors;
* Students.

Currently, _Academy_ only handles one university/college at a time. However, the code is currently ready to be expanded in order to handle more institutions.


## How to compile

The _Makefile_ available allows you to build _Academy_ in two possible modes:

* `make debug` &#8212; _Academy_ will give debugging information no matter what regarding:
    1. Loading from file;
    2. Unlinking relational data;
    3. Saving to file.
* `make release` &#8212; _Academy_ will be built as a final software without any debugging information.

Furthermore, `make clean` is available to erase object and binary files in order to recompile the code using whatever mode you'd like.


## Interaction modes

_Academy_ currently only supports a basic TUI mode (Text-based User Interface).

However, more modes are currently ready to be built into the software, namely:

* An alternative TUI mode;
* A GUI mode;
* A Terminal mode.

The software already can detect these modes, but since they're not implemented, _Academy_ will simply show the message `NOT AVAILABLE`.


## How to fill the database in

Firstly, you must insert the maximum number of curricular units (or subjects) possible.

Then, you must create courses.

Only then you should create professors and students since they rely on those as relational data (using intra-structure (vectors of) pointers).


## Future plans

In spite of the fact that _Academy_ originated as an university project, it will be carried on as a didactic project to further improvise my C programming skills.


# DISCLAIMER

This project is a hobbie. The author and adviser are NOT responsible for any loss of data resulting from the use of this software. **USE IT AT YOUR OWN RISK!**