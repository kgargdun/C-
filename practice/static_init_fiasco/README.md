Static Initialization Order Fiasco

This small demo shows how the order of static object initialization across translation units can lead to undefined behavior.

Build and run:

```sh
cd static_init_fiasco
make
./static_init_demo
```

You may see different output or an empty string when a global in one translation unit is used by a global in another translation unit before it's constructed.
