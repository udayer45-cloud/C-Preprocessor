# C Preprocessor

A custom C preprocessor implemented from scratch in C. It simulates the preprocessing stage of a C compiler — handling comment removal, header file inclusion, and macro expansion.

---

## Features

| Feature | Description |
|---|---|
| Comment Removal | Removes `//` single-line and `/* */` multi-line comments |
| Header Inclusion | Handles `#include <file.h>` and `#include "file.h"` |
| Macro Expansion | Expands `#define` macros with proper word-boundary checking |
| String Protection | Macros are not expanded inside string literals `"..."` |
| Memory Management | All heap memory is freed after use |
| Temp File Cleanup | Intermediate `helpfile.i` is deleted after processing |

---

## Project Structure

```
c-preprocessor/
├── preprocessor.c    # Full source code
├── test.c            # Sample input file for testing
├── Makefile          # Build configuration
├── .gitignore        # Git ignore rules
└── README.md         # Project documentation
```

---

## How It Works

```
your_file.c
     │
     ▼
[1. Comment Removal]
     Remove // and /* */ comments
     Preserve comments inside string literals
     │
     ▼
[2. Header File Inclusion]      ──► helpfile.i (intermediate)
     Paste contents of #include files
     │
     ▼
[3. Macro Expansion]            ──► inputfile.i (final output)
     Replace #define macros
     Skip replacement inside strings
     │
     ▼
[Cleanup]
     Delete helpfile.i
```

---

## Build & Run

### Requirements
- GCC
- Linux / Unix (uses `/usr/include/` for system headers)

### Build
```bash
make
```

Or manually:
```bash
gcc preprocessor.c -o myPreprocessor
```

### Run
```bash
./myPreprocessor <your_file.c>
```

### Example
```bash
./myPreprocessor test.c
```

Output: `inputfile.i` — the preprocessed file

### Clean build
```bash
make clean
```

---

## Example

**Input (`test.c`):**
```c
#include<stdio.h>

#define PI 3.14
#define MAX 100
#define MSG "hello world"

/* multi line
   comment test */

int main()
{
    // single line comment
    int r = 5;

    /* inline block comment */ int x = 10;

    float area = PI * r * r;
    int arr[MAX];
    int val = MAX;

    printf("%s\n", MSG);
    printf("area = %f\n", area);

    char *str = "PI is not replaced here";
    printf("%s\n", str);

    return 0;
}
```

**Output (`inputfile.i`):**
```c
/* ... stdio.h contents expanded here ... */

int main()
{
    int r = 5;
    int x = 10;
    float area = 3.14 * r * r;
    int arr[100];
    int val = 100;
    printf("%s\n", "hello world");
    printf("area = %f\n", area);
    char *str = "PI is not replaced here";
    printf("%s\n", str);
    return 0;
}
```

---

## Test Results

| Test Case | Expected | Status |
|---|---|---|
| `#include` system header | Expanded inline | ✅ |
| `#include` user header | Expanded inline | ✅ |
| Single-line comment `//` | Removed | ✅ |
| Multi-line comment `/* */` | Removed | ✅ |
| Inline block comment | Removed | ✅ |
| `#define` object macro | Replaced | ✅ |
| Multiple macro replacements | All replaced | ✅ |
| Macro word-boundary check | No partial matches | ✅ |
| Macro inside string literal | Not replaced | ✅ |
| Comment inside string literal | Not removed | ✅ |
| Temp file cleanup | `helpfile.i` deleted | ✅ |

---

## Known Limitations

- Function-like macros not supported (e.g. `#define SQUARE(x) x*x`)
- Conditional directives not supported (`#ifdef`, `#ifndef`, `#if`, `#endif`)
- Nested includes not handled (headers included by headers)
- Fixed buffer sizes (file paths limited to 100 chars, lines to 1500 chars)

---

## Author

**Uday**  
Systems Programming — C Preprocessor Implementation
