# BlackScholes WebAssembly Dashboard

A WebAssembly-based Black-Scholes pricing engine with Svelte and C++ (Emscripten).

## Prerequisites

Make sure the following tools are installed:

- **CMake** (>= 3.20)
- **Make** (MinGW / MSYS / GNU Make on Windows)
- **Emscripten SDK (emsdk)**
- **Node.js** (>= 18)
- **Python 3** (required by Emscripten)

## Setup (Windows)

Before building anything, initialize the Emscripten environment:

```bash
emsdk_env.ps1
````
OR

```bash
emsdk_env.bat
```

This must be executed in every new terminal session before building.


## Build WebAssembly module

From the project root:

```bash
make wasm
```

This will:

* configure the CMake project using Emscripten
* compile the C++ backend to WebAssembly
* output `.js` and `.wasm` files into the frontend


## Run frontend (dev mode)

```bash
make dev
```

This starts the Vite development server for the Svelte frontend.
