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

You can test its behavior by entering the ExoLabel-release folder and running:
```sh
./test_ExoLabel.sh
```

Note that included tests are currently fairly barebones, and are just checking for consistency between the results obtained on one trial on an OSX machine vs. the same data with the same set seed. Machine-specific differences in random number generation may lead to different results.

ExoLabel requires both `unistd.h` and `zlib.h`. This distribution is not tested on nor guaranteed to work on Windows. Windows users are encouraged to run ExoLabel via its R distribution.

I'm happy to accept a PR with changes that enable this version of ExoLabel to run on Windows.

## Running ExoLabel

The basic usage pattern of ExoLabel is:

```sh
exolabel [input edgelist files] [-adhiIlosStuV]
```

`[input edgelist files]` is a set of files to analyze. This can be one or more files, including a wildcard expansion (e.g., `./folder/*.tsv`). Each file is expected to contain a set of lines, where each individual line denotes a single edge in the network.

The default parameterization expects input files to be tab-separated values. Each line is expected to contain three values, corresponding to the start vertex, end vertex, and edge weight (e.g., `abc def 0.45`). Each edge is treated as an undirected edge.

Space-separated values, directed edges, and two-column / unweighted edges are also accepted by passing certain flags; see below for more detail.


### Additional Arguments

Additional arguments can be passed in either short form or long form. For example, `-h (--help)` is an argument that can either be specified as `exolabel -h` or as `exolabel --help`.

#### Flags

Flag arguments do not accept a value, they just change the behavior of ExoLabel when passed.

* `-d (--directed)`: Treat edges as directed edges (rather than the default of undirected). Directed edges are specified as `start end weight`.
* `-h (--help)`: display details on how to use ExoLabel.
* `-I (--inplace-sort)`: Uses a inplace variant of external merge sort for internal operations. This is much slower than the the default, but consumes half as much disk space.
* `-S (--space-separated)`: Reads input files as space-separated rather than the default of tab-separated.
* `-u (--unweighted)`: Treats input edgelist files as unweighted, rather than the default of weighted. This will ignore any values past the second column in input edgelist files. If enabled, input edgelist files need only contain two columns, and all edge weights are set to 1.0 internally.
* `-V (--silent)`: suppresses all printed messages.

#### Required parameter arguments

Required parameter arguments **must** be provided an additional value. This value can be provided in the following formats:
```
exolabel -r10
exolabel -r 10
exolabel --random-seed=10
exolabel --random-seed10
```

All of the above pass the value `10` to the `random-seed` parameter. Note that long-form arguments **cannot** have a space between their values (i.e., `exolabel --random-seed 10` will not properly read the value). Use either an equals sign or the short-form equivalent argument.

Note that invalid arguments (e.g., `--random-seed=abc`) will be treated as 0.

* `-i (--iterations) [integer]`: Set value for `iterations`, the maximum times to visit each node. The default value of 0 will automatically determine an appropriate value.
* `-o (--output) [filename]`: Set the name of the file results will be written to. By default, results are written to `ExoLabel_result.tsv` (or `.txt` if using space-separated files).
* `-r (--random-seed) [integer]`: Set seed for the random number generator for reproducible results. If unspecified, defaults to `time(NULL)`.
* `-t (--tempdir) [dirname]`: Set the directory to be used for placing temporary files while ExoLabel is running.

#### Optional parameter arguments

Optional parameter arguments work as either a flag or a parameterized argument. Specifying these arguments without a value is equivalent to passing the value 1.0. For example:

```
exolabel [input files]                 -> attenuation disabled
exolabel [input files] --attenuate     -> attenuation is 1.0
exolabel [input files] --attenuate=2.0 -> attenuation is 2.0
```

As with required parameter arguments, note that invalid arguments (e.g., `--random-seed=abc`) will be treated as 0.

* `-a (--attenuate) [optional float]`: Enables attenuation, which decreases the contribution of a node to determining the next node's community the further the node is from its initial source. This reduces the ability of a single "monster group" to dominate results. Defaults to disabled, sets to enabled (1.0) if passed as a flag. If given a parameter, specifies the strength of attenuation. Higher values create smaller communities, whereas smaller values create larger communities.
* `-l (--self-loop) [optional float]`: Enables self-loops, potentially setting them to a specific value if provided. Self-loops are used as a cutoff value rather than a genuine self-loop. This means that, in order for a node to be assigned to a community, at least one of its incoming edges must be at least the value of `self-loop`. For example, suppose node A has 4 incoming edges -- 3 from community 1, each with weight 0.2, and one from community 2 with weight 0.5. If self-loops are disabled, node A will be assigned to community 1, since the total weight of community 1 is 0.6. However, if self-loops are set to 0.5, node A will be assigned to community 2, since no edge from community 1 is at least 0.5 (and at least one from community 2 is).
* `-s (--skip-header-lines) [optional integer]`: Number of header lines to skip in input files. If passed without a value, skips the first line of input files (often column names). Can be set to any number to skip that many lines.


#### Default configuration

The default configuration (if no arguments are passed, i.e., `exolabel [input edgelist files]`) is equivalent to:

```
ExoLabel [input_files] --iterations=0 --output=ExoLabel_result --tempdir=. --attenuate=0 --self-loop=0 --skip-header-lines=0
```

This uses undirected networks, no in-place sort, tab-separated files, weighted networks, no set random seed, and all normal output displayed.
