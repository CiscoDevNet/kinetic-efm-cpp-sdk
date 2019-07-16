# Cisco Kinetic EFM IOT C++ SDK Readme

## Overview

The Cisco Kinetic EFM IOT C++ SDK provides technologies to develop DSA links with a modern and easy to use C++ API.

## Version

The current SDK version is `1.2.3`.



## System Requirements and Installation

The Cisco Kinetic EFM IOT C++ SDK is provided for the following platforms:

* CentOS 7 (x86_64)
* Ubuntu 14.04 (x86_64 / requires g++ 4.9) - **DEPRECATED Support for Ubuntu 14.04 ends in April 2019**
* Ubuntu 16.04 (x86_64) - also for IOx-powered devices with x86 processor

On all system, excluding Ubuntu 14.04, you will also need the g++ platform compiler installed to build links with the SDK.

## Release Contents

* `include/` - API header files
* `lib/` - API static libraries
* `examples/` - Samples to get you started
* `docs/` - API documentation
* `CHANGELOG.md` - List of changes per version
* `CONFIG.md` - Documentation of configuration files
* `LICENSE.md` - License information
* `README.md` - This readme

## Limitations

* No roll-up yet.
* No support for profiles.

## Known Issues

## Changelog

See `CHANGELOG.md` for changelog information.

## Examples

You can find a couple of samples in the `examples` directory to show the use of the API and should get you started quickly.

* `examples/responder/` - Implements a responder link example
* `examples/requester/` - Implements a requester link example

To build an example, just invoke `make` in the corresponding directory.

To create a package which can be deployed via a LCM run the following command:

    prompt> make package

To use the examples you need a running broker. As the requester example subscribes to paths from the responder
example, you should start the responder example before starting the requester example.

To start an example you have to supply the broker url by using the `-b` command line parameter:

    prompt> ./responder -b http://localhost:8080/conn

In order to connect via HTTPS to a broker, you might need to enable self signed certificates by adding the following
section to your `dslink.json` link configuration file:

    "ssl": {
      "self_signed_tls_certificate_allowed": true
    }

Then connect to a C++ Broker:

    prompt> ./responder -b https://localhost:8463/conn --ssl-ca-file /opt/cisco/kinetic/cpp_broker/server.ca-bundle

## Compatibility

Currently any C++ SDK and C++ Broker with a matching major version (first digit) are compatible with each other.

Minor version updates (second digits) are done for each release cycle and will not break any compatibility.

There is no need to re-compile with a new SDK version if a new broker is being released (barring any issues that might require an update).

Updating either of them is not supposed to be causing any issues.

C++ SDK Version | C++ Broker Version
--------------- | ------------------
any 1.x.y       | any 1.x.y

## License

See `LICENSE.md` for license information.

## Third-party Software

The Cisco Kinetic EFM IOT C++ SDK uses the following third-party software:

* Boost 1.69
* RapidJSON 1.1.0
* zlib 1.2.11
* OpenSSL 1.0.2r

COPYRIGHT © 2018-2019 CISCO SYSTEMS, INC. ALL RIGHTS RESERVED.

