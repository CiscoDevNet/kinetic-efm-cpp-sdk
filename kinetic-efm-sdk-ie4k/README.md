# Cisco Kinetic EFM IOT C++ SDK Readme

## Overview

The Cisco Kinetic EFM IOT C++ SDK provides technologies to develop DSA links with a modern and easy to use C++ API.

## Version

The current SDK version is 1.0.14.

## System Requirements and Installation

The Cisco Kinetic EFM IOT C++ SDK is provided for the following platforms:

* CentOS 7 (x86_64)
* Ubuntu 14.04 (x86_64 / requires g++ 4.9)
* Ubuntu 16.04 (x86_64)

On all system, excluding Ubuntu 14.04, you will also need the g++ platform compiler installed to build links with the SDK.

## Release Contents

* include/ - API header files
* lib/ - API static libraries
* examples/ - Samples to get you started
* docs/ - API documentation
* CHANGELOG.md - List of changes per version
* LICENSE.md - License information
* README.md - This readme

## Limitations

* No roll-up yet.
* No support for profiles

## Changelog

See CHANGELOG.md for changelog information.

## Samples

You can find a couple of samples in the `examples` directory to show the use of the API and should get you started quickly.

* examples/responder/ - Implements a responder link example
* examples/requester/ - Implements a requester link example

To build an example, just invoke `make` in the corresponding directory.

To start an example you have to supply the broker url by using the `-b` command line parameter:

    prompt> ./responder -b http://localhost:8080/conn

## License

See LICENSE.md for license information.

## Third-party Software

The Cisco Kinetic EFM IOT C++ SDK uses the following third-party software:

* boost 1.67
* RapidJSON 1.1.0

See [Cisco_Kinetic_EFM_IOT_C++_Broker_and_SDK_v1.0.pdf](Cisco_Kinetic_EFM_IOT_C++_Broker_and_SDK_v1.0.pdf) for license information.

COPYRIGHT © 2018 CISCO SYSTEMS, INC. ALL RIGHTS RESERVED.

