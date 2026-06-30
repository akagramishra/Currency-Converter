# Currency Converter

A command-line currency converter written in C++ that fetches **live exchange rates** from the [freecurrencyapi.com](https://freecurrencyapi.com) REST API. Enter an amount and two currency codes, and it converts between them using up-to-date rates, displaying the result with the target currency's native symbol.

Built as a hands-on project for learning HTTP requests, JSON parsing, and REST API integration in C++.

## Features

- Fetches real-time exchange rates from a live REST API
- Converts between any two supported currencies (USD, INR, EUR, GBP, and many more)
- Displays results with the target currency's native symbol (e.g. ₹, €, £)
- Case-insensitive currency input — `usd`, `USD`, and `Usd` all work
- Handles unknown currency codes gracefully without crashing
- Continuous loop for multiple conversions until you type `exit`

## Tech Stack

- **C++17**
- **[cpr](https://github.com/libcpr/cpr)** — a modern C++ HTTP client library (a wrapper around libcurl)
- **[nlohmann/json](https://github.com/nlohmann/json)** — JSON parsing for C++
- **CMake** + **vcpkg** for build and dependency management

## Prerequisites

- A C++17-capable compiler (MSVC, GCC, or Clang)
- [CMake](https://cmake.org/) 3.15 or newer
- [vcpkg](https://github.com/microsoft/vcpkg) for installing dependencies
- A free API key from [freecurrencyapi.com](https://freecurrencyapi.com)

## Setup

### 1. Get an API key

Sign up for a free account at [freecurrencyapi.com](https://freecurrencyapi.com) and copy your API key.

### 2. Set the API key as an environment variable

The program reads the key from an environment variable named `API_KEY` — it is **never** hard-coded in the source.

**Windows (PowerShell):**
```powershell
$env:API_KEY = "your_api_key_here"
```

**Linux / macOS:**
```bash
export API_KEY="your_api_key_here"
```

> Note: setting it this way applies only to the current terminal session. Run the program from the same terminal where you set the variable.

### 3. Install dependencies with vcpkg

```bash
vcpkg install cpr nlohmann-json
```

### 4. Build

```bash
cmake -B build -DCMAKE_TOOLCHAIN_FILE=[path-to-vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

Replace `[path-to-vcpkg]` with the location of your vcpkg installation.

## Usage

Run the executable from the same terminal where you set `API_KEY`:

```
Enter the currency you want to convert from USD
Enter the amount you want to convert 100
Enter the currency you want to convert to INR
The converted amount is: ₹ 8350.00
```

Type `exit` at the "convert from" prompt to quit.

## How It Works

On startup, the program makes two requests to the freecurrencyapi REST API:

1. `/v1/currencies` — fetches metadata for all currencies (including native symbols)
2. `/v1/latest` — fetches the latest exchange rates (all relative to a base currency)

To convert between two currencies, it divides the target rate by the source rate (converting through the API's base currency), then multiplies by the amount. The currency symbol is pulled from the metadata response.

## Notes

- Exchange rates are fetched once at startup, so a long session uses rates from when the program started.
- The free API tier has a monthly request limit; this project stays well within it by fetching rates only once per run.
