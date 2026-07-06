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

# Verify optional parameter -a works without argument (should match --attenuate)
echo "  Testing -a optional parameter..."
$EXOLABEL tests/test1.tsv -r 123 -a -o temp_a -V
$EXOLABEL tests/test1.tsv -r 123 --attenuate -o temp_atten -V
if ! cmp -s temp_a temp_atten; then
  echo "  failed -a option comparison!"
  rm -f temp_a temp_atten
  exit 1
fi
rm -f temp_a temp_atten

# Verify that 2-column missing weight warning is printed to stdout when not silent
echo "  Testing 2-column format warning..."
printf "A\tB\nB\tC\nC\tA\n" > temp_2col.tsv
warning_out=$($EXOLABEL temp_2col.tsv -r 123 -o temp_out 2>&1)
if ! echo "$warning_out" | grep -q "Warning: Missing weight column detected"; then
  echo "  failed to print missing weight warning!"
  rm -f temp_2col.tsv temp_out
  exit 1
fi
rm -f temp_2col.tsv temp_out

# Verify that self-loop warning on unweighted graphs is printed to stdout
echo "  Testing self-loop warning on unweighted graph..."
warning_out2=$($EXOLABEL tests/test1.tsv -u -l -o temp_out 2>&1)
if ! echo "$warning_out2" | grep -q "Warning: Specifying self-loops with unweighted graphs is discouraged"; then
  echo "  failed to print self-loop warning on unweighted graph!"
  rm -f temp_out
  exit 1
fi
rm -f temp_out

echo "All tests passed successfully!"