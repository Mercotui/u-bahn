# U-Bahn

U-Bahn game

See [docs/building.md](docs/building.md) for build instructions for each supported platform.

## World file editing

For editing the protobuf level files and configuration files, I recommend to
use [protobufpal](https://www.protobufpal.com/), but don't go pasting your secrets in there.

To store the encoded binary data in a file use a command such as:

```bash
echo "your binary data here" | xxd -r -p > resources/test-track.bin
```
