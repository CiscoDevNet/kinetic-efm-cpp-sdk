# Cisco Kinetic EFM IOT C++ SDK Configuration Files

_This documents just the `dslink.json` settings and command-line option which were added in the C++ broker. (For the original C SDK `dslink.json` documentation see https://github.com/IOT-DSA/docs/wiki/dslink.json)_

## dslink.json

The `dslink.json` is an optional configuration file which need to be located in the working directory of the link to be loaded.

All these options can also be configured programmatically using the `LinkOptions` class (see `efm_lnk_options.h`). This also documents the default values of the options.

* `key` - The key file name
* `max_send_queue_length` - Maximum length of internal send queue
* `qos` - section for Quality of Service (QoS) settings
    * `default_queue_length` - The default queue length used by QoS level 1 and up
* `redo_log`- section of Redo Log (used by QoS level 3) settings
    * `path` - The base path all redo log will be stored in
    * `max_entries_per_file` - The maximum entries of each redo log file
    * `max_size_per_file_bytes` - The maximum size (in bytes) of each reod log file
    * `max_files_per_log` - The maximum number of files in each redo log folder
    * `flush_after_write` - Controls if a flush is performed after each write operation
    * `automatic_recovery` - Controls if consistency issues of the redo log are being resolved automatically on start-up
    * `write_encrypted_values` - Controls if the data written for values is being encrypted
    * `min_available_disk_space_threshold_mb` - The minimum available disk space threshold (in MB)
* `ssl` - section for SSL client settings
    * `self_signed_tls_certificate_allowed` - Specifies if self signed certificates are allowed or not
    * `certs_path` - Specifies the location the certificate verification will look for certificates
    * `ca_file` - Specifies the location of the certification authority (CA) certificates files
    * `cipher_list` - Specifies the cipher list (see https://www.openssl.org/docs/man1.0.2/apps/ciphers.html)
    * `verify_peer` - Specifies if the certificate of the peer should be verified
* `serializer` - section for serializer (responsible for writing `nodes.json`) settings
    * `serialization_frequency` - The serialization will be called with this frequency in ms
    * `serialize_values` - Controls if node values shall also be serialized
    
## Command-Line Options 

* `--broker,-b` - Sets the broker URL to connect to
* `--token,-t` - Sets the token
* `--log,-l` - Sets the logging level
* `--name,-n` - Sets the DSLink name
* `--key,-k` - Sets the key file name
* `--workers,-w` - Number of worker threads available for the thread pool
* `--ssl-self-signed` - Controls if self signed SSL certificates are allowed
* `--ssl-certs-path` - SSL certification authority (CA) path
* `--ssl-ca-file` - SSL certification authority (CA) file
* `--ssl-verify-peer` - Controls SSL peer certificate validation
