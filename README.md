# Single Document Text Summarization with LexRank

## Requirements
1. g++ 5.4.0 and above with full C++14 support
2. cmake 3.2.2 and above
3. dirent.h header to retrieve file information. This normally comes installed with
C POSIX library; therefore, you don't need to install any external library.
However, if you are on Windows and compiling with MSVC, you need to obtain this
header manually.
4. doxygen (optional)

## Build
To build the project, run the following commands in the project root directory

```
chmod +x build.sh
./build.sh release
```

This will build the project and create two executables: idf and lexrank.

### Build Options
You can build the project in debug mode if you want to debug its execution trace
by running
```
./build.sh debug
```

If you want to get rid of all build files, run
```
./build.sh clean
```

## Documentation
You can view the documentation in the source files. Optionally, you can build
the project documentation using doxygen and view it in your browser. To build the
documentation, doxygen must be installed on your system. After installing doxygen,
type the following command in the project root directory to build the documentation:

```
./build.sh doc
```

To view the documentation, open ```doc/html/index.html``` file with your
browser.

## Running
The build process creates two executables:

### idf
This is the executable to compute idf score of each normalized term in the given
document corpus. This step is done with a separate executable to reduce the
running time of subsequent summarization operations done with lexrank
executable.

idf executable expects the path to the directory containing the documents. The
given dataset directory, Dataset, must have the following layout:

```
Dataset
├── 1000.txt
├── 100.txt
├── 101.txt
...
```

where each file's name is of the form <id>.txt .

idf executable creates a file named idf.txt containing terms and their
idf scores.

### lexrank
lexrank is the executable to compute the lexrank score of each sentence in a
given document and print the document summary using the 3 sentences with the
highest LexRank scores. lexrank executable expects to find idf.txt in the
current directory (run directory). Therefore, a simple way to use both
executables is placing them in the same directory and running them both from
that directory.

To run lexrank executable on a document (say 123.txt), type

```
./lexrank Dataset_path 123.txt
```

where Dataset_path is the path to the Dataset folder containing 123.txt .

After the executable finishes successfully, LexRank score of each sentence is
printed on a separate line, in the same order. Additionally, sentences with the
top 3 LexRank scores are printed consecutively as the document summary.

# ROUGE Scores
ROUGE scores are given in the report. Additionally, you can run the scoring
script to generate average ROUGE scores on a custom dataset. To do this, you
need to run rouge_scores.py script under scoring directory.

## Steps
Before running scoring script, you need to build the C++ executables and create
idf file by running idf executable. Afterwards, go to scoring directory. Then run

```
chmod +x setup.sh
./setup.sh
source environment/bin/activate
python rouge_scores.py
```

This script will compute summaries using lexrank for each document and compare
the generated summary with the golden standard. Then, ROUGE scores will be
averaged and printed to STDOUT as a python dictionary.

## Known Bugs
From time to time, lexrank executable crashes when run with the provided scoring
script. I couldn't solve this bug; but a simple workaround is running the script
again when it crashes. It usually works on the second or third try. Sorry for
the inconvenience.
