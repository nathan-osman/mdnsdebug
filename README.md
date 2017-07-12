## mdnsdebug

This simple application uses the [QMdnsEngine](https://github.com/nitroshare/qmdnsengine) library to monitor mDNS traffic travelling through the network.

### Example Output

    [000067.905] message (query) from 10.10.0.108
      1 query(s):
        - service information for "HP ENVY 5530._pdl-datastream._tcp.local."

    [000068.195] message (response) from 10.10.0.110
      4 record(s):
        - "HP ENVY 5530._pdl-datastream._tcp.local." is at "HP5530.local.", port 9100
        - "HP ENVY 5530._pdl-datastream._tcp.local." has the following data:
            - Color: T
            - Duplex: T
            ...
        - address for "HP5530.local." is 10.10.0.110
        - address for "HP5530.local." is fe80::d2bf:9cff:feaf:6223
