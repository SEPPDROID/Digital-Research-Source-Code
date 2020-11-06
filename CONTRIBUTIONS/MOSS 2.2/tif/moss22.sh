#!/bin/bash
#
#This script will extrace all pages of a PDF to Sequential pages, then convert those PDF
#pages to text using TextBridge
#
#
#Larry Kraemer  09-21-2014  Chaffee, MO
#
#Splitting files
#
#Splitting PDF files with pdftk was an interesting experience. The burst option breaks a PDF into #multiple files -- one file for each page:
#
# To test the conversion try these commands:  (I have better luck with TextBridge Conversions)
# convert -monochrome -density 300 pg_0001.pdf pg001.tif
# tesseract pg001.tif pg001 -psm 6
# OR Use Textbridge to convert tif files to TXT as spreadsheet or table
#
#
PAGES=24		# set to the number of PDF pages to process
SOURCE=MOSS22.pdf	# set to the file name of the multipage PDF
#OUTPUT=moss22.txt	# creates the final "output file"
RESOLUTION=400		# set to the resolution the scanner used (the higher, the better)
#
touch $OUTPUT
for i in `seq 1 $PAGES`; do
    convert -monochrome -density $RESOLUTION $SOURCE\[$(($i - 1 ))\] page$i.tif
#    tesseract page$i.tif page$i -psm 6
#    cat $OUTPUT page$i.txt > temp.txt
#    rm $OUTPUT
#    rm page$i.tif
#    rm page$i.txt
#    mv temp.txt $OUTPUT
#
#  OR Use Textbridge to convert tif files to TXT as spreadsheet or table
#
done

