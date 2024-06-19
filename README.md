```markdown
# masscode2md

`masscode2md` is a simple tool for converting code snippets from a JSON format into Markdown files. The project reads a JSON file containing code snippets and organizes them into folders based on the specifications in the JSON file.

## Requirements

- C++ compiler
- [nlohmann/json](https://github.com/nlohmann/json) library
- CMake (optional for build system)

## Installation

1. Get the source code:

   ```sh
   git clone git@github.com:tomasmark79/masscode2md.git
   cd masscode2md
   ```

2. Download dependencies (nlohmann/json):

   - Using vcpkg:

     ```sh
     vcpkg install nlohmann-json
     ```

   - Or you can include the library directly in the project by downloading `json.hpp` from [GitHub](https://github.com/nlohmann/json/releases).

## Compilation

If you are using CMake, follow these steps:

1. Create a build directory:

   ```sh
   mkdir build
   cd build
   ```

2. Generate build system files and compile:

   ```sh
   cmake ..
   cmake --build .
   ```

If you are not using CMake, you can compile directly using a C++ compiler:

```sh
g++ -o masscode2md main.cpp -std=c++17 -lstdc++fs
```

## Usage

The program requires two arguments: the path to the input JSON file and the output directory.

```sh
./masscode2md <input_json_path> <output_directory>
```

For example:

```sh
./masscode2md snippets.json output
```

## Example JSON File

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

## Code Description

The main function of the program performs the following steps:

1. Loads the path to the JSON file and the output directory from the input arguments.
2. If the `ERASE_OUTPUT_DIR_AT_STARTUP` macro is set to `true`, it deletes the contents of the output directory at program startup.
3. Loads and parses the JSON file.
4. Creates folders specified in the JSON file.
5. Creates Markdown files for each code snippet that is not marked as deleted.

## License

This project is licensed under the MIT License. For more information, see the LICENSE file.

## Author

This project was created by [Tomáš Mark](git@github.com:tomasmark79/masscode2md.git).
```

This `README.md` contains basic information about the project, how to install, compile, and use it. If you have any specific requirements or additional information you'd like to include, feel free to add them.