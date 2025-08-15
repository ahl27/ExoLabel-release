# ExoLabel (commandline C distribution)

This is the standalone ExoLabel distribution that can be run from the commandline.

## Compiling

To compile ExoLabel for your machine, first clone the repository with included submodules:

```sh
git clone --recurse-submodules https://github.com/ahl27/ExoLabel-release.git
```

Then enter the directory and run `make`:
```sh
cd ExoLabel-release
make
```

This should compile an executable called `exolabel`. To view the man page:
```sh
./exolabel --help
```

Note that `ExoLabel` requires both `unistd.h` and `zlib.h`. This distribution is not tested on nor guaranteed to work on Windows. Windows users are encouraged to run ExoLabel via its R distribution.

I'm happy to accept a PR with changes that enable this version of ExoLabel to run on Windows.

## Running ExoLabel

This is **IN-PROGRESS** and may not yet work.

Help message:
```
Usage: ExoLabel [input_files] [-adhiIlosStuV]
Additional optional arguments:
  Flags:
    -d (--directed): treat input edgelist files as directed.
    -h (--help): display this message.
    -I (--inplace-sort): use in-place external merge sort.
    -S (--space-separated): read space-separated input instead of TSVs.
    -u (--unweighted): treat input edgelist files as unweighted.
    -V (--silent): suppress all output.

  Required parameter args:
    -i (--iterations) [integer]: set iterations to value.
    -o (--output) [filename]: set name of output file.
    -t (--tempdir) [dirname]: set directory to be used for temp files.

  Optional parameter args:
    -a (--attenuate) [optional float]: set attenuation to value
    -l (--self-loop) [optional float]: set self-loops to value.
    -s (--skip-header-lines) [optional integer]: skip header lines in file.

  Optional parameter args treat missing arguments as zero, e.g.:
   `ExoLabel [input files]`                 -> attenuation disabled
   `ExoLabel [input files] --attenuate`     -> attenuation is 1.0
   `ExoLabel [input files] --attenuate=2.0` -> attenuation is 2.0

Invalid values (e.g., --self-loop=xyz) default to zero.

Default configuration (i.e., `ExoLabel [input_files]`):
  ExoLabel [input_files] \
    --iterations=0 --output=ExoLabel_result \
    --tempdir=. --attenuate=0 --self-loop=0 --skip-header-lines=0
  undirected networks, no in-place sort, tab-separated files,
  weighted networks, all normal output displayed
```