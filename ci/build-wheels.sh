#!/bin/bash
set -e -x

# nanobind split-mode stable ABI wheels only need one build per platform
# (the wheel itself is tagged cp310-abi3 and works on Python >= 3.10).
python -m pip install cibuildwheel twine
cibuildwheel --output-dir wheelhouse

ls wheelhouse/

# Upload
twine upload \
    --skip-existing \
    -u "${TWINE_USERNAME}" \
    -p "${TWINE_PASSWORD}" \
    wheelhouse/*.whl
