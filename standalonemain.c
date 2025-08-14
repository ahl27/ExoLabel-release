//#include "ExoLabel/ExoLabel.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

//#ifdef _GNU_SOURCE
#include <getopt.h>
//#endif

void stop_invalid_input(const char* argname, const char* argval){
  fprintf(stderr, "Error: Invalid input '%s' to argument '%s'.\n", argval, argname);
  exit(1);
}

void display_help(){
  printf("Usage: ExoLabel [input_files] [-adhiIlosStuV]\n\n");
  printf("Additional optional arguments:\n");

  printf("  Flags:\n");
  printf("    -d (--directed): treat input edgelist files as directed.\n");
  printf("    -h (--help): display this message.\n");
  printf("    -I (--inplace-sort): use in-place external merge sort.\n");
  printf("    -S (--space-separated): read space-separated input instead of TSVs.\n");
  printf("    -u (--unweighted): treat input edgelist files as unweighted.\n");
  printf("    -V (--silent): suppress all output.\n");
  printf("\n");

  printf("  Required parameter args:\n");
  printf("    -i (--iterations) [integer]: set iterations to value.\n");
  printf("    -o (--output) [filename]: set name of output file.\n");
  printf("    -t (--tempdir) [dirname]: set directory to be used for temp files.\n");
  printf("\n");

  printf("  Optional parameter args:\n");
  printf("    -a (--attenuate) [optional float]: set attenuation to value\n");
  printf("    -l (--self-loop) [optional float]: set self-loops to value.\n");
  printf("    -s (--skip-header-lines) [optional integer]: skip header lines in file.\n");
  printf("\n");

  printf("  Optional parameter args treat missing arguments as zero, e.g.:\n");
  printf("   `ExoLabel [input files]`                 -> attenuation disabled\n");
  printf("   `ExoLabel [input files] --attenuate`     -> attenuation is 1.0\n");
  printf("   `ExoLabel [input files] --attenuate=2.0` -> attenuation is 2.0\n");
  printf("\n");

  printf("Invalid values (e.g., --self-loop=xyz) default to zero.\n\n");

  printf("Default configuration (i.e., `ExoLabel [input_files]`):\n");
  printf("ExoLabel [input_files] \\\n");
  printf("  --iterations=0 --output=ExoLabel_result \\\n");
  printf("  --tempdir=. --attenuate=0 --self-loop=0 --skip-header-lines=0\n");
  printf("undirected networks, no in-place sort, tab-separated files,\n");
  printf("weighted networks, all normal output displayed.");
  printf("\n");

  exit(0);
}

int main (int argc, char *argv[]){
  // boolean values
  int ignore_weights = false,
      use_inplace_sort = false,
      is_undirected = true,
      verbose = true,
      is_space_separated = false;

  // integer values
  int num_iter = 0,
      skip_header_lines = 0;

  // double values
  double atten_power = 0.0,
         self_loop_weights = 0.0;

  // character values
  const char *dir = "./",
            *output_name = NULL;

  /*
   * struct option has the following fields:
   *  - const char *name
   *  - int has_arg [no_argument, required_argument, optional_argument]
   *  - int *flag (reference to place to store value)
   *              (if NULL, returns val from the call)
   *  - int val (value to return, or to load into value pointed by flag)
   */
  struct option fields[] = {
    /* flag values */
    {"unweighted", no_argument, &ignore_weights, true},
    {"inplace-sort", no_argument, &use_inplace_sort, true},
    {"directed", no_argument, &is_undirected, false},
    {"silent", no_argument, &verbose, false},
    {"space-separated", no_argument, &is_space_separated, true},

    /* fields storing a value (passing NULL to forward control to switch) */
    {"iterations", required_argument, NULL, 'i'},
    {"skip-header-lines", optional_argument, NULL, 's'},
    {"tempdir", required_argument, NULL, 't'},
    {"attenuate", optional_argument, NULL, 'a'},
    {"self-loop", optional_argument, NULL, 'l'},
    {"output", required_argument, NULL, 'o'},
    {"help", no_argument, NULL, 'h'},
    {0,0,0,0}
  };

  int c;
  while(true){
    int argindex = 0;
    c = getopt_long(argc, argv, ":a:dhi:Il:o:s:St:uV", fields, &argindex);

    if(c == -1) break; // end of argument string

    /*
     * note that atof and atoi default to 0 if the input is invalid,
     * which is honestly fine behavior for me.
     */
    switch (c){

    case 0: // set an option -- do nothing and break
      break;

    case 'a': // attenuation
      atten_power = optarg == NULL ? 1.0 : atof(optarg);
      break;

    case 'd': // directed
      is_undirected = !is_undirected;
      break;

    case 'h': // help
      display_help();

    case 'i': // iterations
      num_iter = optarg == NULL ? 0 : atoi(optarg);
      if(num_iter < 0){
        printf("Warning: Negative values for 'iterations' will be set to 0.\n");
        num_iter = 0;
      }
      break;

    case 'I': // inplace sort
      use_inplace_sort = !use_inplace_sort;
      break;

    case 'l': // self loops
      self_loop_weights = optarg == NULL ? 1.0 : atof(optarg);
      break;

    case 'o': // output name
      output_name = optarg;
      break;

    case 's': // skip header lines
      skip_header_lines = optarg == NULL ? 1 : atoi(optarg);
      break;

    case 'S': // space separated
      is_space_separated = !is_space_separated;
      break;

    case 't': // tempdir
      dir = optarg;
      break;

    case 'u': // undirected
      ignore_weights = !ignore_weights;
      break;

    case 'V': // silent
      verbose = !verbose;
      break;

    case ':': // missing parameter for argument that requires one
      fprintf(stderr, "Error: Argument '%s' requires a parameter.\n", argv[optind-1]);
      exit(1);

    case '?': // incorrect parameter
      fprintf(stderr, "Error: Unrecognized argument '%s'.\n", argv[optind-1]);
      exit(1);

    default: // should never get here
      fprintf(stderr, "Internal error, aborting...\n");
      abort();
    }
  }

  // fix some non-standard args into format expected by calling function
  const char *seps = is_space_separated ? " \n" : "\t\n";
  if(!output_name)
    output_name = is_space_separated ? "ExoLabel_result.txt" : "ExoLabel_result.tsv";

  /* Print any remaining command line arguments (not options) */
  int num_input_files = argc - optind;
  if(num_input_files < 1){
    fprintf(stderr, "Error: no input files were provided!\n");
    exit(1);
  }

  const char** input_files = malloc(sizeof(const char*) * num_input_files);
  for(int i=0; i<num_input_files; i++)
    input_files[i] = argv[optind + i];

  printf("\nBoolean parameters:\n");
  printf("ignore_weights: %d\n", ignore_weights);
  printf("use_inplace_sort: %d\n", use_inplace_sort);
  printf("is_undirected: %d\n", is_undirected);
  printf("verbose: %d\n", verbose);
  printf("is_space_separated: %d\n", is_space_separated);

  printf("\nNumeric parameters:\n");
  printf("num_iter: %d\n", num_iter);
  printf("skip_header_lines: %d\n", skip_header_lines);
  printf("atten_power: %.01f\n", atten_power);
  printf("self_loop_weights: %.01f\n", self_loop_weights);

  printf("\nCharacter parameters:\n");
  printf("tempdir: %s\n", dir);
  printf("output_name: %s\n", output_name);
  printf("separator: %s\n", seps[0] == ' ' ? "SPACE" : "TAB");

  printf("\nInput Files:\n");
  for(int i=0; i<num_input_files; i++)
    printf("  %s\n", input_files[i]);

  printf("\n");

  /*
  C_LPOOM_cluster(input_files, num_input_files,
                  dir, 1, &output_name,
                  seps, &num_iter, verbose,
                  is_undirected,
                  &self_loop_weights,
                  ignore_weights,
                  use_inplace_sort,
                  &atten_power,
                  skip_header_lines);
  */
  free(input_files);
  return 0;
}