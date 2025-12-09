# Glad GL bindings generation

To dynamically load OpenGL functions, we can generate bindings with [GLAD](https://github.com/Dav1dde/glad).

Install GLAD2:

```bash
sudo dnf install glad2
```

The bindings are generated with the following command:

```bash
glad --api='gles2=3.0' --extensions='' --out-path='./' c --debug
```

Note that GLAD will automatically download the latest API specifications,
so even though the API is stable, the generated code might not be reproducible.
