*LabVIEW uses a unique timestamp format based on a 1904 epoch and a 128-bit seconds/fraction structure. This repository simply aims to allow timestamps to be used more naturally in your LabVIEW/C++ projects using the `chrono` library.*

# LabVIEW Timestamp Converter (C++20 DLL)

This project provides a C++20-compatible DLL for use with LabVIEW. It enables conversion of LabVIEW timestamps into C++ `std::chrono::sys_time` objects. The user is intended to extend the functions to their need for their specific use-case, *this repository provides the bare minimum functionality for the task of converting LabVIEW timestamps*.


# Building the DLL

You will need a C++20-compatible compiler.
When compiling, be sure the DLL is `x64` compatible. LabVIEW does not accept `x86` DLLs.


# Planned Functionality

- Reverse conversion (`std::chrono::sys_time` to LabVIEW-compatible timestamp)
- Time zone awareness