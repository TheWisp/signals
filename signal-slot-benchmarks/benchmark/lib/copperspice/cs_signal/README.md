## CsSignal

### Introduction

CsSignal is a library for thread aware Signal/Slot delivery. This library does not depend upon
CopperSpice or any other libraries.

One of the major benefits of CsSignal is how the library delivers signals in a multithreaded
application. Signals can be delivered using a queued connection or a blocking queued connection.

In the CsSignal library signals and slots are both methods whereas in many other signal/slot
libraries each signal is a separate object.


### System Requirements

To use CsSignal you will need a C++14 compiler and a C++14 standard library.

Currently uses the Autotools build system for building and running the unit test suite. The library has been tested with
clang sanitizer and a major code review.


### Documentation

Class level documentation for CsSignal is available on the CopperSpice website:

www.copperspice.com/docs/cs_signal/index.html


### Presentations

Multiple videos discussing Signals and Slots can be found on the following pages:

www.youtube.com/copperspice <br>
www.copperspice.com/presentations.html


### Authors / Contributors

* **Ansel Sermersheim**
* **Barbara Geller**


### License

This library is released under the BSD 2-clause license. For more information refer to the LICENSE file provided with this
project.


### References

* Website: www.copperspice.com
* Email:   info@copperspice.com
