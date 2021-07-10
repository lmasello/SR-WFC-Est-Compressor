# SR-WFC-Est-Compressor
A data compression algorithm that combines the Symbol Ranking, Weighted Frequency Count and Arithmetic Coding algorithms, reaching a compression rate of 2.45 bits per Byte on the [Calgary Corpus](https://en.wikipedia.org/wiki/Calgary_corpus). The project has been part of the semestral project of the subject ["Organización de datos"](http://www.fi.uba.ar/sites/default/files/7506.pdf) of the University of Buenos Aires, Faculty of Engineering (first semester of 2014). 

The algorithm has been based on the following papers:
- [Fenwick, P. M. (1997). Symbol ranking text compression with Shannon recodings. Journal of Universal Computer Science, 3(2), 70-85.](http://www.jucs.org/jucs_3_2/symbol_ranking_text_compression/fenwick_p.pdf)
- [Fenwick, P. (1997). A fast, constant-order, symbol ranking text compressor. Department of Computer Science, The University of Auckland, New Zealand.](http://cs.auckland.ac.nz/~peter-f/FTPfiles/TechRep145.ps)

## Usage options
Compress:
```bash
c fileIn
# output file: fileIn.sr
```

Decompress:
```bash
d fileIn.sr
```

## Performance reports
Pending performance reports (on memory usage and speed) and table charts comparing the compressor with well-known algorithms.
