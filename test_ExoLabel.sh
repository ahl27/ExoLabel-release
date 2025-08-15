#!/bin/bash

EXOLABEL=./exolabel

if [ ! -f $EXOLABEL ]; then
  echo "ExoLabel has not been built! Run \`make\` before testing."
  exit 1
fi
echo "ExoLabel executable located."
echo "Running tests..."

for f in tests/*.tsv; do
  echo "  Testing on $f..."
  $EXOLABEL $f -r 123 -o tempfile -V
  cmp -s $f.answer tempfile || echo "failed"
  if ! cmp -s $f.answer tempfile; then
    echo "  test failed!"
    rm -f tempfile
    exit 1
  fi
  rm -f tempfile
done

echo "All tests passed successfully!"