import re
from typing import Any, Dict

class Tokenizer:
    def __init__(self, code: str):
        self.tokens = []
        self.tokenize(code)
        self.pos = 0

    def tokenize(self, code: str):
        patterns = [
            ('NUMBER', r'\d+(\.\d+)?'),
            ('STRING', r'"[^"]*"'),
            ('SET', r'\bset\b'),
            ('TO', r'\bto\b'),
            ('IF', r'\bif\b'),
            ('IS', r'\bis\b'),
            ('EQUAL', r'\bequal\b'),
            ('NOT', r'\bnot\b'),
            ('GREATER', r'\bgreater\b'),
            ('LESS', r'\bless\b'),
            ('THAN', r'\bthan\b'),
            ('PRINT', r'\bprint\b'),
            ('NAME', r'[a-zA-Z_]\w*'),
            ('COLON', r':'),
            ('NEWLINE', r'\n'),
            ('INDENT', r'^[ \t]+'),
            ('SKIP', r'[ \t]+'),
        ]

        lines = code.split("\n")
        for line in lines:
            col = 0
            # Detect indentation
            indent = len(line) - len(line.lstrip())
            if line.strip():
                self.tokens.append(("INDENT", indent))

            line = line.strip()
            while col < len(line):
                matched = False
                for token_type, pattern in patterns:
                    regex = re.compile(pattern)
                    match = regex.match(line, col)
                    if match:
                        text = match.group(0)
                        if token_type != "SKIP":
                            self.tokens.append((token_type, text))
                        col = match.end()
                        matched = True
                        break
                if not matched:
                    col += 1

            if line:
                self.tokens.append(("NEWLINE", "\n"))

class Parser:
    def __init__(self, tokens):
        self.tokens = tokens
        self.pos = 0

    def peek(self):
        return self.tokens[self.pos] if self.pos < len(self.tokens) else None

    def consume(self, expected=None):
        token = self.tokens[self.pos]
        if expected and token[0] != expected:
            raise SyntaxError(f"Expected {expected}, got {token[0]}")
        self.pos += 1
        return token

    def parse(self):
        statements = []
        while self.pos < len(self.tokens):
            if self.peek()[0] == "NEWLINE":
                self.consume()
                continue
            stmt = self.parse_statement()
            if stmt:
                statements.append(stmt)
        return statements

    def parse_statement(self):
        indent = 0
        if self.peek()[0] == 'INDENT':
            indent = int(self.peek()[1])
            self.consume()
        
        token = self.peek()
        if token[0] == 'SET':
            return self.parse_assignment(indent)
        elif token[0] == 'PRINT':
            return self.parse_print(indent)
        elif token[0] == 'IF':
            return self.parse_if(indent)
        return None
    
    def parse_assignment(self, indent):
        self.consume('SET')
        name = self.consume('NAME')[1]
        self.consume('TO')
        value = self.parse_value()
        self.consume('NEWLINE')
        return ('assign', indent, name, value)
    
    def parse_print(self, indent):
        self.consume('PRINT')
        value = self.parse_value()
        self.consume('NEWLINE')
        return ('print', indent, value)
    
    def parse_if(self, indent):
        self.consume('IF')
        left = self.parse_value()
        self.consume('IS')
        
        # Handle "not equal" vs "equal"
        negated = False
        if self.peek()[0] == 'NOT':
            self.consume('NOT')
            negated = True
        
        op_token = self.peek()[0]
        if op_token == 'EQUAL':
            self.consume('EQUAL')
            op = '!=' if negated else '=='
        elif op_token == 'GREATER':
            self.consume('GREATER')
            self.consume('THAN')
            op = '<=' if negated else '>'
        elif op_token == 'LESS':
            self.consume('LESS')
            self.consume('THAN')
            op = '>=' if negated else '<'
        
        right = self.parse_value()
        self.consume('COLON')
        self.consume('NEWLINE')
        
        # Parse block
        block = []
        block_indent = indent + 4
        while self.pos < len(self.tokens):
            if self.peek()[0] != 'INDENT':
                break
            next_indent = int(self.peek()[1])
            if next_indent <= indent:
                break
            stmt = self.parse_statement()
            if stmt:
                block.append(stmt)
        
        return ('if', indent, left, op, right, block)
    
    def parse_value(self):
        token = self.peek()
        if token[0] == 'NUMBER':
            self.consume()
            return ('number', float(token[1]))
        elif token[0] == 'STRING':
            self.consume()
            return ('string', token[1].strip('"'))
        elif token[0] == 'NAME':
            self.consume()
            return ('var', token[1])
        raise SyntaxError(f"Unexpected token: {token}")

class Interpreter:
    def __init__(self):
        self.variables: Dict[str, Any] = {}
    
    def run(self, statements):
        for stmt in statements:
            self.execute(stmt)
    
    def execute(self, stmt):
        stmt_type = stmt[0]
        
        if stmt_type == 'assign':
            _, _, name, value = stmt
            self.variables[name] = self.eval_value(value)
        
        elif stmt_type == 'print':
            _, _, value = stmt
            result = self.eval_value(value)
            print(result)
        
        elif stmt_type == 'if':
            _, _, left, op, right, block = stmt
            left_val = self.eval_value(left)
            right_val = self.eval_value(right)
            
            condition = False
            if op == '==':
                condition = left_val == right_val
            elif op == '!=':
                condition = left_val != right_val
            elif op == '>':
                condition = left_val > right_val
            elif op == '<':
                condition = left_val < right_val
            elif op == '>=':
                condition = left_val >= right_val
            elif op == '<=':
                condition = left_val <= right_val
            
            if condition:
                for block_stmt in block:
                    self.execute(block_stmt)
    
    def eval_value(self, value):
        val_type, val = value
        if val_type == 'number':
            return val
        elif val_type == 'string':
            return val
        elif val_type == 'var':
            if val not in self.variables:
                raise NameError(f"Variable '{val}' not defined")
            return self.variables[val]

# Example usage
code = """
set x to 10
set y to 10

if x is equal y:
    print "x equals y!"

if x is not equal 5:
    print "x is not 5"

set age to 25

if age is greater than 18:
    print "Adult"

if age is less than 30:
    print "Young adult"
"""

tokenizer = Tokenizer(code)
parser = Parser(tokenizer.tokens)
ast = parser.parse()
interpreter = Interpreter()
interpreter.run(ast)