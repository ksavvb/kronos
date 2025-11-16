# Kronos Documentation

Complete documentation for the Kronos programming language.

## Documentation Files

### For Users

- **[SYNTAX.md](SYNTAX.md)** - Complete language syntax reference

  - Variables, data types, operators
  - Control flow structures
  - Examples and best practices
  - 600+ lines of detailed documentation

- **[QUICKREF.md](QUICKREF.md)** - Quick reference card
  - Fast syntax lookup
  - Common patterns
  - Cheat sheet format

### For Developers

- **[PROJECT.md](PROJECT.md)** - Architecture and internals

  - System design
  - Component breakdown
  - VM architecture
  - Performance details

- **[IMPLEMENTATION_STATUS.md](IMPLEMENTATION_STATUS.md)** - Implementation status
  - Feature completion status
  - Roadmap and plans
  - Known limitations
  - Performance metrics

## Quick Links

### Getting Started

- [Main README](../README.md) - Project overview
- [Examples](../examples/README.md) - Sample programs

### Building & Running

```bash
# Build
make

# Run a program
./kronos examples/hello.kr

# Start REPL
./kronos
```

## Documentation Organization

```
kronos/
├── README.md              # Main project readme (you are here reference)
├── LICENSE                # MIT License
├── docs/                  # All documentation
│   ├── README.md         # This file
│   ├── SYNTAX.md         # Language reference
│   ├── QUICKREF.md       # Quick reference
│   ├── PROJECT.md        # Architecture
│   └── IMPLEMENTATION_STATUS.md  # Status & roadmap
├── examples/             # Example programs
│   ├── README.md         # Example guide
│   └── *.kr              # Kronos programs
└── src/                  # Source code
    ├── core/             # Runtime & GC
    ├── frontend/         # Tokenizer & parser
    ├── compiler/         # Bytecode compiler
    └── vm/               # Virtual machine
```

## Contributing to Documentation

When updating documentation:

1. Keep examples practical and tested
2. Update cross-references if moving files
3. Maintain consistent formatting
4. Add to this index when creating new docs

---

_For the latest information, see the individual documentation files._
