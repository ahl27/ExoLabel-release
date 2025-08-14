# ExoLabel (commandline C distribution)

This is the standalone ExoLabel distribution that can be run from the commandline. Full documentation is coming soon. In the meantime, the help file is included below.

This is **IN-PROGRESS** and will not yet work.

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

Invalid values (e.g., --self-loop=xyz) default to zero.\n
Default configuration (i.e., `ExoLabel [input_files]`):
ExoLabel [input_files] \
  --iterations=0 --output=ExoLabel_result \
  --tempdir=. --attenuate=0 --self-loop=0 --skip-header-lines=0
undirected networks, no in-place sort, tab-separated files,
weighted networks, all normal output displaye
```