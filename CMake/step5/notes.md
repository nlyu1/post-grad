1. Put makefile on the top level
2. Go to build folder and run `cmake ../`
3. Make cache & files would have been written to the build folder. 
    - Run `make .` to use native make, or 
    - Run `cmake --build .`
4. Build and install: `cmake --build . && cmake --install . --prefix "/home/nlyu/Code/post-grad/CMake/step5/installation"`

Miscellaneous notes: 

1. `add_library` and `add_executable` defines CMake targets. 
2. Targets have their own features (e.g. linked libraries, include targets) as well as interface features. Own features are directly applied to the target, while interface features are applied to other targets which become associated with the current target. 

# Building notes 

Looking at your CMakeLists.txt files, these are excellent questions about CMake fundamentals! Let me break down each concept:

## 1. What does `add_subdirectory` do and not do?

**What it does:**
- **Processes the CMakeLists.txt in the subdirectory**: When CMake encounters `add_subdirectory(MathFunctions)` on line 15, it immediately stops processing the current file and goes to execute `MathFunctions/CMakeLists.txt`
- **Creates a separate build scope**: The subdirectory gets its own build directory (e.g., `build/MathFunctions/`)
- **Makes targets available**: Any targets created in the subdirectory (like the `MathFunctions` library) become available to the parent scope

**What it does NOT do:**
- Does **not** automatically link anything
- Does **not** add include directories
- Does **not** copy files or headers
- Simply makes the targets "known" to CMake - you still need to explicitly use them

## 2. What does "link library" mean? Static vs Dynamic linking?

**Static Linking:**
- The library code is **copied into** your executable at build time
- Results in a larger executable file that contains everything it needs
- No external dependencies at runtime - the executable is self-contained
- In your build directory: you'll see the static library file (e.g., `libMathFunctions.a` on Linux)

**Dynamic Linking:**
- The library exists as a separate file (.so on Linux, .dll on Windows)
- Your executable contains **references** to the library functions
- At runtime, the operating system loads the shared library
- Smaller executable, but requires the shared library to be present at runtime

**In your case:** Since you use `add_library(MathFunctions ...)` without specifying STATIC or SHARED, CMake defaults to static linking. Your `Tutorial` executable will contain the MathFunctions code directly.

## 3. Why both `target_link_libraries` AND `target_include_directories`?

This is a crucial distinction:

**`target_link_libraries(Tutorial MathFunctions)` (line 20):**
- Tells the **linker** where to find the compiled object code/implementations
- Provides the actual function bodies at link time
- Handles the "where is the code?" question

**`target_include_directories()` (lines 23-27):**
- Tells the **compiler** where to find header files during compilation
- Needed so `#include "MathFunctions.h"` can be resolved
- Handles the "where are the declarations?" question

**You need both because:**
1. **Compile time**: The compiler needs headers to understand function signatures, types, etc.
2. **Link time**: The linker needs the actual compiled code to resolve function calls

Think of it this way: headers are like a restaurant menu (tells you what's available), while linking provides the actual food (the implementation).

## 4. When is the subdirectory CMakeLists invoked?

**Exactly at line 15** when `add_subdirectory(MathFunctions)` is encountered.

The execution flow is:
1. Lines 1-14: Process project setup, configure header file
2. **Line 15**: Stop and process `MathFunctions/CMakeLists.txt` completely
   - This creates the `MathFunctions` library target
3. Lines 16-34: Continue with main CMakeLists.txt
   - Now `MathFunctions` target is available for use in `target_link_libraries`

This is why the `target_link_libraries(Tutorial MathFunctions)` on line 20 works - the `MathFunctions` target was already created when the subdirectory was processed.

**Key insight:** CMake processes subdirectories **immediately** when encountered, not at the end of the parent file. This is why order matters in CMake!

# Variable notes 
Great question! This touches on an important CMake scoping concept. The answer depends on whether there's a `project()` call in the subdirectory.

## **Key CMake Directory Variables:**

Let me first clarify the different directory variables:

- **`PROJECT_SOURCE_DIR`** = Directory where the most recent `project()` command was called
- **`CMAKE_SOURCE_DIR`** = Top-level source directory (where top-level CMakeLists.txt is)  
- **`CMAKE_CURRENT_SOURCE_DIR`** = Current directory being processed

## **In Your Case:**

Looking at your `MathFunctions/CMakeLists.txt`, I don't see a `project()` call in the subdirectory. This means:

**Inside `MathFunctions/CMakeLists.txt`:**
```cmake
# Since there's no project() call in MathFunctions/CMakeLists.txt:
PROJECT_SOURCE_DIR      = /path/to/your/top-level/directory  # Same as top-level!
CMAKE_SOURCE_DIR        = /path/to/your/top-level/directory  # Always top-level
CMAKE_CURRENT_SOURCE_DIR = /path/to/your/top-level/directory/MathFunctions
```

So if you use `target_include_directories` in `MathFunctions/CMakeLists.txt`:

```cmake
# Inside MathFunctions/CMakeLists.txt:
target_include_directories(MathFunctions PUBLIC
    ${PROJECT_SOURCE_DIR}  # Points to TOP-LEVEL directory!
    ${PROJECT_SOURCE_DIR}/MathFunctions  # Points to MathFunctions subdirectory
)
```

## **If Subdirectory Had Its Own project() Call:**

```cmake
# If MathFunctions/CMakeLists.txt had this:
project(MathFunctionsLib)

# Then:
PROJECT_SOURCE_DIR      = /path/to/your/top-level/directory/MathFunctions
CMAKE_SOURCE_DIR        = /path/to/your/top-level/directory  # Still top-level
CMAKE_CURRENT_SOURCE_DIR = /path/to/your/top-level/directory/MathFunctions
```

## **Best Practice Recommendation:**

For subdirectories, prefer `CMAKE_CURRENT_SOURCE_DIR` for clarity:

```cmake
# In MathFunctions/CMakeLists.txt - CLEARER:
target_include_directories(MathFunctions PUBLIC
    ${CMAKE_SOURCE_DIR}              # Explicitly top-level
    ${CMAKE_CURRENT_SOURCE_DIR}      # Explicitly this subdirectory
)

# Instead of:
target_include_directories(MathFunctions PUBLIC
    ${PROJECT_SOURCE_DIR}            # Ambiguous - which project?
    ${PROJECT_SOURCE_DIR}/MathFunctions
)
```

## **Summary:**

In your current setup, `PROJECT_SOURCE_DIR` inside `MathFunctions/CMakeLists.txt` **points to the top-level directory** because that's where the most recent `project(Tutorial ...)` call was made. However, using `CMAKE_SOURCE_DIR` and `CMAKE_CURRENT_SOURCE_DIR` makes the intent much clearer and is more robust.

# Includes 

Excellent observation! You're absolutely correct in your interpretation. Let me break this down:

## **`add_library()` Only Needs Source Files**

```cmake
add_library(MathFunctions MathFunctions.cc)
add_library(SqrtLibrary STATIC mysqrt.cc)
```

Yes, `add_library()` only specifies **source files** (.cc, .cpp, .c) - never header files (.h, .hpp). CMake doesn't need to know about headers for compilation of the library itself.

## **Header Files Are Found Automatically**

The compiler **automatically looks for headers** in the same directory as source files. So if you have:

```
MathFunctions/
├── MathFunctions.cc     # Source file
├── MathFunctions.h      # Header file (same directory)
├── mysqrt.cc           # Source file  
└── mysqrt.h            # Header file (same directory)
```

When `MathFunctions.cc` does:
```cpp
#include "MathFunctions.h"  // Found automatically - same directory
#include "mysqrt.h"         // Found automatically - same directory
```

The compiler finds these headers without any `target_include_directories()`.

## **When Do You Need `target_include_directories()`?**

### **1. For External Code Using the Library**
The **parent CMakeLists.txt** needs to tell the `Tutorial` executable where to find MathFunctions headers:

```cmake
# In top-level CMakeLists.txt:
target_include_directories(Tutorial PUBLIC
    ${PROJECT_SOURCE_DIR}/MathFunctions  # So Tutorial can find MathFunctions.h
)
```

### **2. When Library Needs Headers from Other Directories**
If your library source files need headers that aren't in the same directory:

```cmake
# In MathFunctions/CMakeLists.txt:
target_include_directories(MathFunctions PRIVATE
    ${CMAKE_SOURCE_DIR}/some/other/directory  # For internal use only
)

# Or if external code also needs these headers:
target_include_directories(MathFunctions PUBLIC
    ${CMAKE_SOURCE_DIR}/shared/headers  # Both library and users can access
)
```

## **Your Current Setup Analysis**

Looking at your `MathFunctions/CMakeLists.txt`, it works because:

1. **Internal headers**: `MathFunctions.cc` and `mysqrt.cc` find their headers automatically (same directory)
2. **External access**: The parent CMakeLists.txt handles exposing headers to `Tutorial`:
   ```cmake
   target_include_directories(Tutorial PUBLIC
       ${PROJECT_SOURCE_DIR}/MathFunctions  # Makes MathFunctions.h available
   )
   ```

## **Example When You'd Need `target_include_directories()` in Subdirectory**

```cpp
// If MathFunctions.cc needed to include something like:
#include "common/utilities.h"  // Not in same directory
#include "third_party/boost/math.hpp"  // External library
```

Then you'd add:
```cmake
# In MathFunctions/CMakeLists.txt:
target_include_directories(MathFunctions PRIVATE
    ${CMAKE_SOURCE_DIR}/common      # For utilities.h
    ${CMAKE_SOURCE_DIR}/third_party # For boost headers
)
```

**Key insight**: Header inclusion works at two levels - **internal** (library finding its own headers) and **external** (other targets finding the library's public headers). The subdirectory CMakeLists.txt handles internal needs, while the parent handles external exposure.