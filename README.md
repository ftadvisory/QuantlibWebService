Protoype creation of a web service wrapper for Quantlib using oat++ 

See more:

- [Quantlib Website](https://www.quantlib.org/)

- [Oat++ Website](https://oatpp.io/)
- [Oat++ Github Repository](https://github.com/oatpp/oatpp)
- [Get Started](https://oatpp.io/docs/start)

## Overview

A prototype to demonstrate exposing Quantlib as a webservice.  Uses oat++ including the Swagger module to expose creating a Vanilla Swap object 

### Project layout

```
|- CMakeLists.txt                        // projects CMakeLists.txt
|- src/
|    |
|    |- controller/                      // Folder containing SwapController where all endpoints are declared
|    |- dto/                             // DTOs (data transfer objects) are declared here (VanillaSwap and helpers)
|    |- AppComponent.hpp                 // Service config
|    |- App.cpp                          // main() is here
|
|- test/                                 // test folder (to be updated)
|- utility/install-oatpp-modules.sh      // utility script to install required oatpp-modules.  
|- .vscode/                              // configuration to build and run using Microsoft Visual Studio code 
|-                                       // will require C++ / Intellisense, CMake and autoconf extensions
```

---

### Build and Run

#### Using CMake

**Requires** 

- `oatpp` module must be installed. You may run `utility/install-oatpp-modules.sh` script to install required oatpp modules (or get the source via `git clone https://github.com/oatpp/oatpp` and make)

- `oatpp-swagger` must be installed
``` 
$ git clone https://github.com/oatpp/oatpp-swagger
$ cd oatpp-swagger/
$ mkdir build && cd build/
$ cmake ..
$ make install

``` 

- `Quantlib` must be installed.  Follow instructions at `https://www.quantlib.org/download.shtml`

To build the servie
``` 
$ mkdir build && cd build
$ cmake ..
$ make 
$ ./QuantlibWebService  # - run application.

```