# Packaging Kronos Extension for VSCode

## Current Status

✅ **Syntax highlighting works locally** - No packaging needed!

The `.vscode/` folder contains TextMate grammar that VSCode automatically loads for any project.

## Option 1: Keep Using Local Files (Recommended)

**Pros:**

- Already works
- No extra steps
- Easy to modify
- Just open the project folder in VSCode

**Cons:**

- Only works when the Kronos project is open
- Not available globally for all `.kr` files

## Option 2: Package as VSCode Extension (Future)

To make Kronos syntax highlighting available globally (even outside the project), you'd need to package it.

### Steps to Package:

```bash
# Install packaging tool
npm install -g @vscode/vsce

# Package the extension
vsce package

# This creates: kronos-language-0.1.0.vsix
```

### Install the Extension:

```bash
# Install locally
code --install-extension kronos-language-0.1.0.vsix

# Or publish to marketplace
vsce publish
```

### Required Files for Packaging:

- ✅ `package.json` - Extension manifest
- ✅ `.vscode/kronos.tmLanguage.json` - Grammar
- ✅ `language-configuration.json` - Editor config
- ⚠️ `README.md` - Would need extension-specific docs
- ⚠️ `LICENSE` - MIT license file
- ⚠️ Icon/screenshots - For marketplace

### Update `package.json` for Publishing:

```json
{
  "name": "kronos-language",
  "displayName": "Kronos Language Support",
  "description": "Syntax highlighting and language support for Kronos",
  "version": "0.1.0",
  "publisher": "your-publisher-name",
  "repository": {
    "type": "git",
    "url": "https://github.com/yourusername/kronos"
  },
  "engines": {
    "vscode": "^1.60.0"
  },
  "categories": ["Programming Languages"],
  "contributes": {
    "languages": [
      {
        "id": "kronos",
        "aliases": ["Kronos"],
        "extensions": [".kr"],
        "configuration": "./language-configuration.json"
      }
    ],
    "grammars": [
      {
        "language": "kronos",
        "scopeName": "source.kronos",
        "path": "./.vscode/kronos.tmLanguage.json"
      }
    ]
  }
}
```

## Option 3: Manual Installation

Copy grammar files to VSCode's extensions folder:

```bash
# macOS
mkdir -p ~/.vscode/extensions/kronos-language/
cp .vscode/kronos.tmLanguage.json ~/.vscode/extensions/kronos-language/
cp language-configuration.json ~/.vscode/extensions/kronos-language/
cp package.json ~/.vscode/extensions/kronos-language/

# Restart VSCode
```

## Recommendation

**For now:** Keep using local files (Option 1)

- Syntax highlighting already works
- No extra complexity
- Easy to iterate and improve

**Later:** Package as extension (Option 2)

- When you want to share Kronos
- When you want syntax highlighting everywhere
- When ready to publish to marketplace

## Current Status

The current `package.json` is:

- ✅ Valid for local development
- ⚠️ Incomplete for publishing
- ❓ Optional - Can be removed without breaking anything

**Syntax highlighting works regardless!**
