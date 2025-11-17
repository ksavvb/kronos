# Kronos Language Server Setup

Get TypeScript/Python-style IDE features for Kronos with real-time error checking and autocomplete.

## Quick Start (One Command)

```bash
./install_extension.sh
```

Then restart your editor and open any `.kr` file.

## What You Get

✅ **Syntax Highlighting** - Keywords, strings, numbers  
✅ **Real-time Errors** - Red squiggles under mistakes before running  
✅ **Autocomplete** - Keyword suggestions  
✅ **Diagnostics Panel** - See all errors at once  

## How It Works

```
You edit a .kr file
    ↓
Extension sends it to kronos-lsp
    ↓
LSP parses and checks for errors
    ↓
Errors appear as red squiggles
    ↓
Autocomplete suggestions pop up
```

## Installation Details

The `install_extension.sh` script will:

1. Build the LSP server (`kronos-lsp`) if needed
2. Install npm dependencies for the extension
3. Copy extension to VSCode/Cursor/Windsurf
4. Set everything up automatically

### Manual Installation

If you prefer to do it manually:

```bash
# 1. Build LSP server
make lsp

# 2. Install extension dependencies
cd vscode-extension
npm install

# 3. Copy extension to VSCode
mkdir -p ~/.vscode/extensions/kronos-lsp-0.1.0
cp -r . ~/.vscode/extensions/kronos-lsp-0.1.0/

# 4. Restart your editor
```

## Current LSP Capabilities

✅ **Working:**
- Syntax error detection (basic)
- Keyword autocomplete
- Initialize handshake
- Document synchronization

🔄 **Planned:**
- Detailed error messages with line/column
- Variable name completion
- Function signature help
- Hover information
- Go to definition
- Rename refactoring

## Testing the LSP

### Quick Test

```bash
./test_lsp_manually.sh
```

### Manual Test

```bash
# Send raw JSON-RPC to LSP
echo '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{}}' | ./kronos-lsp
```

### Create a Test File

Create `test_lsp.kr`:

```kronos
set x to 10
print x

# Add a syntax error:
set y to
```

Open in VSCode - you should see a red squiggle under the incomplete statement.

## Troubleshooting

### Extension Not Loading?

**Check Output Panel:**
- **View → Output**
- Select **"Kronos Language Server"** from dropdown
- Look for error messages

### No Syntax Highlighting?

1. Check file extension is `.kr`
2. Check bottom-right corner - should say "Kronos"
3. Try: **Cmd+Shift+P** → "Change Language Mode" → "Kronos"

### LSP Server Not Starting?

```bash
# Check binary exists
ls -lh kronos-lsp

# Check permissions
chmod +x kronos-lsp

# Test manually
./kronos-lsp
# (Should wait for JSON-RPC input)
```

### Syntax Highlighting Works But No LSP?

That's normal! Syntax highlighting works independently:
- **Syntax highlighting** = `.vscode/kronos.tmLanguage.json` (always works)
- **LSP features** = Requires `kronos-lsp` + extension setup

### Still Having Issues?

Verify these files exist:

```bash
# LSP binary
ls -lh kronos-lsp

# Extension installed
ls -la ~/.vscode/extensions/kronos-lsp-*/

# Or for Cursor:
ls -la ~/.cursor/extensions/kronos-lsp-*/

# Or for Windsurf:
ls -la ~/.windsurf/extensions/kronos-lsp-*/
```

## Debugging the LSP

### Add Logging

Edit `src/lsp/lsp_server.c` and add:

```c
fprintf(stderr, "LSP: Received message: %s\n", buffer);
```

Rebuild:

```bash
make lsp
```

### View Logs in VSCode

LSP logs appear in: **Output → Kronos Language Server**

### Check Process

```bash
# See if LSP is running
ps aux | grep kronos-lsp
```

## Architecture

```
Your Editor (VSCode/Cursor/Windsurf)
    ↓
Extension Client (extension.js)
    ↓ JSON-RPC 2.0 over stdio
LSP Server (kronos-lsp binary)
    ↓
Kronos Parser (tokenize → parse)
    ↓
Diagnostics/Completion Results
    ↓ JSON-RPC 2.0 response
Back to Editor
```

## LSP Protocol Implementation

The LSP server (`src/lsp/lsp_server.c`) implements:

| Method | Status | Purpose |
|--------|--------|---------|
| `initialize` | ✅ Complete | Handshake with client |
| `shutdown` | ✅ Complete | Clean shutdown |
| `textDocument/didOpen` | ✅ Complete | File opened |
| `textDocument/didChange` | ✅ Complete | File edited |
| `textDocument/completion` | ✅ Basic | Autocomplete |
| `textDocument/hover` | ⏳ Planned | Hover info |
| `textDocument/definition` | ⏳ Planned | Go to definition |
| `textDocument/references` | ⏳ Planned | Find references |
| `textDocument/rename` | ⏳ Planned | Rename symbol |

## Verification Checklist

After installation, verify everything works:

- [ ] `make lsp` builds successfully
- [ ] Extension appears in editor extensions list
- [ ] Syntax highlighting works in `.kr` files
- [ ] Bottom-right shows "Kronos" for `.kr` files
- [ ] Output panel shows "Kronos Language Server"
- [ ] Red squiggles appear on syntax errors
- [ ] Autocomplete triggers with spacebar

## Advanced: Packaging the Extension

To create a `.vsix` package:

```bash
cd vscode-extension
npm install -g @vscode/vsce
vsce package
```

This creates `kronos-lsp-0.1.0.vsix` that can be shared.

Install the package:

```bash
code --install-extension kronos-lsp-0.1.0.vsix
```

See [PACKAGING.md](PACKAGING.md) for details.

## Contributing

Want to improve the LSP?

**Easy wins:**
- Add more keywords to autocomplete
- Improve error messages
- Add more diagnostic checks

**Advanced features:**
- Implement hover information
- Add go to definition
- Implement find references
- Add rename refactoring

See `src/lsp/lsp_server.c` to get started!
