# Cisco Kinetic EFM IOT C++ SDK Changelog

## Changes since 1.2.2

### Third-party library changes

* OpenSSL has been updated to 1.0.2r

### Other changes

* Specifying an invalid broker port will now cause a proper error.
* A `list` request on a non-existent node will now give a proper response.
* Fixed response memory leak.
* Fixed missing `ack` by requester link when subscription updates are being processed.
* Added `error_` to `InvokeResponse` which contains the error message if the invoke failed.
* Fixed missing `@subscription` actors.
* Added an additional header with json conversion functions.
* The SDK base64 decode function will now decode correctly a binary with missing padding.

## Changes since 1.2.1

### Packaging changes

The package names have been changed to reflect the actual platform and architure:
* `Centos7` -> `centos7-x86_64`
* `Ubuntu14.04` -> `ubuntu14.04-x86_64`
* `Ubuntu16.04` -> `ubuntu16.04-x86_64`

### Other changes

* The `responder` and `simple-responder` examples no longer proceed if the link initialization fails.
* `LinkOptions::parse()` will no longer throw if reading `dslink.json` fails.
* The stream insertion operator for `Variant` will now properly restore the flags of the stream in case a `Variant::Double` was processed.
* The extended `dslink.json` format and the command-line options were documented in `CONFIG.md`.

## Changes since 1.2.0

* An `ack` for a message was being before all requests and responses had been properly processed.
* An `invoke` response might have contained incorrect meta data.
* Column overrides in action responses are now being applied.

## Changes since 1.0.15

From now on the SDK version is being aligned with the Cisco Kinetic EFM IOT C++ Broker version. This has no effect on the existing compatibility.

### Other changes

* Added `package` and `package-shared` targets to all examples to generate a package which can be deployed via the LCM.
* Improved performance of message and subscription update processing.

## Changes since 1.0.14

### Breaking changes

* Updated redo log file format to version 2 (no migration necessary as it is compatible with version 1).
    * Files generated with this version can no longer be read by earlier versions.

### Packaging Notes

* All C++ links have to supply a package name in their `dslink.json` file which contains the string `-cpp-`. This is
  necessary for the C++ Broker Lifecycle Manager to distinguish the different link SDK languages.

### Third-party library changes

* Updated Boost to 1.69.
* Added zlib 1.2.11.

### Other changes

* Better error handling for link options.
* Fixed typos in error messages.
* Fixed premature calling of callback in node removal operations.
* Reduced CPU usage while reading from redo log.
* Fixed potential redo log consistency errors when using `Qos::Persistent` in case of high message load when a link disconnects and connects again.
* Fixed crash with a redo log containing files with invalid data only when `automatic_recovery` is enabled (default is `true`).
* Reduced CPU usage when `-log debug` is used.
* Added simple_responder example. This example is more like the classic 'hello, world!' and much simpler than the responder example.
* Fixed potential redo log consistency error when loading valid files (could happen if a cleanup was already performed on that particular log).
* Added new command line options:
    Command line      | dslink.json (ssl section)
    ----------------- | -----------------------------------
    --ssl-self-signed | self_signed_tls_certificate_allowed
    --ssl-certs-path  | certs_path
    --ssl-ca-file     | ca_file
    --ssl-verify-peer | verify_peer

## Changes since 1.0.13

* Peak memory usage has been greatly reduced.
* CPU usage has been reduced - also improves overall throughput.
* Bug fixes.

## Changes since 1.0.12

### Breaking changes

* The names of the members of `NodeInfo` and `NodeCreateOrSetValueDescription` in `efm_responder.h` have changed which might require code change.

### Other changes

* Links developed with the C++ SDK can now be started with the Dart Lifecycle Manager.

## Changes since 1.0.11

* Bug fixes.

## Changes since 1.0.10

* Bug fixes.

## Changes since 1.0.9

* Bug fixes.

## Changes since 1.0.8

### Breaking changes

* The `certs_path` and `ca_file` no longer point to common locations by default but to the `ca` folder within the working directory instead. You either need to change those settings to the one matching your system or just add the CA certificates you require to either the folder or the file.
* The default cipher list have been adjusted to only include the most secure ciphers. Because of a configuration issue in the C++ broker they were set to include all ciphers. This also causes the certificate verification not to be performed at all. So if you are using HTTPS you might no longer be able to connect. The following solutions are available:
  * You need to place your CA certificate in the `certs_path` or in the `ca_file`.
  * You can disable the certificate verification by setting the `verify_peer` option in the `ssl` section to `false` - _unsafe - highly discouraged_
  * If you are using self-signed certificate you also need to set the `self_signed_tls_certificate_allowed` in the `ssl` section to `true`
* The redo logs link options base path was renamed from `base_path` to `path`.

### Other changes

* Fixed a regression which required links to provide a certificate and key to be able to start.
* It was not possible to configure the client cipher list using `cipher_list` in the `ssl` configuration.
* Added `verify_peer` to the `ssl` configuration to control the verification of the peer certificate.
* Some folders which are implicitly created got the wrong (unsafe) permissions.
* New link option `max_send_queue_length`. This option specifies the maximum length of the internal send queue. If this number of send messages has not been acknowledged yet, the sending will be paused until at least some of these messages have been acknowledged.

## Changes since 1.0.7

### Breaking changes

* To restore compatibility with existing SDKs and the C broker the file format of the `key` has been changed. Please delete your existing file (defaults to `.key`).

### Other changes

* A node name will be checked for illegal characters. Illegal characters are: ```/\?*:|"<>```.
* A new function ```string_escape``` to encode a node name with illegal characters was added.
* A callback can be added via ```NodeBuilder::on_subscribe``` to be informed about subscribers to the node.
* A new ```NodeBuilder::writable``` overload also accepts a callback that will be called upon an \@set action to update the nodes value.
* Bug fixes

## Changes since 1.0.6

### Breaking changes

* The SDK enum ```Permission``` was renamed to ```PermissionLevel```.
* Added an enum value None to ```LinkType```.
* Changed the return type of the ```LinkType``` operations ```&``` and ```|``` to ```LinkType``` instead of bool.

### Other changes

* The ```sdk-cpp-serial``` example has been removed.
* The top of folder of the redo log ```base_path``` will now be created automatically and is no longer necessary to exist on start up. Though it will still fail if the ```base_path``` doesn't exist.
* The timestamps now support a variable decimal fraction length.
* Value updates will now be serialized for serializable nodes.
* The actual serialization of serializable nodes will now be deferred and executed with a configurable frequency.
* The serialization can also be configured to not include value updates in order to reduce write operations to the filesystem.
* The ```NodeBuilder::serializable``` method now accepts an enum which configures the serialization behaviour concerning value updates.
* The ```ActionResult``` now has a ```set_error_text``` method to provide an error to the peer from an action.
* The ```ActionResultStream``` now has a ```is_closed``` method to check if the stream was already closed.
* Bug fixes

## Changes since 1.0.5

* Removed support for Debian 8.
* Connecting with a broker URL containing an IPv6 address is now possible.
* Removing a serializable node, that is a direct child of the root node, works now as expected and the node is removed
  from the nodes.json file.
* Bug fixes.

## Changes since 1.0.4

### Breaking changes

* The namespace has been changed from ```cisco::dsa``` to ```cisco::efm_sdk```.
* The prefix of the includes has been changed from ```dsa_``` to ```efm_```.
* The prefix of the logging macros has been changed from ```LOG_SDK_``` to ```LOG_EFM_```.
* The redo log format has changed and existing files are no longer compatible. Existing logs found in ```.redo``` folder need to be removed.
* It is now also required for the redo log base folder (```.redo``` by default) to exist before the link is being started.
* TLS certificates will now be validated by default. This has the following impact:
    * _broker with self-signed certificate_: Connections with self-signed certificates are not allowed by default. To allow this again you need to set ```self_signed_tls_certificate_allowed``` in the SSL settings to ```true```.
    * _broker with proper certificate_: By default it will use the certificates found in ```/etc/ssl/certs``` and ```/etc/ssl/certs/ca-bundle.crt``` for verification. If the locations differs on your system you can change the location via ```certs_path``` and ```ca_file``` in the SSL settings.

### Other changes

* Logging for TLS connections is now CSDL conform.
* Added configuration options for Quality of Service, redo logs and SSL.
* Subscriptions to not yet available nodes work now.
* All folders will be created with restricted file permissions.
* Added a new responder API function to retrieve detailed node information for a path.
* Added a new responder API function to atomically create a node or update the value of the node path.
* QoS3 redo log entries will now be encrypted by default.
* Changed external logging macros to take a stream for the message composition.
* Added external debug logging macro LOG_EFM_DEBUG.
* Added build for Ubuntu 14.04 (requires g++ 4.9).
* Bug fixes.

## Changes since 1.0.3

* QoS level 3 handling added.
* Action result columns will be validated to the columns set on the action. An error log entry will be created if the validation failed.
* A link will now add automatically a /defs/profile node and will register automatically not yet registered node profiles.
* The NodeBuilder config and attribute handling was fixed to match the documentation.
* Added the writable config to the NodeBuilder. Setting a node to writeable will make the \@set action available on a node.
* The existence of a node can now be checked synchronously.
* The child paths of a node can now be enumerated.
* A requester will now correctly send a subscribe request to the peer even if it has already subscribed previously.
* Added callbacks for subscribe, unsubscribe, metadata changed, and value changed to responder.
* Fixed link ack message handling.
* Bug fixes.


## Changes since 1.0.2

* Logging macros were added to the SDK.
* Node creation callbacks now flag in which context (serialization or normal operation) they were called.
* The variant implementation is now more robust and cleaner to handle.
* Download editor was added for actions.
* Bug fixes.


## Changes since 1.0.1

* QoS level 0, 1, and 2 have been implemented.
* Permission tokens can be used with a link.
* Action parameters can have editors configured.
* Number of worker threads can be configured.
* Table streams have been added.
* Debian8 and Ubuntu 16.04 builds added.
* Bug fixes.


## Changes since 1.0.0

* Nodes can be serialized and deserialized now. Serialization creates a nodes.json file compatible to the Java SDK.
* Callbacks for node path creation patterns can be added to a responder firing upon node creation.
* Actions can stream data to the peer.
* Node values and configs can be set via the DSA set method from a requester.
* Node configs can be removed via the DSA remove method from a requester.
* A non blocking run method was added to the Link for better integration into other libs/frameworks.
* Bug fixes.

