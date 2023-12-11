# AST Parser

Simple program which parse and calculate with AST simple expressions.

#### Project Structure

In `src` is the sources, and in `ut` unit tests.

### Building The Project

```bash
❯ cmake .
❯ cmake --build .
```

Builded files can be found in src directory.

#### Running the tests

```bash
❯ src/test_ast
```

or

```bash
❯ cd src
❯ ctest -VV
```

Result of unit tests are also in files in Test directory.

#### Running the CLI Executable

```bash
Usage:
	parser_app <expresion>
```
```bash
❯ src/parser_app 2+2

Equation: 2+2 Result: 4
```
