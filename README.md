# DSP_HW3

* This work generates a dictionary between Zhuyin (symbols of pronunciation of Chinese characters) and Chinese characters, and transforms the Zhuyin symbols into Chinese characters.

### Usage
* Unzip the file of “data”.
* Command line: python mapping.py
* Command line: make
* Command line: ./mydisambig -text testdata/1.txt -map mapping.py -lm bigram.lm -order 2 > result2/1.txt

-- 1 can be replaced with 2~10.

### Footnote
* For more details, please refer to the file dsp_hw3.pdf.
