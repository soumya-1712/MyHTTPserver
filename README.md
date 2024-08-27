# My HTTP Server

The project 'Myhttpserver' is a lightweight HTTP server developed in C++, offering support for HTTP/1.1 with basic request handling capabilities (GET, POST, PUT, DELETE) and efficient file serving.

## Table of Contents
- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
  - [Dependencies](#dependencies)
  - [Building](#building)
- [Usage](#usage)
- [Configuration](#configuration)
- [Examples](#examples)
- [Troubleshooting](#troubleshooting)

## Introduction

My HTTP Server is a lightweight HTTP server implemented in C++. It supports HTTP/1.1 protocol and can handle common HTTP requests such as GET, POST, PUT, and DELETE. The server also includes a simple graphical user interface (GUI) built with ImGui, allowing users to interact with the server visually.

## Features

- **HTTP/1.1 support**: Full implementation of the HTTP/1.1 protocol.
- **Request handling**: Capable of processing GET, POST, PUT, and DELETE requests.
- **File serving**: Serve files directly from a local directory.
- **Simple GUI**: Includes a basic GUI using ImGui for server management.

## Installation

### Dependencies

To build and run My HTTP Server, you will need the following:

- **CMake** 3.3 or later
- A **C++ compiler** (e.g., GCC, Clang, MSVC)
- **OpenGL** libraries (required for the GUI functionality)
- **System.IO** library (for file system operations)

### Building

To build the server, follow these steps:

1. **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```

2. **Generate the build system files using CMake:**

    ```bash
    cmake ..
    ```

3. **Build the server:**

    ```bash
    cmake --build .
    ```

This will create a `myhttpserver` executable in the `build` directory.

## Usage

To run the server, simply execute the `myhttpserver` executable:

  ```bash
  ./myhttpserver
  ```
By default, the server will start listening for incoming HTTP requests on port 8080.

## Configuration

No specific configuration is required to start the server. However, you can customize the server's behavior by modifying the source code.

## Examples

Here are a few examples of how to interact with My HTTP Server:

1. **Starting the server:**
   ```bash
   ./myhttpserver
   ```
   
2. **Sending a GET request (using 'curl'):
  ```bash
  curl http://localhost:8080
  ```

## Troubleshooting

If you encounter issues during the build or runtime, consider the following:

- Ensure all dependencies are installed and correctly configured.
- Verify that your C++ compiler supports the necessary C++ standards.
- Check if the correct OpenGL libraries are linked.
