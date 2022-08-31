# bctl

Display backlight control.

# Usage

```bash
# Get the brightness level.
bctl

# Set the brightness level.
bctl <integer>

# Increase the brightness level.
bctl +<integer>

# Decrease the brightness level.
bctl -<integer>
```

# Building

Just build the project with:

```bash
cc main.c -o bctl
```