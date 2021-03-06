---
id: coding-standard
title: Coding Standard
---

For starters, please read through the
[C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines).
Some of its points may only be repeated here on this page for emphasis.

## General Practices

* Don't use run-time type information.
* Don't use exceptions.
* Initialise variables in declaration, as close to the site of first use as
  possible, and in the most limiting scope possible.
* Avoid static and global variables unless they can be declared `const` or
  `constexpr`. Otherwise, use the `Global` template for classes that should be
  globally accessible.
* Avoid type casting. Use explicit casts like `static_cast` and
  `reinterpret_cast` if you must. Old C-style cast is forbidden.
* Use `constexpr` and `const` whenever you can.
* Use macros sparingly. If you cannot avoid them, try to `#define` them just
  before usage site and `#undef` right after.
* Use `0` for integers, `0.0` for reals, `nullptr` for pointers, and `\0` for
  chars.
* Prefer `sizeof()` on the instance itself rather than the type.

### Interfaces

* Declare freestanding functions within the `rainbow` namespace, or as static
  member functions. Global functions are not allowed.
* Use `NotNull` to indicate that `nullptr` is not valid.
* Use `Owner` to indicate ownership when a smart pointer makes things
  unnecessarily more complex.
  * Assume that a naked pointer/reference is borrowed. Don't delete these
    yourself.
* Use `final` and `override` when overriding methods.
* Declare derived classes `final` whenever possible.
* Avoid separating template method declaration and definition whenever possible.
  Some compilers fail to recognise the corresponding definitions for non-trivial
  templates when separated.

### Namespaces

* Use unnamed namespaces in your `.cpp`s.
* _using-directives_, e.g. `using namespace std;`, is forbidden.
* _using-declarations_, e.g. `using std::vector;`, are allowed anywhere in
  `.cpp`s, and inside classes, functions, methods, and namespaces in `.h`s.

### Comments

Use [XML documentation](https://msdn.microsoft.com/en-us/library/ms177226.aspx).
It is supported by both [Doxygen](http://doxygen.org/) and Visual Studio's
[IntelliSense](https://msdn.microsoft.com/en-us/library/hcw1s69b.aspx).

### Logging

* Use `R_ASSERT` for assertions.
* Use `LOGD/E/F/I/W` macros for printing to the console with a timestamp and
  severity level (debug, error, fatal, info, and warning respectively). Note
  that only messages logged with `LOGE` will be visible in release builds.
* Use `R_ABORT` to terminate with an error message.

## Source Files

### Headers

Filenames must end in `.h`, and should be self-contained. That means that you
should be able to simply include it without having to include or declare
anything else.

### Header Guards

All header files must have `#define` guards. The format of the macro should be
the full path of the file starting from `src/`. E.g. the file
`src/Common/impl/DataMap_Unix.h` should have the following guard:

```c++
#ifndef COMMON_IMPL_DATAMAP_UNIX_H_
#define COMMON_IMPL_DATAMAP_UNIX_H_
…
#endif
```

### Names and Order of Includes

1. Corresponding header
2. C/C++ library
3. Third-party libraries
4. Rainbow headers

Within each section, order the includes alphabetically. The paths to Rainbow's
header files must always be descendants of `src/`. Don't use relative paths, or
paths with `.` or `..`.

For example, see `src/ThirdParty/Spine/spine-rainbow.cpp`:

```c++
#include "ThirdParty/Spine/spine-rainbow.h"

#include <algorithm>

#include <spine/SkeletonJson.h>
#include <spine/extension.h>

#include "Common/DataMap.h"
#include "FileSystem/Path.h"
#include "Graphics/Renderer.h"
#include "Graphics/SpriteVertex.h"
#include "Graphics/TextureAtlas.h"
```

### Forward Declarations

Use forward declarations whenever possible to avoid unnecessary `#include`s.

## Classes

* Always initialise members in the constructor using an initialisation list.
* Use the `explicit` keyword for constructors callable with one argument.
* Prefer composition over inheritance.
* Data members should be private. If access to them are needed, define accessors
  and mutators.

### Declaration Order

Declare public members first, followed by protected, and finally private. Within
each section, use the following order:

1. `using`s, enums, and inner classes/structs
2. Constants (static const data members)
3. Static methods
4. Data members (except static const data members)
5. Constructors
6. Destructor
7. Accessors and mutators
8. Methods
9. Event handlers
10. Operator overloads
11. Overrides

## Naming Conventions

* File names are **PascalCased** and suffixed `.cpp` or `.h` for C++ files.
  * Corresponding test files are suffixed `.test.cc`.
* Class names are **PascalCased**.
  * Pure interfaces must have an `I` prefixed.
* Variable names are **snake_cased**.
  * Member variables have a trailing underscore, e.g. `a_local_variable_`.
  * Struct members don't have a trailing underscore.
* Constant names are **PascalCased** and are prefixed with `k`.
* Function names are **snake_cased**.
  * Member accessors are named after the backing variable without the trailing
    underscore, e.g. `int status() const { return status_; }`.
  * Member setters are named after the backing variable with `set_` prefixed,
    e.g. `void set_status(int status) { status_ = status; }`.
* Namespace names are **snake_cased**.
* Enumerator names are **PascalCased**.
* Macro names are all **UPPER_CASE_AND_UNDERSCORES**.

## Formatting

Use the provided `.clang-format` configuration to format any written code. There
should be a [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html) plugin
for your favourite editor:

* Sublime Text: [Clang Format](https://github.com/rosshemsley/SublimeClangFormat)
* Vim: [vim-clang-format](https://github.com/rhysd/vim-clang-format)
* Visual Studio: [clang-format plugin](https://llvm.org/builds/)
* Visual Studio Code: [C/C++ for Visual Studio Code](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)

## Using Linters

You can tell CMake to run **clang-tidy** as part of the build process by passing
`-DCMAKE_CXX_CLANG_TIDY=/path/to/clang-tidy` as a parameter to the build script.

## Other Guidelines

* [Google C++ Style Guide](https://google-styleguide.googlecode.com/svn/trunk/cppguide.html)
* [LLVM Coding Standards](https://llvm.org/docs/CodingStandards.html)
* [Unreal Engine Coding Standard](https://docs.unrealengine.com/latest/INT/Programming/Development/CodingStandard/)
