#!/bin/bash

if [ $# -lt 1 ]; then
    echo "Please: $0 archivo1.png [archivo2.png ...]"
    exit 1
fi

OUTPUT_DIR="assets/embedded"
mkdir -p "$OUTPUT_DIR"

for file in "$@"; do
    if [[ "$file" != *.png ]]; then
        echo "Omiting '$file': not a PNG file."
        continue
    fi

    base=$(basename "$file")
    name="${base//[^a-zA-Z0-9]/_}"  # C/C++ naming style
    name="${name,,}"                # lowercase

    output="${OUTPUT_DIR}/${name}.h"

    echo "Creating $output..."
    xxd -i "$file" > "$output"
done

echo "All ready, check '$OUTPUT_DIR'"
