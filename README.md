# GTK 3 desktop application

### Compile the GTK-3 code:

```
gcc $(pkg-config --cflags gtk+-3.0) -o main main.c $(pkg-config --libs gtk+-3.0)
```

### Run the compiled file:

```
./main
```
