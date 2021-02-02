# Introduction

The Thingsml library helps you create and parse [senml documents](https://tools.ietf.org/html/rfc8428)
in both json and [cbor format](https://tools.ietf.org/html/rfc7049).

# Documentation

Read the docs [here](https://kpn-iot.github.io/thingsml-c-library/)

# Key features

- Object oriented design. 
- Built in support for [senml's unit registry](https://tools.ietf.org/html/draft-ietf-core-senml-12#section-12.1) 
- Extensible for new data types
- Makes use of (but doesn't restrict to) ThingsML predefined list of record names via ThingsML indices.
- Direct support to read/write in json and cbor format.
- Automatically adjusts record data with respect to base time, base value & base sum.
- The library is optimized for devices with restricted memory requirements (runs on AVR systems with 1KB ram). 

# Under improvement (PR's welcome!)

- Examples
- Documentation
- Tests

# Running

## Bazel
Bazel can be installed from here: https://docs.bazel.build/versions/master/install.html \
Run the unit tests using:

    bazel test test --test_output=all

## Arduino
Install using the built in library manager, search for "ThingsML".\
The library includes various examples which can be openened from the examples dropdown.
