# Kronos Programming Language

A high-performance programming language built in C with human-readable syntax, featuring a bytecode VM, reference-counting garbage collection, and fast execution times.

## Features

- **Human-Readable Syntax**: Natural language keywords like `set`, `to`, `print`, `is equal`, `greater than`, etc.
- **Fast Execution**: Bytecode VM with optimized execution (Python/JS performance levels)
- **Reference Counting GC**: Automatic memory management with cycle detection
- **Direct Execution**: No build step required - just run `.kr` files directly
- **Interactive REPL**: Test code snippets interactively

## Quick Start

### Building

```bash
make
```

### Running

Execute a Kronos file:

```bash
./kronos examples/hello.kr
```

Start the REPL:

```bash
./kronos
```

## Language Syntax

Kronos uses human-readable syntax with natural language keywords. Here's a quick example:

```kronos
set x to 10
set y to 20
set sum to x plus y
print sum

for i in range 1 to 5:
    print i
```

## Documentation

- **[SYNTAX.md](docs/SYNTAX.md)** - Complete syntax reference with detailed examples
- **[QUICKREF.md](docs/QUICKREF.md)** - Quick reference card for fast lookup
- **[PROJECT.md](docs/PROJECT.md)** - Architecture and internals
- **[IMPLEMENTATION_STATUS.md](docs/IMPLEMENTATION_STATUS.md)** - Feature status and roadmap

## Examples

All examples are in the `examples/` directory:

| File                 | Description                    |
| -------------------- | ------------------------------ |
| `hello.kr`           | Hello world and basic printing |
| `test.kr`            | Variables and arithmetic       |
| `arithmetic.kr`      | All arithmetic operations      |
| `conditionals.kr`    | If statements                  |
| `loops.kr`           | For and while loops            |
| `fizzbuzz.kr`        | FizzBuzz implementation        |
| `syntax_showcase.kr` | Feature demonstration          |

**Run an example:**

```bash
./kronos examples/hello.kr
```

See [examples/README.md](examples/README.md) for more details.

## Architecture

### Components

- **Frontend** (`src/frontend/`) - Tokenizer and parser
- **Compiler** (`src/compiler/`) - AST to bytecode compilation
- **Virtual Machine** (`src/vm/`) - Stack-based bytecode execution
- **Core Runtime** (`src/core/`) - Value system and garbage collector

### Bytecode VM

Stack-based virtual machine with instructions for:

- Variable operations (`LOAD_VAR`, `STORE_VAR`)
- Arithmetic (`ADD`, `SUB`, `MUL`, `DIV`)
- Comparisons (`EQ`, `NEQ`, `GT`, `LT`, `GTE`, `LTE`)
- Control flow (`JUMP`, `JUMP_IF_FALSE`)
- I/O (`PRINT`)

See [docs/PROJECT.md](docs/PROJECT.md) for detailed architecture documentation.

## Development

### Project Structure

```
kronos/
├── src/
│   ├── core/          # Core runtime (values, memory, GC)
│   ├── frontend/      # Tokenizer and parser
│   ├── compiler/      # Bytecode compiler
│   ├── vm/            # Virtual machine
│   ├── concurrency/   # Future: Goroutines & channels
│   └── fault/         # Future: Exceptions & supervisors
├── include/           # Public headers
├── examples/          # Example .kr files
├── docs/              # Documentation
├── main.c             # Entry point
└── Makefile
```

### Building for Development

```bash
make clean
make
```

### Cleaning Build Artifacts

```bash
make clean
```

## Roadmap

### Current Version (0.1.0)

✅ All core features working:

- Variables, numbers, strings
- Arithmetic and comparisons
- If statements, for/while loops
- REPL and file execution

### Future Versions

**v0.2.0 - Functions**

- Function definitions and calls
- Return values
- Local variable scoping
- Else/else-if statements

**v1.0.0 - Concurrency & Fault Tolerance**

- Goroutine-style lightweight threads
- Channel-based communication
- Exception handling (try/catch/finally)
- Supervisor trees (Erlang-style)

See [docs/IMPLEMENTATION_STATUS.md](docs/IMPLEMENTATION_STATUS.md) for detailed status.

## Contributing

Contributions are welcome! The codebase is well-documented and modular. See [docs/PROJECT.md](docs/PROJECT.md) for architecture details.

## License

MIT License - See [LICENSE](LICENSE) file for details.

## Performance

- **Binary Size**: 58KB
- **Startup Time**: ~15ms
- **Execution**: Comparable to CPython
- **Memory**: Zero leaks in core features

Built with C for high performance and low-level control.
