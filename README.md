# ALI
A simple C++ memory editor that uses Lua as it's scripting language

Usage: `ALI <process name> <script file name>`

Custom Lua functions:
```lua
(void) memory.write(<address>, <value>)
```

```lua
(int) memory.read(<address>)
```
