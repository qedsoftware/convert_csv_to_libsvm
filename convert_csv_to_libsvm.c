/* convert cvs data to libsvm/svm-light format */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // getopt()

char buf[10000000];
float feature[100000];

void
usage (void) {
	printf ("NAME:\n");
	printf ("\tconvert_csv_to_libsvm - Converts CSV file to LibSVM format.\n");
	printf ("\tPlace labels in first column of CSV, and features in remaining columns.\n\n");
	printf ("SYNOPSIS:\n");
	printf ("\t./convert_csv_to_libsvm -i [CSV_input_file] -o [LibSVM_output_file]\n\n");
	printf ("OPTIONS:\n");
    printf ("\t-i [CSV_input_file]\n");
    printf ("\t\tThe input CSV file.\n");
    printf ("\t-o [LibSVM_output_file]\n");
    printf ("\t\tThe output LibSVM file.\n");
    printf ("\t-m [mode]\n");
    printf ("\t\t0: Reprints classes verbatim as they appear in the CSV.\n");
    printf ("\t\t1: Forces classes in output file to be '+1' or '-1'. (Required by some binary classifiers such as LaSVM.)\n");
    printf ("\t-h\n");
    printf ("\t\tOutput a small usage guide; no other output is generated.\n");
}

int 
main(int argc, char **argv)
{
	// command-line options
	int g;
	int iflag=0, oflag=0, mflag=0, rflag=0, vflag=0;
	FILE *ifp;
	FILE *ofp;
	char *ipath = NULL;
	char *opath = NULL;
	// defaults
	int mode = 0;
	int reporting_interval = 10000;
	while ((g = getopt(argc,argv,"i:o:m:r:bhv")) != -1) {
		switch(g) {
			case 'i': {		// name of input file
				iflag=1;
				ipath = optarg;
				ifp = fopen (ipath,"r");
				if (ifp == NULL) {
					printf ("Error: Cannot open input file %s to read.\n",ipath);
					return -2;					
				}
				break;
			}
			case 'o': {		// name of output file
				oflag=1;
				opath = optarg;
				ofp = fopen (opath,"w");
				if (ofp == NULL) {
					printf ("Error: Cannot open output file %s to write.\n",opath);
					return -4;
				}
				break;
			}
			case 'r': {
				rflag=1;
				reporting_interval = atoi(optarg);
				break;				
			}
			case 'm': {
				mflag=1;
				mode = atoi(optarg);
				break;				
			}
			case 'v': {
				vflag=1;
				break;				
			}
			case 'h': {
				usage ();
				return 1;
				break;
			}
			case '?': {
				usage ();
				return 1;
				break;
			}
			default: {
				usage ();
				return 1;
			}			
		}
	}
	// argument checking
	if (0==iflag && 0==oflag) {
		usage();
		return 1;
	}
	if (0==iflag) {
		printf("Please specify an input file with the -i option.\n");
		return 1;
	}
	if (0==oflag) {
		printf("Please specify an output file with the -o option.\n");
		return 1;
	}

	int line_count = 0;
	while(fscanf(ifp,"%[^\n]\n",buf)==1)
	{
		if (rflag && (line_count > 0) && (0 == line_count % reporting_interval)) {
			printf("Processed %d lines ...\n", line_count);
		}

		int i=0,j;
		char *p=strtok(buf,",");		
		feature[i++]=atof(p);
		while((p=strtok(NULL,",")))
			feature[i++]=atof(p);

		// class labels
		switch (mode) {
		case 0: // default
			fprintf(ofp,"%d ", (int) feature[0]);		
			break;
		case 1: // "+1" or "-1"
			if ((int) feature[0]==1)
				fprintf(ofp,"+1 ");
			else
				fprintf(ofp,"-1 ");
			break;
		default:
			printf("Invalid mode choice.\n");
			break;
		}

		// features
		for(j=1;j<i;j++)
			fprintf(ofp," %d:%f",j,feature[j]);

		fprintf(ofp,"\n");
		line_count++;
	}

	fclose(ifp);
	fclose(ofp);
	return 0;
}
