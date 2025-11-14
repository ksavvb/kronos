# Kronos Programming Language

A high-performance programming language built in C with human-readable syntax, featuring a bytecode VM, reference-counting garbage collection, and fast execution times.

## Features

- **Human-Readable Syntax**: Natural language keywords like `set`, `to`, `print`, `is equal`, `greater than`, etc.
- **Fast Execution**: Bytecode VM with optimized execution (Python/JS performance levels)
- **Reference Counting GC**: Automatic memory management with cycle detection
- **Direct Execution**: No build step required - just run `.kr` files directly
- **Interactive REPL**: Test code snippets interactively

## Building

```bash
make
```

## Running

Execute a Kronos file:

```bash
./kronos examples/test.kr
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

**For complete syntax documentation, see:**

- [SYNTAX.md](SYNTAX.md) - Comprehensive syntax reference with detailed examples
- [QUICKREF.md](QUICKREF.md) - Quick reference card for fast lookup

## Architecture

### Components

- **Frontend** (`src/frontend/`)

  - Tokenizer: Lexical analysis
  - Parser: Syntax analysis and AST building

- **Compiler** (`src/compiler/`)

  - Bytecode compiler: Converts AST to bytecode

- **Virtual Machine** (`src/vm/`)

  - Stack-based VM: Executes bytecode

- **Core Runtime** (`src/core/`)
  - Value system: Dynamic typing with reference counting
  - Garbage collector: Memory management

### Bytecode Instructions

- `LOAD_CONST`: Load constant from pool
- `LOAD_VAR`: Load variable value
- `STORE_VAR`: Store variable value
- `PRINT`: Print value
- `ADD`, `SUB`, `MUL`, `DIV`: Arithmetic operations
- `EQ`, `NEQ`, `GT`, `LT`, `GTE`, `LTE`: Comparisons
- `JUMP`, `JUMP_IF_FALSE`: Control flow
- `HALT`: Stop execution

## Examples

See the `examples/` directory for sample programs:

- `hello.kr` - Hello world and basic printing
- `test.kr` - Basic variable and print operations
- `arithmetic.kr` - Arithmetic operation examples
- `conditionals.kr` - If statement examples
- `loops.kr` - For and while loop examples
- `syntax_showcase.kr` - Comprehensive feature demonstration
- `fizzbuzz.kr` - FizzBuzz implementation

**Run an example:**

```bash
./kronos examples/hello.kr
```

**For more details, see [examples/README.md](examples/README.md)**

## Future Roadmap

### Phase 5: Concurrency

- Goroutine-style lightweight threads
- Channel-based communication
- Cooperative multitasking scheduler

### Phase 6: Fault Tolerance

- Exception handling (try/catch/finally)
- Supervisor trees (Erlang-style)
- Process monitoring and restart

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

## Documentation

- **[README.md](README.md)** - This file (overview and quick start)
- **[SYNTAX.md](SYNTAX.md)** - Complete syntax reference (600+ lines)
- **[QUICKREF.md](QUICKREF.md)** - Quick reference card
- **[PROJECT.md](PROJECT.md)** - Architecture and internals
- **[examples/README.md](examples/README.md)** - Example programs guide

## License

MIT License - See [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! The codebase is well-documented and modular. See [PROJECT.md](PROJECT.md) for architecture details.

## Author

Built with C for high performance and low-level control.
