# masscode2md

`masscode2md` is a lightweight tool designed to convert code snippets stored in JSON format into organized Markdown files. The tool reads a JSON file containing code snippets and automatically arranges them into directories based on the provided structure in the JSON file.

## Prerequisites

- C++ compiler supporting C++17 or later
- [nlohmann/json](https://github.com/nlohmann/json) library for JSON parsing
- CMake (optional, recommended for streamlined build process)

## Installation

1. Clone the repository:

   ```sh
   git clone git@github.com:tomasmark79/masscode2md.git
   cd masscode2md
   ```

2. Install the required dependencies (nlohmann/json):

   - Using `vcpkg`:

     ```sh
     vcpkg install nlohmann-json
     ```

   - Alternatively, you can directly include the library in your project by downloading `json.hpp` from [GitHub](https://github.com/nlohmann/json/releases).

## Compilation

If you prefer using CMake, follow these steps:

1. Create a build directory:

   ```sh
   mkdir build
   cd build
   ```

2. Generate the build system and compile the project:

   ```sh
   cmake ..
   cmake --build .
   ```

Alternatively, if you are not using CMake, you can compile the project directly using a C++ compiler:

```sh
g++ -o masscode2md main.cpp -std=c++17 -lstdc++fs
```

## Usage

The program accepts two command-line arguments: the path to the input JSON file and the output directory where the Markdown files will be saved.

```sh
./masscode2md <input_json_path> <output_directory>
```

### Example

```sh
./masscode2md snippets.json output
```

## Example JSON Structure

Below is an example of the input JSON file:

```json
{
  "folders": [
    {
      "name": "Example Folder",
      "id": "folder_1"
    }
  ],
  "snippets": [
    {
      "folderId": "folder_1",
      "name": "example_snippet",
      "isDeleted": false,
      "content": [
        {
          "label": "example",
          "value": "#include <iostream>\nint main() { std::cout << \"Hello, World!\"; return 0; }",
          "language": "cpp"
        }
      ]
    }
  ]
}
```

## Functionality Overview

The core functionality of `masscode2md` is as follows:

1. The program accepts two input arguments: the path to the JSON file and the desired output directory.
2. If the `ERASE_OUTPUT_DIR_AT_STARTUP` macro is defined as `true`, the output directory's contents will be cleared upon program startup.
3. The program parses the JSON file and reads the structure of folders and snippets.
4. Folders specified in the JSON are created in the output directory.
5. For each snippet that is not marked as deleted (`isDeleted: false`), a Markdown file is generated containing the snippet’s code.

## License

This project is licensed under the MIT License. For more information, please refer to the `LICENSE` file.

## Author

Developed and maintained by [Tomáš Mark](git@github.com:tomasmark79/masscode2md.git).
