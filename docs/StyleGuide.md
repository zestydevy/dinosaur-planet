# Style Guide

The Dinosaur Planet decompilation style guide.

- [Naming](#naming)
    - [Global Functions (core code)](#global-functions-core-code)
    - [Global Functions (DLL code)](#global-functions-dll-code)
    - [Static Functions](#static-functions)
    - [Variables](#variables)
    - [Types](#types)
    - [Macros/Defines](#macrosdefines)
- [Indentation](#indentation)
- [Braces](#braces)
- [Pointers](#pointers)

## Naming

### Global Functions (core code)

`camelCase` with a prefix. The prefix should be short and identify the subsystem/file.

Examples:
- `objSetupObject`
- `texLoadTexture`
- `mainGetBits`

Official function names and prefixes should be used where possible. These will be in the same style. If an official name is particularly confusing, a custom name may be used instead.

### Global Functions (DLL code)

Exports:
`<prefix>[_subPrefix]_<FuncName>` where:
- `prefix` - A (generally) `camelCase` prefix that represents the name of the DLL (this is similar to core code global functions). Example: `amSfx`.
- `_subPrefix`- For exports that implement a nested interface (i.e. a more generic interface not specific to that DLL), the sub prefix is a `camelCase` prefix identifying that interface. Examples: `_obj`, `_vehicle`.
- `FuncName` - The rest of the function name in `PascalCase`.
- Examples:
    - `amSfx_Play` (self interface)
    - `BWlog_obj_Setup` (object interface)
    - `BWlog_vehicle_GetRacePosition` (vehicle interface)

Constructors/destructors:
`<prefix>_<ctor or dtor>`.
- `prefix` - The same prefix used by exports.
- Examples: `amSfx_ctor`, `amSfx_dtor`.

Global DLL functions are special as they represent DLL constructors, destructors, and exports. Export functions implement a DLL interface and as such the name of the interface should be included in the function name. In cases where an export implements a nested interface, an identifier for that interface is also included.

Official function names should be adapted to this format where possible. For example: `amSfxPlay` (official) -> `amSfx_Play`.

### Static Functions

`camelCase` *without* a prefix. Avoid starting the name with a word that may collide with the prefix of global functions (e.g. don't start a static function name with `obj`).

Note that for core code, it is generally not recommended to mark functions as static even if the function logically should be static. In those cases, the global function naming style should be preferred, unless the official function name is known.

### Variables

- Global variable: `gCamelCase`
- Static variable: `sCamelCase`
- Local variable: `camelCase`

### Types

- Struct: `PascalCase`
- Enum: `PascalCase`

#### DLL Interface Structs
- Struct name: `DLL_<NamePascalName>`
    - For generic interfaces, it's recommended to prefix the name with `I` (e.g. `DLL_IVehicle`).
- Struct fields: `PascalCase` (these should be the export function names without any prefixes).
    - Example: `amSfx_Play` (export name), `Play` (interface struct field name)

### Macros/Defines

`SCREAMING_SNAKE_CASE`, with one exception. Macros that are meant to be used as function calls may follow the global function naming style.

DLL vtable macros:
`dll_<prefix>`/`dll_<prefix>_<FuncName>`.


## Indentation

4 spaces, no tabs.

When wrapping a line, use an extra indent:
```c
function(arg0, arg1, arg2,
    arg3, arg4,
    arg5, arg6);
```

Switch cases should not be indented:
```c
switch (x) {
case 0:
    break;
default:
    break;
}
```

## Braces

Starting braces should be on the same line. Ending braces should be on a new line by itself (except when followed by an `else`).

Example:
```c
if (cond) {
    // ...
} else if (cond2) {
    // ...
} else {
    // ...
}
```

This also applies to functions:
```c
void function(void) {
    // ...
}
```

Single statement blocks should always have braces (never omit them):
```c
if (cond) {
    a = 0;
} else {
    b = 1;
}
```

## Pointers

The pointer `*` should be declared on the type, not the variable:
```c
s32* var;
u8* function(void* arg0);
```

**Do not** declare multiple pointers on the same line:
```c
s32* ptr1, *ptr2; // BAD!
```

## Whitespace

NOTE: Matching takes precedence over style with regards to whitespace. Whitespace *can* affect a match.

- Do not put more than one statement on the same line (except if required to match).
- Use Unix line endings (`\n`).
- Ensure files end with a newline.
- Strip trailing whitespace.

