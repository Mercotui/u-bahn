# U-Bahn Blender Exporter Addon

This directory contains a Blender Addon that can export world sections in the format the U-Bahn project requires.

## Usage

## Development

To work on this addon you should use the poetry manifest to install the dependencies.
Assuming you have pipx you can install poetry and this project using:

```bash
pipx install poetry
poetry install
```

To package the extension you can run the script:

```bash
./package_addon.sh
```

If you're running blender as a flatpak, then package can then be installed using
the [CLI](https://docs.blender.org/manual/en/4.4/advanced/command_line/extension_arguments.html#subcommand-build):

```bash
./package_addon.sh && flatpak run org.blender.Blender --command extension install-file --repo user_default --enable dist/ubahn_exporter_blender_addon-0.1.0-linux_x64.zip
```
